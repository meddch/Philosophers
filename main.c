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

static int	ft_init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	return (0);
}


static int	ft_init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		data->philos[i].ate_times = 0;
		data->philos[i].pos = i + 1;
		data->philos[i].right_fork = i;
		data->philos[i].left_fork = (i + 1) % data->count;
		data->philos[i].data = data;
		i++;
	}
	if (i != data->count)
		return (1);
	return (0);
}

int	ft_init(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->count);
	if (!data->philos)
		return (0);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->count);
	if (!data->forks)
	{
		free(data->philos);
		return (0);
	}
	if (ft_init_mutex(data))
	{
		free(data->philos);
		free(data->forks);
		return (0);
	}
	if (ft_init_philo(data))
	{
		free(data->philos);
		free(data->forks);
		return (0);
	}
	return (1);
}

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * (unsigned long)1000) + (time.tv_usec / 1000));
}

void	philo_print(char *msg, t_philo *philo, int unlock)
{
	char	*timestamp;

	// timestamp = ft_itoa(get_time() - philo->data->start_time);
	pthread_mutex_lock(&philo->data->print);
	if (!philo->data->stop_condition && !philo->data->max_ate)
		printf("%s  %s\n", timestamp, msg);
	if (unlock)
		pthread_mutex_unlock(&philo->data->print);
	free(timestamp);
}

void	new_sleep(unsigned long duration, t_data *data)
{
	unsigned long	start;

	start = get_time();
	while (!data->stop_condition)
	{
		if (get_time() - start >= duration)
			break ;
		usleep(data->count * 2);
	}
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	philo_print("has taken a fork", philo, UNLOCK);
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	philo_print("has taken a fork", philo, UNLOCK);
	philo_print("is eating", philo, UNLOCK);
	philo->last_ate = get_time();
	new_sleep(philo->data->X_to_eat, philo->data);
	philo->ate_times++;
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}





static void	*routine(void *params)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)params;
	data = philo->data;
	if (philo->pos % 2 && data->count > 1)
		new_sleep(data->X_to_eat / 50, data);
	while (!data->stop_condition && !data->max_ate)
	{
		philo_eat(philo);
		philo_print("is sleeping", philo, UNLOCK);
		new_sleep(data->X_to_sleep, data);
		philo_print("is thinking", philo, UNLOCK);
	}
	return (NULL);
}
int	create_threads(t_data *data)
{
	int	i;

	i = -1;
	data->start_time = get_time();
	while (++i < data->count)
	{
		data->philos[i].last_ate = get_time();
		if (pthread_create(&data->philos[i].thread_id,
				NULL, routine, &(data->philos[i])))
			return (0);
	}
	pthread_mutex_unlock(&data->print);
	return (1);
}



int main()
{
	t_data data;

	if (!ft_init(&data))
		return(0);
	if (!create_threads(&data))
		return(0);
	
	return(0);
}