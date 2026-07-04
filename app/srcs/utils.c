/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 22:02:24 by agirona           #+#    #+#             */
/*   Updated: 2026/06/12 11:28:12 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

void	store_flag(t_parameters *params, char *flag_id, char *flag_value)
{
    if (ft_strcmp(flag_id, "m") == 0)
    {
    	params->ttl_max = atoi(flag_value);
    	params->string_ttl_max = flag_value;
    }
    else if (ft_strcmp(flag_id, "f") == 0)
    {
    	params->hop_start = atoi(flag_value);
    	params->string_hop_start = flag_value;
    }
    else if (ft_strcmp(flag_id, "q") == 0)
    {
    	params->probes_per_hop = atoi(flag_value);
    	params->string_probes_per_hop = flag_value;
    }
    else if (ft_strcmp(flag_id, "z") == 0)
    {
    	params->probe_interval = atof(flag_value); //todo : handle more than 10 (look the man)
    	params->string_probe_interval = flag_value;
    }
}

void	init_flag_structure(t_parameters *params)
{
	ft_bzero(params, sizeof(t_parameters));
	params->ttl_max = 30;
	params->hop_start = 1;
	params->probes_per_hop = 3;
	params->rdns = false;
	params->probe_interval = 0;
	params->dns_name[0] = '\0';
	params->packet_len = DEFAULT_PACKET_SIZE;
	params->destination_reached = false;
}