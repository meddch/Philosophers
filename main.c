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

static int	ft_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		data->philos[i].data = data;
		data->philos[i].right_fork = i;
		data->philos[i].left_fork = (i + 1) % data->num_of_philos;
		data->philos[i].pos = i + 1;
		data->philos[i].ate_times = 0;
		data->philos[i].last_ate = get_time();
		i++;
	}
	if (i != data->num_of_philos)
		return (1);
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	return (0);
}


int	ft_create(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philos);
	if (!data->forks)
		return (0);
	data->philos = malloc(sizeof(t_philo) * data->num_of_philos);
	if (!data->philos)
	{
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

void	ft_print(char *msg, t_philo *philo)
{
	char	*time;

	time = ft_itoa(get_time() - philo->data->start_time);
	pthread_mutex_lock(&philo->data->print);
	if (!philo->data->philos_alive && !philo->data->max_ate)
		printf("%s %d %s\n", time,philo->pos , msg);
	pthread_mutex_unlock(&philo->data->print);
	free(time);
}

void	ft_sleep(unsigned long duration, t_data *data)
{
	unsigned long	start;

	start = get_time();
	while (!data->philos_alive)
	{
		if (get_time() - start >= duration)
			break ;
	}
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	ft_print("has taken a fork", philo);
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	ft_print("has taken a fork", philo);
	ft_print("is eating", philo);
	philo->last_ate = get_time();
	ft_sleep(philo->data->X_to_eat, philo->data);
	philo->ate_times++;
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}





static void	*to_do(void *args)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)args;
	data = philo->data;
	if (philo->pos % 2)
		ft_sleep(data->X_to_eat, data);
	while (!data->philos_alive && !data->max_ate)
	{
		philo_eat(philo);
		ft_print("is sleeping", philo);
		ft_sleep(data->X_to_sleep, data);
		ft_print("is thinking", philo);
	}
	return (NULL);
}
int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time();
	while (i < data->num_of_philos)
	{
		data->philos[i].last_ate = get_time();
		if (pthread_create(&data->philos[i].thread_id,
				NULL, to_do, &(data->philos[i])))
			return (0);
		i++;
	}
	// i = 0;
	// while(i < data->num_of_philos)
	// {
	// 	if (pthread_join(data->philos[i].thread_id, NULL))
	// 		return(0);
	// 	i++;
	// }
	i = 0;
	while(i < data->num_of_philos)
	{
		if (pthread_detach(data->philos[i].thread_id))
			return(0);
		i++;
	}
	pthread_mutex_unlock(&data->print);
	return (1);
}

void is_philo_dead(t_data *data)
{
	int i;
	unsigned long start;

	i = 0;
	while(i < data->num_of_philos && !data->philos_alive)
	{
			unsigned long t = get_time();
			unsigned long x = data->X_to_die;
			unsigned long y = data->philos[i].last_ate;
			if (t > y && (t - y)  >= x)
			{
				// printf("        %ld\n", y);
				pthread_mutex_lock(&data->print);
				printf("%d died \n", i + 1);
				data->philos_alive = 1;
			}
	}
}

int main()
{
	t_data data;

	data.num_of_philos = 5;
	data.philos_alive = 0;
	data.X_to_eat = 100;
	data.X_to_sleep = 100;
	data.X_to_die = 400;

	if (!ft_create(&data))
		return(0);
	if (ft_init(&data))
	{
		free(data.philos);
		free(data.forks);
		return (0);
	}
	if (!create_threads(&data))
		return(0);
	while(1)
	{
		is_philo_dead(&data);
		if(data.philos_alive)
			break;
	}
	return(0);
}