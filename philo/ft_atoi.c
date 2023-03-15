/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:51:44 by mechane           #+#    #+#             */
/*   Updated: 2023/03/09 19:51:44 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_isdigit(const char *s)
{
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			s++;
		else
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long long		res;
	int				sign;
	int				i;

	i = 0;
	res = 0;
	sign = 1;
	if (!*str)
		return (0);
	if (!ft_isdigit(str))
		return (-1);
	while (((((str[i] >= 9) && (str[i] <= 13))
				|| (str[i] == ' ')) && (str[i] != '\0')))
				i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return ((sign * res));
}

int	invalid_args(void)
{
	printf("\033[0;33m ----------------------------------------------------- \n");
	printf("|\033[0;33m---------------> Invalid arguments !? <--------------|\n");
	printf("|\033[0;33m------> Please enter the arguments as follow : <-----|\n");
	printf("|\033[0;33m------> First Argument : Number of philosophers <----|\n");
	printf("|\033[0;33m------> Second Argument : Time to die <--------------|\n");
	printf("|\033[0;33m------> Third Argument : Time to eat <---------------|\n");
	printf("|\033[0;33m------> Fourth Argument : Time to sleep <------------|\n");
	printf("|\033[0;33m------> Fifth Argument : Time to sleep <-------------|\n");
	printf("\033[0;33m ----------------------------------------------------- \n");
	return (1);
}
