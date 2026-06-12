/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:08:53 by agirona           #+#    #+#             */
/*   Updated: 2026/04/11 19:50:42 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ctype.h>
# include <netinet/ip_icmp.h>
# include <stddef.h>
# include <netdb.h>
# include <arpa/inet.h>

/****************** LIMITS ******************/

# define MAX_HOPS 255
# define MAX_PROBES 10


/****************** ERRORS ******************/

# define ERROR_PRINT_TRY "ft_traceroute: %s\n\
Try \'ft_traceroute --help\' for more information.\n"
# define ERROR_PRINT "ft_traceroute: %s\n"
# define UNKNOW_HOST "%s: Temporary failure in name resolution\n\
Cannot handle \"host\" cmdline arg `%s'"
# define MISSING_HOST "Specify \"host\" missing argument."
# define MISSING_VALUE "Option `-%s' requires an argument."
# define INVALID_OPTION "Bad option `-%s'"
# define INVALID_VALUE "Cannot handle `-%s' option with arg '%s'"
# define UNRECOGNIZED "%s: Name or service not known\n\
Cannot handle \"host\" cmdline arg `%s'"
# define INVALID_PACKETLEN "Cannot handle \"packetlen\" cmdline arg `%s'"
# define HOPS_TOO_BIG "max hops cannot be more than 255"
# define HOPS_OUT_RANGE "first hop out of range"
# define TOO_MUCH_PROBES "no more than 10 probes per hop"


typedef struct s_parameters
{
    char				dns_name[NI_MAXHOST];

    /******************  ARGUMENTS   ******************/

    // Target ip address
    struct addrinfo		*ip_address;
	char				string_ip_address[NI_MAXHOST];
	char				*string_original_target;

    // Packet size to send
    int                 packet_len;
    char                *string_packet_len;


    /******************  BONUS FLAGS  ******************/

	// [-m] Set the max number of hops (max TTL to be reached). Default is 30
	uint16_t		    ttl_max;
	char				*string_ttl_max;

	// [-f] Start from the N hop (instead from 1)
	uint16_t			hop_start;
	char				*string_hop_start;

	// [-q] Set the number of probes per each hop. Default is 3
	uint32_t		    probes_per_hop;
	char				*string_probes_per_hop;

	// [-rdns] Resolve IP addresses to their domain names
	bool					rdns;

	// [-z] Minimal time interval between probes (default 0).
    // If the value is more than 10, then it specifies a
    // number in milliseconds, else it is a number of
    // seconds (float point values allowed too)
	float				probe_interval;
	char				*string_probe_interval;
}						t_parameters;

/****************** print ******************/
void	error_exit(int code, bool print_try, const char *msg, ...);
void	print_help_menu(void);


/****************** parsing ******************/

int	parse_args(char **args, int argc, t_parameters *params);


/****************** dns ******************/

void	dns_lookup(t_parameters *params);
void	verify_target_address(t_parameters *params);


/****************** utils ******************/

void	store_flag(t_parameters *params, char *flag_id, char *flag_value);
void	init_flag_structure(t_parameters *params);


/****************** libft_function ******************/

size_t	ft_strlen(const char *s);
void	ft_bzero(void *s, size_t n);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
int     ft_strcmp(char *s1, char *s2);

#endif