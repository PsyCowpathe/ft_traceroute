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
#include <stdio.h>

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
	struct timeval	timeout = TIMEOUT;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		error_exit(NULL, 1, false, "Sockopt failure !\n");
		exit(1);
	}
	return socket_fd;
}

int		create_socket(char *socket_type)
{
	int socketmp;

	if (strcmp(socket_type, "UDP") == 0)
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
	params->destination.sin_port = htons(params->current_port);
	params->current_ttl = params->hop_start;
	while (i < params->packet_len)
	{
		params->packet[i] = i + '0';
		i++;
	}
}

void	update_socket(t_parameters *params)
{
	if (setsockopt(params->udp_socket, IPPROTO_IP, IP_TTL, &params->current_ttl, sizeof(params->current_ttl)))
		error_exit(params, 1, false, "Sockopt failure for TTL !\n");
}

void	send_probe(t_parameters *params)
{
	int	sent_bytes;
	
	sent_bytes = sendto(params->udp_socket, &params->packet,params->packet_len, 0,
		(struct sockaddr *)&params->destination, sizeof(params->destination));
	if (sent_bytes < 0)
	{
		perror("Could not send packet ! ");
	}
	gettimeofday(&params->start, 0);
}

void	save_one_probe(t_parameters *params, uint32_t probes_count, struct sockaddr *answerer)
{
	long double	rtt = (params->end.tv_sec - params->start.tv_sec) * 1000000.0 + params->end.tv_usec - params->start.tv_usec;
        rtt /= 1000;

	snprintf(
    	params->probe_infos[probes_count].ip_result,
    	INET_ADDRSTRLEN,
    	"%s",
    	inet_ntoa(((struct sockaddr_in *)answerer)->sin_addr));
	params->probe_infos[probes_count].probe_time = rtt;
	bzero(params->probe_infos[probes_count].dns, NI_MAXHOST);
	if (params->rdns == true)
		reverse_dns_lookup(params, params->probe_infos[probes_count].dns, params->probe_infos[probes_count].ip_result);
	else
		strncpy(params->probe_infos[probes_count].dns, params->probe_infos[probes_count].ip_result, strlen(params->probe_infos[probes_count].ip_result));

	if (((struct sockaddr_in *)answerer)->sin_addr.s_addr == params->destination.sin_addr.s_addr)
    	params->destination_reached = true;
}

void	receive_response(t_parameters *params, uint32_t	probes_count)
{
	int			received_bytes;
	char		buffer[MAX_PACKET_SIZE];
	struct		sockaddr_in	answerer;
	socklen_t	answerer_size = sizeof(answerer);

	received_bytes = recvfrom(params->icmp_socket, &buffer, sizeof(buffer), 0,
						(struct sockaddr *)&answerer, &answerer_size);
	gettimeofday(&params->end, 0);

	if (received_bytes > 0)
	{
		save_one_probe(params, probes_count, (struct sockaddr *)&answerer);
	}
	else
	{
		params->probe_infos[probes_count].probe_time = -1;
	}
}

void	ft_traceroute(t_parameters *params)
{
	uint32_t	probes_count;

	params->current_port = params->starting_port;
	params->udp_socket = create_socket("UDP");
	params->icmp_socket = create_socket("ICMP");
	create_packet(params);
	init_probes_saving(params);

	printf(PRINT_START, params->string_original_target, params->string_ip_address, params->ttl_max, params->packet_len);
	while (params->destination_reached == false && params->current_ttl <= params->ttl_max)
	{
		probes_count = 0;
		update_socket(params);
		while (probes_count < params->probes_per_hop)
		{
			send_probe(params);
			receive_response(params, probes_count);
			increment_port(params);
			probes_count++;
		}
		print_one_hop(params);
		params->current_ttl++;
	}
	free(params->probe_infos);
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