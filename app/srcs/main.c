/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/06/05 22:25:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

/*unsigned short	checksum(void *b, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	result;

	buf = b;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}*/

/*void	create_header(t_parameters *params, t_pcket *packet)
{
	int				i;

	i = 0;
	packet->send_header.code = 0;
	packet->send_header.type = ICMP_ECHO;
	packet->send_header.un.echo.id = getpid();
	fflush(stdout);
	packet->send_header.un.echo.sequence = params->iteration - 1;
	while (i < params->paquet_size)
	{
		packet->message[i] = i + '0';
		i++;
	}
	packet->send_header.checksum = 0;
	packet->send_header.checksum = checksum(packet,
			sizeof(struct icmphdr) + (params->paquet_size));
}*/

int		set_sockopt(int socket_fd)
{
	struct timeval	timeout = {5, 0};
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		error_exit(1, false, "Sockopt failure !\n");
		exit(1);
	}
	return socket_fd;
}

int		create_socket(char *socket_type)
{
	int socketmp;

	if (ft_strcmp(socket_type, "UDP") == 0)
	{
		socketmp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else
	{
		socketmp = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	}
	if (socketmp < 0)
	{
		printf("socket type = %s", socket_type);
		perror(SOCKET_CREATION_ERROR);
		exit(1);
	}
	socketmp = set_sockopt(socketmp);
	return socketmp;
}

void	create_packet(t_parameters *params)
{
	int		i = 0;

	params->destination = *(struct sockaddr_in *)params->ip_address->ai_addr;
	params->destination.sin_port = htons(PORT); //todo : handle port increment
	params->current_ttl = 1;
	while (i < params->packet_len)
	{
		params->packet[i] = i + '0';
		i++;
	}
}

void	update_socket(t_parameters *params)
{
	if (setsockopt(params->udp_socket, IPPROTO_IP, IP_TTL, &params->current_ttl, sizeof(params->current_ttl)))
		error_exit(1, false, "Sockopt failure for TTL !\n");
}

void	send_probe(t_parameters *params)
{
	int	sent_bytes;
	
	sent_bytes = sendto(params->udp_socket, &params->packet,params->packet_len, 0,
		(struct sockaddr *)&params->destination, sizeof(params->destination));
	if (sent_bytes < 0)
	{
		perror("Could not send packet ! "); // TODO : interdit
	}
	//fill start
	gettimeofday(&params->start, 0);
}

void	print_one_step(t_parameters *params, struct sockaddr *answerer)
{
	char		ip_result[NI_MAXHOST];
	long double	rtt = (params->end.tv_sec - params->start.tv_sec) * 1000000.0 + params->end.tv_usec - params->start.tv_usec;
        rtt /= 1000;

	snprintf(
    	ip_result,
    	INET_ADDRSTRLEN,
    	"%s",
    	inet_ntoa(((struct sockaddr_in *)answerer)->sin_addr));
	printf(PRINT_STEP, params->current_ttl, ip_result, rtt);

	if (ft_strcmp(ip_result, params->string_ip_address) == 0)
		params->destination_reached = true;
}

void	interpret_response(t_parameters *params, struct sockaddr *answerer, char *buffer)
{
	struct icmphdr		*receive_header;
	struct ip			*ip_header;

	ip_header = (struct ip *)buffer;
	receive_header = (struct icmphdr *)((char *)buffer
		+ (ip_header->ip_hl * 4));
	
	if (receive_header->type == 11)
	{
		print_one_step(params, answerer);
	}
	else if (receive_header->type == 3)
	{
		printf(HOST_UNREACHABLE);
	}
	else
	{
		printf("answer header type = %d", receive_header->type); //todo : handle
	}
}

void	receive_response(t_parameters *params)
{
	int			received_bytes;
	char		buffer[MAX_PACKET_SIZE];
	struct		sockaddr_in	answerer;
	socklen_t	answerer_size = sizeof(answerer);

	printf("waiting");
	fflush(stdout);
	received_bytes = recvfrom(params->icmp_socket, &buffer, sizeof(buffer), 0,
						(struct sockaddr *)&answerer, &answerer_size);
	gettimeofday(&params->end, 0);


	if (received_bytes > 0)
	{
		interpret_response(params, (struct sockaddr *)&answerer, buffer);
	}
	else if (received_bytes == 0)
	{
		printf("no response received\n"); //todo : handle
	}
	else
	{
		printf("error while receiving !\n");
		printf("%s\n", strerror(errno));
	}
}

static double    get_time_seconds(void)
{
    struct    timeval    t;

    gettimeofday(&t, 0);
    return (t.tv_sec + t.tv_usec / 1e9);
}

void	ft_traceroute(t_parameters *params)
{
	uint32_t	probes_count;
	double		current_time = 0.0;
	double		last_time = 0.0;

	params->udp_socket = create_socket("UDP");
	params->icmp_socket = create_socket("ICMP");
	create_packet(params);

	while (params->destination_reached == false && params->current_ttl <= params->ttl_max)
	{
		probes_count = 0;
		update_socket(params);
		while (probes_count < params->probes_per_hop)
		{
			current_time = get_time_seconds();
			if (current_time - last_time > params->probe_interval || last_time == 0.0)
			{
				last_time = current_time;
				send_probe(params);
				receive_response(params);
				probes_count++;
			}
		}
		params->current_ttl++;
	}
}

int     main(int argc, char **argv)
{
    t_parameters	params;

	if (argc >= 2)
	{
		init_flag_structure(&params);
		if (parse_args(argv, argc, &params) == -1)
			return (1);
		ft_traceroute(&params);
		return (0);
	}
	else
	{
        print_help_menu();
	}
	return (0);
}