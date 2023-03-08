#include "philosopher.h"

static void	check_meal(t_ph **ph)
{
	int		i;
	t_data	*data;

	i = 0;
	data = ph[0]->data;
	i = 0;
	while (data->max_meal != -1 && i < data->nums_of_philo
		&& ph[i]->nbr_meal >= data->max_meal)
		i++;
	if (i == data->nums_of_philo)
		data->is_all_eaten = 1;
}

static void	check_death(t_ph **ph)
{
	t_data	*data;
	int		i;

	data = ph[0]->data;
	while (!(data->is_all_eaten))
	{
		i = 0;
		while (i < data->nums_of_philo)
		{
			pthread_mutex_lock(&data->mtx_meal);
			if (ft_time() - ph[i]->last_eat > data->time_to_die)
			{
				ft_print(ph[i], "died\n");
				data->is_dead = 1;
				// pthread_mutex_lock(&data->mtx_print);
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

static void	ft_eat(t_ph *ph)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->forks[ph->pos]);
	ft_print(ph, "has taken a fork\n");
	if (data->nums_of_philo == 1)
	{
		ft_sleep(ph, data->time_to_die + 50);
		return ;
	}	
	pthread_mutex_lock(&data->forks[(ph->pos + 1) % data->nums_of_philo]);
	ft_print(ph, "has taken a fork\n");
	pthread_mutex_lock(&data->mtx_meal);
	ft_print(ph, "is eating\n");
	ph->last_eat = ft_time();
	pthread_mutex_unlock(&data->mtx_meal);
	ft_sleep(ph, data->time_to_eat);
	ph->nbr_meal += 1;
	pthread_mutex_unlock(&data->forks[ph->pos]);
	pthread_mutex_unlock(&data->forks[(ph->pos + 1) % data->nums_of_philo]);
}

static void	*ft_routine(void *args)
{
	t_ph	*ph;
	t_data	*data;

	ph = (t_ph *)args;
	data = ph->data;
	if (ph->pos % 2)
		usleep(15000);
	while (!(data->is_dead))
	{
		ft_eat(ph);
		if (data->is_all_eaten)
			break ;
		ft_print(ph, "is sleeping\n");
		ft_sleep(ph, data->time_to_sleep);
		ft_print(ph, "is thinking\n");
	}
	return (NULL);
}

int	ft_thread(t_ph **ph)
{
	int	i;

	i = 0;
	while (i < ph[0]->data->nums_of_philo)
	{
		if (pthread_create(&ph[i]->th, NULL, &ft_routine, ph[i]) != 0)
			ft_error(CREAT_THRD_ERR);
		ph[i]->last_eat = ft_time();
		i++;
	}
	check_death(ph);
	i = 0;
	while (i < ph[0]->data->nums_of_philo)
	{
		if (pthread_join(ph[i]->th, NULL) != 0)
			ft_error(JOIN_THRD_ERR);
		i++;
	}
	free_all(ph);
	return (0);
}
