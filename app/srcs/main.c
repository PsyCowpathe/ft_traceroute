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

int		set_udp_sockopt(int socket_fd)
{
	struct timeval	timeout = {5, 0};
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		error_exit(1, false, "Sockopt failure for UDP socket !\n");
		exit(1);
	}
	return socket_fd;
}

int		create_socket(t_parameters *params)
{
	
	params->socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (params->socket_fd < 0)
	{
		perror("Can't create socket ! ");
		exit(1);
	}
	params->socket_fd = set_udp_sockopt(params->socket_fd);
	return params->socket_fd;
}

void	create_packet(t_parameters *params)
{
	int		i = 0;

	params->destination = *(struct sockaddr_in *)params->ip_address->ai_addr;
	params->destination.sin_port = htons(PORT);
	params->current_ttl = 1;
	while (i < params->packet_len)
	{
		params->packet[i] = i + '0';
		i++;
	}
}

void	update_socket(t_parameters *params)
{
	if (setsockopt(params->socket_fd, IPPROTO_IP, IP_TTL, &params->current_ttl, sizeof(params->current_ttl)))
		error_exit(1, false, "Sockopt failure for TTL !\n");
}

void	send_probe(t_parameters *params)
{
	int	sent_bytes;
	
	sent_bytes = sendto(params->socket_fd, &params->packet,params->packet_len, 0,
		(struct sockaddr *)&params->destination, sizeof(params->destination));
	if (sent_bytes < 0)
	{
		perror("Could not send packet ! ");
	}
	else
	{
		printf("Probe sent !");
	}
}

void	receive_response(t_parameters *params)
{
	int			received_bytes;
	char		buffer[MAX_PACKET_SIZE];
	struct		sockaddr_in	answerer;
	socklen_t	answerer_size = sizeof(answerer);

	received_bytes = recvfrom(params->socket_fd, &buffer, sizeof(buffer), 0,
						(struct sockaddr *)&answerer, &answerer_size);
	if (received_bytes > 0)
	{
		printf("Answer received");
	}
	else if (received_bytes == 0)
	{
		printf("no response received");
	}
	else
	{
		printf("error while receiving !");
	}
}

void	ft_traceroute(t_parameters *params)
{
	int		current_index = 0;

	create_socket(params);
	create_packet(params);

	while (current_index < 10)
	{
		update_socket(params);
		send_probe(params);
		receive_response(params);
		current_index++;
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