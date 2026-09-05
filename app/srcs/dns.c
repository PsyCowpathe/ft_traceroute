/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 19:48:10 by agirona           #+#    #+#             */
/*   Updated: 2026/06/05 20:42:43 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"
#include <netdb.h>
#include <stdio.h>

//convert DNS en IP
void	dns_lookup(t_parameters *params)
{
	struct addrinfo	hints;
	struct addrinfo	*result;
	int				ret;

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(params->string_original_target, NULL, &hints, &result);
	if (ret != 0)
		error_exit(params,1, false, UNKNOW_HOST, params->string_original_target, gai_strerror(ret), params->string_original_target);
	params->ip_address = (struct addrinfo *)result;
    snprintf(
    params->string_ip_address,
    INET_ADDRSTRLEN,
    "%s",
    inet_ntoa(((struct sockaddr_in *)params->ip_address->ai_addr)->sin_addr));
}

// convert IP en DNS
void	reverse_dns_lookup(t_parameters *params, char *result, char *ip)
{

	struct sockaddr_in	tmp_addr;
	socklen_t			len;
	int					ret;

	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_addr.s_addr = inet_addr(ip);
	len = sizeof(struct sockaddr_in);
	ret = getnameinfo((struct sockaddr *)&tmp_addr, len,
			result, NI_MAXHOST, NULL, 0,
			NI_NAMEREQD);
	if (ret == EAI_NONAME)
	{
		strncpy(result, ip, strlen(ip));
	}
	else if (ret != 0)
	{
		error_exit(params, 1, false, gai_strerror(ret));
	}
}

void	verify_target_address(t_parameters *params)
{
	if (params->string_original_target == NULL)
		error_exit(params, 1, true, MISSING_HOST);
	dns_lookup(params);
}
