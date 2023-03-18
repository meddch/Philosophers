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

static int	check_data(int ac, t_data *data)
{
	if ((data->nums_of_philo < 1 || data->nums_of_philo > 200)
		|| (data->time_to_die < 0 || data->time_to_eat < 0)
		|| (data->time_to_sleep < 0 || (data->max_meal < 1 && ac == 6)))
		return (ft_error(DATA_ERR));
	return (0);
}

static pthread_mutex_t	*ft_mutex_init(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;
	int				nums;

	i = 0;
	nums = data->nums_of_philo;
	forks = malloc(sizeof(pthread_mutex_t) * nums);
	pthread_mutex_init(&data->mtx_meal, NULL);
	pthread_mutex_init(&data->mtx_print, NULL);
	while (i < nums)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

static t_ph	**philo_init(t_data *data)
{
	int		i;
	t_ph	**ph;

	ph = malloc(sizeof(t_ph *) * data->nums_of_philo);
	if (!ph)
	{
		free(data->forks);
		free(data);
		return (ft_error(MALLOC_ERR), NULL);
	}
	i = 0;
	while (i < data->nums_of_philo)
	{
		ph[i] = malloc(sizeof(t_ph));
		ph[i]->pos = i;
		ph[i]->data = data;
		ph[i]->nbr_meal = 0;
		i++;
	}
	return (ph);
}

static t_data	*ft_init(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		ft_error(MALLOC_ERR);
	data->nums_of_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->max_meal = ft_atoi(av[5]);
	else
		data->max_meal = -1;
	data->stop_flag = 0;
	data->ms_start = ft_time();
	data->forks = ft_mutex_init(data);
	if (check_data(ac, data))
		return (free(data->forks), free(data), NULL);
	return (data);
}

int	main(int ac, char **av)
{
	t_ph	**ph;
	t_data	*data;

	if (ac < 5 || ac > 6)
		return (invalid_args());
	data = ft_init(ac, av);
	if (!data)
		return (0);
	ph = philo_init(data);
	if (!*ph)
		return (0);
	ft_thread(ph);
}
