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

unsigned long int	current_time(void)
{
	struct timeval	time;
	long long int	ms;

	gettimeofday(&time, NULL);
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (ms);
}

void	ft_error(char *msg)
{
	printf("%s", msg);
	return ;
}

void	free_all(t_ph **ph)
{
	int		i;
	t_data	*data;

	i = -1;
	data = ph[0]->data;
	while (++i < data->fork_size)
		free(ph[i]);
	free(data->forks);
	free(data);
	free(ph);
}

void	print_status(t_ph *ph, const char *msg)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->mtx_print);
	if (!(data->is_dead))
	{
		printf("%lld ", current_time() - data->ms_start);
		printf("%d %s", ph->i + 1, msg);
	}	
	pthread_mutex_unlock(&data->mtx_print);
}


void	ft_usleep(t_ph *ph, long long time_eat)
{
	long long	start;

	start = current_time();
	while (!(ph->data->is_dead))
	{
		if (current_time() - start >= time_eat)
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
	while (data->max_eat != -1 && i < data->fork_size
		&& ph[i]->nbr_eat >= data->max_eat)
		i++;
	if (i == data->fork_size)
		data->is_all_eaten = 1;
}

void	check_death(t_ph **ph)
{
	int		i;
	t_data	*data;

	data = ph[0]->data;
	while (!(data->is_all_eaten))
	{
		i = -1;
		while (++i < data->fork_size)
		{
			pthread_mutex_lock(&data->mtx_meal);
			if (current_time() - ph[i]->last_eat_t > data->time_die)
			{
				print_status(ph[i], "died\n");
				data->is_dead = 1;
			}
			pthread_mutex_unlock(&data->mtx_meal);
			usleep(100);
		}
		if (data->is_dead)
			break ;
		check_meal(ph);
	}
}

void	eating(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->forks[ph->i]);
	print_status(ph, "has taken a fork\n");
	if (data->fork_size == 1)
	{
		ft_usleep(ph, data->time_die + 50);
		return ;
	}	
	pthread_mutex_lock(&data->forks[(ph->i + 1) % data->fork_size]);
	print_status(ph, "has taken a fork\n");
	pthread_mutex_lock(&data->mtx_meal);
	print_status(ph, "is eating\n");
	ph->last_eat_t = current_time();
	pthread_mutex_unlock(&data->mtx_meal);
	ft_usleep(ph, data->time_eat);
	ph->nbr_eat += 1;
	pthread_mutex_unlock(&data->forks[ph->i]);
	pthread_mutex_unlock(&data->forks[(ph->i + 1) % data->fork_size]);
}

void	*dining(void *param)
{
	t_ph	*ph;
	t_data	*data;

	ph = (t_ph *)param;
	data = ph->data;
	if (ph->i % 2)
		usleep(15000);
	while (!(data->is_dead))
	{
		eating(ph);
		if (data->is_all_eaten)
			break ;
		print_status(ph, "is sleeping\n");
		ft_usleep(ph, data->time_sleep);
		print_status(ph, "is thinking\n");
	}
	return (NULL);
}

int	threading(t_ph **ph)
{
	long long int	i;

	i = 0;
	while (i < ph[0]->data->fork_size)
	{
		if (pthread_create(&ph[i]->th, NULL, &dining, ph[i]) != 0)
			error("Error creating threads!\n");
		ph[i]->last_eat_t = current_time();
		i++;
	}
	check_death(ph);
	i = 0;
	while (i < ph[0]->data->fork_size)
	{
		if (pthread_join(ph[i]->th, NULL) != 0)
			error("Error joining threads!\n");
		i++;
	}
	free_all(ph);
	return (0);
}


pthread_mutex_t	*fork_init(long long size, t_data *data)
{
	long long		i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * size);
	if (!forks)
	{
		free(data);
		ft_error("Allocation failed\n");
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
		ft_error("Allocation failed\n");
	data->fork_size = 10;
	data->time_die = 300;
	data->time_eat = 100;
	data->time_sleep = 100;
	data->max_eat = -1;
	data->forks = fork_init(data->fork_size, data);
	data->ms_start = current_time();
	data->is_dead = 0;
	data->is_all_eaten = 0;
	return (data);
}

t_ph	**philo_init(t_data *data)
{
	t_ph	**ph;
	int		i;

	ph = (t_ph **)malloc(sizeof(t_ph *) * data->fork_size);
	if (!ph)
	{
		free(data->forks);
		free(data);
		ft_error("Allocation failed\n");
	}
	i = 0;
	while (i < data->fork_size)
	{
		ph[i] = (t_ph *)malloc(sizeof(t_ph));
		ph[i]->i = i;
		ph[i]->data = data;
		ph[i]->nbr_eat = 0;
		i++;
	}
	return (ph);
}



int	main(int ac, char **av)
{
	t_data	*data;
	t_ph	**ph;

	data = data_init(ac, av);
	ph = philo_init(data);
	threading(ph);
}