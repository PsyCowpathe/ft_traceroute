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

//convert DNS en IP
void	dns_lookup(t_parameters *params)
{
	struct addrinfo	hints;
	struct addrinfo	*result;
	int				ret;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(params->string_original_target, NULL, &hints, &result);
	if (ret != 0)
		error_exit(1, false, UNKNOW_HOST, params->string_original_target, gai_strerror(ret), params->string_original_target); //TODO : TESTER
	params->ip_address = (struct addrinfo *)result;
    snprintf(
    params->string_ip_address,
    INET_ADDRSTRLEN,
    "%s",
    inet_ntoa(((struct sockaddr_in *)params->ip_address->ai_addr)->sin_addr));
}

// convert IP en DNS
void	reverse_dns_lookup(t_parameters *params)
{
	struct sockaddr_in	tmp_addr;
	socklen_t			len;
	int					ret;

	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_addr.s_addr = inet_addr(params->string_ip_address);
	len = sizeof(struct sockaddr_in);
	ret = getnameinfo((struct sockaddr *)&tmp_addr, len,
			params->dns_name, sizeof(params->dns_name), NULL, 0,
			NI_NAMEREQD);
	if (ret == 0)
		ft_strncpy(params->dns_name, params->string_ip_address, NI_MAXHOST);
	else
		error_exit(1, false, gai_strerror(ret)); //TODO : TESTER

}

void	verify_target_address(t_parameters *params)
{
	if (params->string_original_target == NULL)
		error_exit(1, true, MISSING_HOST);
	dns_lookup(params);
	ft_strncpy(params->dns_name, params->string_original_target, NI_MAXHOST);
	if (params->rdns == true)
		reverse_dns_lookup(params);
}
