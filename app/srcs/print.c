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
	printf("-z, --z\t\t\tMinimal time interval between probes (default 0).\n");
	printf("\t\t\tIf the value is more than 10, then it specifies a\n");
	printf("\t\t\tnumber in milliseconds, else it is a number of\n");
	printf("\t\t\tseconds (float point values allowed too)\n\n");
	printf("Arguments:\n");
	printf("host\t\t\tThe host to traceroute to\n");
	printf("packetlen\t\tThe full packet length (default is the length of an IP\n");
    printf("\t\t\theader plus 40). Can be ignored or increased to a minimal\n");
    printf("\t\t\tallowed value\n");
	exit(0);
}


void	error_exit(int code, bool print_try, const char *msg, ...)
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
	exit(code);
}
