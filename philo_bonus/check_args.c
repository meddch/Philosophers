/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 07:57:33 by mechane           #+#    #+#             */
/*   Updated: 2023/03/14 08:52:14 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	check_data(int ac, t_data *data)
{
	if ((data->nums_of_philo < -1 || data->nums_of_philo > 200)
		|| (data->time_to_die < 0 || data->time_to_eat < 0)
		|| (data->time_to_sleep < 0 || (data->max_meal < 1 && ac == 6)))
		return (ft_error(DATA_ERR));
	return (0);
}