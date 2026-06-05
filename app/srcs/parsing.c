/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 22:01:13 by agirona           #+#    #+#             */
/*   Updated: 2026/06/12 11:24:34 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

char	*parse_flag_identifier(char *to_parse)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (to_parse[i] != '\0' && to_parse[i] == '-')
	{
		count++;
		i++;
	}
	if (count >= 3)
		error_exit(1, true, UNRECOGNIZED, to_parse);
	return (to_parse + i);
}

int	get_neg_index(char *flag_value, int *current_index)
{
	int	i;

	i = 0;
	while (isspace(flag_value[i]))
		i++;
	*current_index = i;
	if (flag_value[i] == '-')
		return (i);
	return (-1);
}

int	check_is_float(char *flag_value)
{
	int	i;
	int	neg_i;
	int	dot_i;

	neg_i = get_neg_index(flag_value, &i);
	dot_i = -1;
	if (flag_value[i] == '\0')
		return (-1);
	while (flag_value[i])
	{
		if (isdigit(flag_value[i]) == 0)
		{
			if (flag_value[i] == '.')
			{
				if (dot_i == -1)
					dot_i = i;
				else
					return (-1);
			}
			if (i != neg_i && i != dot_i)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	check_is_int(char *flag_value)
{
	int	i;
	int	neg_index;

	neg_index = get_neg_index(flag_value, &i);
	if (flag_value[i] == '\0')
		return (-1);
	while (flag_value[i])
	{
		if (isdigit(flag_value[i]) == 0)
		{
			if (i != neg_index)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	verify_flag_value(char *flag_id, char *flag_value)
{
	if (flag_value[0] == '-' && flag_value[1] == '\0')
	{
		return (-1);
	}
	if (flag_value[0] == '\0')
		return (0);
	if (ft_strcmp(flag_id, "z") == 0)
	{
		if (check_is_float(flag_value) == -1)
			return (-1);
	}
	else
	{
		if (check_is_int(flag_value) == -1)
			return (-1);
	}
	return (0);
}

int	verify_flag_limits(t_parameters *params)
{
	(void)params;
	printf("todo : implementer la vérification des limits des flags");
	/*if (params->time_to_live == 0)
		error_exit(1, false, TOO_SMALL, params->string_time_to_live);
	else if (params->time_to_live > TTL_MAX || params->time_to_live < TTL_MIN)
		error_exit(1, false, TOO_BIG, params->string_time_to_live);
	if (params->timeout == 0)
		error_exit(1, false, TOO_SMALL, params->string_timeout);
	else if (params->timeout < 0)
		error_exit(1, false, TOO_BIG, params->string_timeout);
	if (params->interval < 0.2)
	{
		if (getuid() != 0)
			error_exit(1, false, TOO_SMALL, params->string_interval);
	}
	if (params->paquet_size > PACKET_MAX_SIZE || params->paquet_size < 0)
		error_exit(1, false, TOO_BIG, params->string_paquet_size);
	if (params->interval < 0)
		error_exit(1, false, BAD_TIMING_INTERVAL, params->string_interval);
	if (params->count == 0)
		params->count = UINT32_MAX;*/
	return (0);
}

int	verify_flag_identifier(char *flag_id)
{
	if (ft_strcmp(flag_id, "m") == 0)
		return (0);
	if (ft_strcmp(flag_id, "f") == 0)
		return (0);
	if (ft_strcmp(flag_id, "q") == 0)
		return (0);
	if (ft_strcmp(flag_id, "z") == 0)
		return (0);
	return (-1);
}

int	parse_flag(t_parameters *params, char **args, int argc, int index)
{
	char	*flag_id;
	char	*flag_value;

	flag_id = parse_flag_identifier(args[index]);
	if (ft_strcmp(flag_id, "help") == 0)
		print_help_menu();
	if (ft_strcmp(flag_id, "rdns") == 0)
	{
		params->rdns = true;
		return (1);
	}
	index++;
	if (index == argc)
		error_exit(1, true, MISSING_VALUE, flag_id);
	flag_value = args[index];
	if (verify_flag_identifier(flag_id) == -1)
		error_exit(1, true, INVALID_OPTION, flag_id);
	if (verify_flag_value(flag_id, flag_value) == -1)
		error_exit(1, true, INVALID_VALUE, flag_id, flag_value);
	store_flag(params, flag_id, flag_value);
	verify_flag_limits(params);
	return (0);
}

int	parse_args(char **args, int argc, t_parameters *params)
{
	int	i;
	int	ret;

	i = 1;
	while (i < argc)
	{
		if (args[i][0] == '-')
		{
			ret = parse_flag(params, args, argc, i);
			if (ret == -1)
				return (-1);
			if (ret == 0)
				i++;
		}
		else if (params->string_original_target == NULL)
		{
			params->string_original_target = args[i];
		}
		i++;
	}
	verify_target_address(params);
	return (0);
}