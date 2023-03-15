/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:59:30 by mechane           #+#    #+#             */
/*   Updated: 2023/03/09 19:59:30 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	check_meal(t_ph **ph)
{
	int		i;
	t_data	*data;

	i = 0;
	data = ph[0]->data;
	i = 0;
	while (i < data->nums_of_philo && ph[i]->nbr_meal >= data->max_meal
		&& data->max_meal != -1)
		i++;
	if (i == data->nums_of_philo)
		return (1);
	return (0);
}

static void	check_death(t_ph **ph)
{
	t_data	*data;
	int		i;

	data = ph[0]->data;
	while (!data->stop_flag)
	{
		i = 0;
		while (i < data->nums_of_philo)
		{
			pthread_mutex_lock(&data->mtx_meal);
			if (ft_time() - ph[i]->last_eat > (unsigned long)data->time_to_die)
			{
				ft_print(ph[i], DIED);
				pthread_mutex_lock(&data->mtx_print);
				data->stop_flag = 1;
				return ;
			}
			pthread_mutex_unlock(&data->mtx_meal);
			usleep(500);
			i++;
		}
		if (check_meal(ph))
			break ;
	}
	return ;
}

static void	ft_eat(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->forks[ph->pos]);
	ft_print(ph, FORK);
	if (data->nums_of_philo == 1)
	{
		ft_sleep(data->time_to_die);
		return ;
	}	
	pthread_mutex_lock(&data->forks[(ph->pos + 1) % data->nums_of_philo]);
	ft_print(ph, FORK);
	pthread_mutex_lock(&data->mtx_meal);
	ft_print(ph, EAT);
	ph->last_eat = ft_time();
	pthread_mutex_unlock(&data->mtx_meal);
	ft_sleep(data->time_to_eat);
	ph->nbr_meal ++;
	if (data->stop_flag)
		return ;
	pthread_mutex_unlock(&data->forks[ph->pos]);
	pthread_mutex_unlock(&data->forks[(ph->pos + 1) % data->nums_of_philo]);
}

static void	*ft_routine(void *args)
{
	t_ph	*ph;
	t_data	*data;
	int		i;

	i = 0;
	ph = (t_ph *)args;
	data = ph->data;
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

int	ft_thread(t_ph **ph)
{
	int	i;

	i = -1;
	while (++i < ph[0]->data->nums_of_philo)
		ph[i]->last_eat = ft_time();
	i = 0;
	while (i < ph[0]->data->nums_of_philo)
	{
		ph[i]->last_eat = ft_time();
		if (pthread_create(&ph[i]->th, NULL, &ft_routine, ph[i]) != 0)
			ft_error(CREAT_THRD_ERR);
		i++;
	}
	i = 0;
	while (i < ph[0]->data->nums_of_philo)
	{
		if (pthread_detach(ph[i]->th) != 0)
			ft_error(JOIN_THRD_ERR);
		i++;
	}
	check_death(ph);
	free_all(ph);
	return (0);
}
