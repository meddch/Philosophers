/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:11:53 by mechane           #+#    #+#             */
/*   Updated: 2023/03/15 12:35:29 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	ft_kill(t_ph **ph)
{
	int		i;
	int		size;
	t_data	*data;

	data = ph[0]->data;
	i = -1;
	size = data->nums_of_philo;
	while (++i < size)
		kill(ph[i]->pid, SIGKILL);
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("meal");
	return ;
}

static void	ft_processes(t_ph **ph)
{
	int	i;
	int	size;

	size = ph[0]->data->nums_of_philo;
	i = -1;
	while (++i < size)
	{
		ph[i]->pid = fork();
		if (!ph[i]->pid)
		{
			ft_thread(ph[i]);
			exit (0);
		}
	}
}

static t_ph	**philo_init(t_data *data)
{
	int		i;
	t_ph	**ph;

	ph = malloc(sizeof(t_ph *) * data->nums_of_philo);
	if (!ph)
	{
		free(data);
		return (ft_error(MALLOC_ERR), NULL);
	}
	i = 0;
	while (i < data->nums_of_philo)
	{
		ph[i] = malloc(sizeof(t_ph));
		ph[i]->data = data;
		ph[i]->pos = i;
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
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("meal");
	data->forks = sem_open("forks", O_CREAT, 0600, data->nums_of_philo);
	data->sem_print = sem_open("print", O_CREAT, 0600, 1);
	data->sem_meal = sem_open("meal", O_CREAT, 0600, 1);
	if (check_data(ac, data))
		return (free(data), NULL);
	return (data);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_ph	**ph;

	if (ac < 5 || ac > 6)
		return (invalid_args());
	data = ft_init(ac, av);
	ph = philo_init(data);
	ft_processes(ph);
	waitpid(-1, NULL, 0);
	ft_kill(ph);
	free_all(ph);
}
