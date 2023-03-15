/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 09:37:11 by mechane           #+#    #+#             */
/*   Updated: 2023/03/15 12:35:45 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	check_meal(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	if (ph->nbr_meal > data->max_meal && data->max_meal != -1)
		return (1);
	return (0);
}

static void	check_death(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	while (!data->stop_flag)
	{
		sem_wait(data->sem_meal);
		if (ft_time() - ph->last_eat > (unsigned long)data->time_to_die)
		{
			ft_print(ph, DIED);
			sem_wait(data->sem_print);
			data->stop_flag = 1;
			return ;
		}
		sem_post(data->sem_meal);
		if (check_meal(ph))
			break ;
	}
	return ;
}

static void	ft_eat(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	sem_wait(data->forks);
	ft_print(ph, FORK);
	if (data->nums_of_philo == 1)
	{
		ft_sleep(data->time_to_die);
		return ;
	}	
	sem_wait(data->forks);
	ft_print(ph, FORK);
	sem_wait(data->sem_meal);
	ft_print(ph, EAT);
	ph->last_eat = ft_time();
	sem_post(data->sem_meal);
	ft_sleep(data->time_to_eat);
	ph->nbr_meal ++;
	if (data->stop_flag)
		return ;
	sem_post(data->forks);
	sem_post(data->forks);
}

void	*ft_routine(void *args)
{
	t_ph	*ph;
	t_data	*data;
	int		i;

	ph = (t_ph *)args;
	data = ph->data;
	i = 0;
	if (ph->pos % 2)
	{
		while (++i <= 500)
			usleep(1);
	}
	while (!data->stop_flag)
	{
		ft_eat(ph);
		if (data->nums_of_philo == 1)
			break ;
		ft_print(ph, SLEEP);
		ft_sleep(data->time_to_sleep);
		ft_print(ph, THINK);
	}
	return (NULL);
}

int	ft_thread(t_ph *ph)
{
	ph->last_eat = ft_time();
	if (pthread_create(&ph->th, NULL, &ft_routine, ph) != 0)
		ft_error(CREAT_THRD_ERR);
	if (pthread_detach(ph->th) != 0)
		ft_error(JOIN_THRD_ERR);
	check_death(ph);
	return (0);
}
