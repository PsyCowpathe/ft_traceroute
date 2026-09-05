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
    if (strcmp(flag_id, "m") == 0)
    {
    	params->ttl_max = atoi(flag_value);
    	params->string_ttl_max = flag_value;
    }
    else if (strcmp(flag_id, "f") == 0)
    {
    	params->hop_start = atoi(flag_value);
    	params->string_hop_start = flag_value;
    }
    else if (strcmp(flag_id, "q") == 0)
    {
    	params->probes_per_hop = atoi(flag_value);
    	params->string_probes_per_hop = flag_value;
    }
    else if (strcmp(flag_id, "p") == 0)
    {
    	params->starting_port = atoi(flag_value);
    	params->string_starting_port = flag_value;
    }
}

void	increment_port(t_parameters *params)
{
	if (params->current_port == UINT16_MAX)
	{
		params->current_port = params->starting_port;
	}
	else
	{
		params->current_port++;
	}
	params->destination.sin_port = htons(params->current_port);
}

void	init_probes_saving(t_parameters *params)
{
	params->probe_infos = malloc(sizeof(*params->probe_infos) * params->probes_per_hop);
	if (params->probe_infos == NULL)
	{
		error_exit(params, 1, false, MALLOC_ERROR);
	}
}

void	free_malloced_var(t_parameters *params)
{
	if (params == NULL)
		return;
	if (params->probe_infos != NULL)
		free(params->probe_infos);
}

void	init_flag_structure(t_parameters *params)
{
	bzero(params, sizeof(t_parameters));
	params->ttl_max = 30;
	params->hop_start = 1;
	params->probes_per_hop = 3;
	params->rdns = false;
	params->starting_port = PORT;
	params->packet_len = DEFAULT_PACKET_SIZE;
	params->destination_reached = false;
	params->probe_infos = NULL;
}