/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:41:38 by mechane           #+#    #+#             */
/*   Updated: 2023/02/27 14:45:14 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosopher.h"

unsigned long int	ft_time(void)
{
	struct timeval	time;
	long long int	time_ms;

	gettimeofday(&time, NULL);
	time_ms = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return (time_ms);
}

void	ft_error(char *msg)
{
	printf("%s", msg);
	return ;
}

void	free_all(t_ph **ph)
{
	t_data	*data;
	int		i;

	i = 0;
	data = ph[0]->data;
	while (i < data->philo_count)
		free(ph[i++]);
	free(data->forks);
	free(data);
	free(ph);
}

void	print(t_ph *ph, const char *msg)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->mtx_print);
	if (!(data->is_dead))
	{
		printf("%lu ", ft_time() - data->ms_start);
		printf("%d %s", ph->i + 1, msg);
	}	
	pthread_mutex_unlock(&data->mtx_print);
}


void	ft_sleep(t_ph *ph, long long time_to_eat)
{
	long long	start;

	start = ft_time();
	while (!(ph->data->is_dead))
	{
		if (ft_time() - start >= time_to_eat)
			break ;
		usleep(50);
	}
}

void	check_meal(t_ph **ph)
{
	int		i;
	t_data	*data;

	i = 0;
	data = ph[0]->data;
	i = 0;
	while (data->max_eat != -1 && i < data->philo_count
		&& ph[i]->nbr_eat >= data->max_eat)
		i++;
	if (i == data->philo_count)
		data->is_all_eaten = 1;
}

void	check_death(t_ph **ph)
{
	t_data	*data;
	int		i;

	data = ph[0]->data;
	while (!(data->is_all_eaten))
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->mtx_meal);
			if (ft_time() - ph[i]->last_eat_t > data->time_to_die)
			{
				print(ph[i], "died\n");
				data->is_dead = 1;
			}
			pthread_mutex_unlock(&data->mtx_meal);
			usleep(100);
			i++;
		}
		if (data->is_dead)
			break ;
		check_meal(ph);
	}
}

void	ft_eat(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->forks[ph->i]);
	print(ph, "has taken a fork\n");
	if (data->philo_count == 1)
	{
		ft_sleep(ph, data->time_to_die + 50);
		return ;
	}	
	pthread_mutex_lock(&data->forks[(ph->i + 1) % data->philo_count]);
	print(ph, "has taken a fork\n");
	pthread_mutex_lock(&data->mtx_meal);
	print(ph, "is ft_eat\n");
	ph->last_eat_t = ft_time();
	pthread_mutex_unlock(&data->mtx_meal);
	ft_sleep(ph, data->time_to_eat);
	ph->nbr_eat += 1;
	pthread_mutex_unlock(&data->forks[ph->i]);
	pthread_mutex_unlock(&data->forks[(ph->i + 1) % data->philo_count]);
}

void	*ft_routine(void *param)
{
	t_ph	*ph;
	t_data	*data;

	ph = (t_ph *)param;
	data = ph->data;
	if (ph->i % 2)
		usleep(15000);
	while (!(data->is_dead))
	{
		ft_eat(ph);
		if (data->is_all_eaten)
			break ;
		print(ph, "is sleeping\n");
		ft_sleep(ph, data->time_to_sleep);
		print(ph, "is thinking\n");
	}
	return (NULL);
}

int	ft_thread(t_ph **ph)
{
	long long int	i;

	i = 0;
	while (i < ph[0]->data->philo_count)
	{
		if (pthread_create(&ph[i]->th, NULL, &ft_routine, ph[i]) != 0)
			ft_error(CREAT_THRD_ERR);
		ph[i]->last_eat_t = ft_time();
		i++;
	}
	check_death(ph);
	i = 0;
	while (i < ph[0]->data->philo_count)
	{
		if (pthread_join(ph[i]->th, NULL) != 0)
			ft_error(JOIN_THRD_ERR);
		i++;
	}
	free_all(ph);
	return (0);
}


pthread_mutex_t	*fork_init(int size, t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * size);
	if (!forks)
	{
		free(data);
		ft_error(MALLOC_ERR);
	}	
	i = -1;
	while (++i < size)
		pthread_mutex_init(&forks[i], NULL);
	pthread_mutex_init(&data->mtx_print, NULL);
	pthread_mutex_init(&data->mtx_meal, NULL);
	return (forks);
}

t_data	*data_init()
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		ft_error(MALLOC_ERR);
	data->philo_count = 10;
	data->time_to_die = 300;
	data->time_to_eat = 100;
	data->time_to_sleep = 100;
	data->max_eat = -1;
	data->forks = fork_init(data->philo_count, data);
	data->ms_start = ft_time();
	data->is_dead = 0;
	data->is_all_eaten = 0;
	return (data);
}

t_ph	**philo_init(t_data *data)
{
	t_ph	**ph;
	int		i;

	ph = malloc(sizeof(t_ph *) * data->philo_count);
	if (!ph)
	{
		free(data->forks);
		free(data);
		ft_error(MALLOC_ERR);
	}
	i = 0;
	while (i < data->philo_count)
	{
		ph[i] = malloc(sizeof(t_ph));
		ph[i]->i = i;
		ph[i]->data = data;
		ph[i]->nbr_eat = 0;
		i++;
	}
	return (ph);
}


int	main(int ac, char **av)
{
	t_ph	**ph;
	t_data	*data;

	data = data_init(ac, av);
	ph = philo_init(data);
	ft_thread(ph);
}