/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 22:01:13 by agirona           #+#    #+#             */
/*   Updated: 2026/06/12 11:24:34 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"
#include <stdio.h>

void	print_help_menu(void)
{
	printf("Usage:\n");
    printf("\tft_traceroute [OPTION...] HOST [ packetlen ]\n\n");
	printf("Default options:\n");
	printf("--help\t\t\tRead this help and exit\n\n");
	printf("Bonus options :\n");
	printf("-m, --m\t\t\tttl max (default = 30)\n");
	printf("-f, --f\t\t\tStart from the N hop (instead from 1)\n");
	printf("-q, --q\t\t\tSet the number of probes per each hop. Default is 3\n");
	printf("-rdns, --rdns\t\tResolve IP addresses to their domain names\n");
	printf("-p, --p\t\t\tStarting port\n\n");
	printf("Arguments:\n");
	printf("host\t\t\tThe host to traceroute to\n");
	printf("packetlen\t\tThe full packet length (default is the length of an IP\n");
    printf("\t\t\theader plus 40). Can be ignored or increased to a minimal\n");
    printf("\t\t\tallowed value\n");
	exit(0);
}

void	print_one_hop(t_parameters *params)
{
	uint32_t		i;
	char			last_ip[NI_MAXHOST];

	i = 0;
	bzero(last_ip, NI_MAXHOST);
	printf("%2d ", params->current_ttl);
	while (i < params->probes_per_hop)
	{
		if (params->probe_infos[i].probe_time == -1)
		{
			printf("* ");
		}
		else
		{
			if (strcmp(last_ip, params->probe_infos[i].ip_result) == 0)
			{
				printf("%.3LF ms  ", params->probe_infos[i].probe_time);
			}
			else
			{
				printf("%s (%s) %.3LF ms  ", params->probe_infos[i].dns, params->probe_infos[i].ip_result, params->probe_infos[i].probe_time);
			}
			strncpy(last_ip, params->probe_infos[i].ip_result, strlen(params->probe_infos[i].ip_result));
		}
		i++;
	}
	printf("\n");
}

void	error_exit(t_parameters *params, int code, bool print_try, const char *msg, ...)
{
	va_list	args;
	int		len;
	char	*fmt_msg;

	va_start(args, msg);
	len = vsnprintf(NULL, 0, msg, args);
	va_end(args);
	if (len < 0)
		exit (67);
	fmt_msg = malloc(sizeof(char) * (len + 1));
	if (!fmt_msg)
		exit (68);
	va_start(args, msg);
	vsnprintf(fmt_msg, len + 1, msg, args);
	va_end(args);
	if (print_try == true)
		printf(ERROR_PRINT_TRY, fmt_msg);
	else
		printf(ERROR_PRINT, fmt_msg);
	free(fmt_msg);
	free_malloced_var(params);
	exit(code);
}
