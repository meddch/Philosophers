/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:07:50 by mechane           #+#    #+#             */
/*   Updated: 2023/03/09 20:07:50 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_error(char *msg)
{
	return (printf("%s", msg));
}

unsigned long	ft_time(void)
{
	struct timeval	time;
	unsigned long	time_ms;

	if (gettimeofday(&time, NULL))
		return (ft_error(TIME_ERR));
	time_ms = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return (time_ms);
}

void	free_all(t_ph **ph)
{
	t_data	*data;
	int		i;

	i = -1;
	data = ph[0]->data;
	pthread_mutex_destroy(&data->mtx_meal);
	pthread_mutex_destroy(&data->mtx_print);
	while (++i < data->nums_of_philo)
		pthread_mutex_destroy(&data->forks[i]);
	i = -1;
	while (++i < data->nums_of_philo)
		free(ph[i]);
	free(data->forks);
	free(data);
	free(ph);
}

void	ft_print(t_ph *ph, const char *msg)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->mtx_print);
	printf("%lu ", ft_time() - data->ms_start);
	printf("%d %s", ph->pos + 1, msg);
	pthread_mutex_unlock(&data->mtx_print);
}

void	ft_sleep(unsigned long time_to_eat)
{
	unsigned long	start;

	start = ft_time();
	while (1)
	{
		if (ft_time() - start >= time_to_eat)
			break ;
		usleep(50);
	}
}
