x	

#include "philo.h"

long long int	current_time(void)
{
	struct timeval	time;
	long long int	ms;

	gettimeofday(&time, NULL);
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (ms);
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
			pthread_mutex_lock(&data->mtx_eat);
			if (current_time() - ph[i]->last_eat_t > data->time_die)
			{
				print_status(ph[i], "died\n");
				data->is_dead = 1;
			}
			pthread_mutex_unlock(&data->mtx_eat);
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
	pthread_mutex_lock(&data->mtx_eat);
	print_status(ph, "is eating\n");
	ph->last_eat_t = current_time();
	pthread_mutex_unlock(&data->mtx_eat);
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
		error("Malloc error\n");
	}	
	i = -1;
	while (++i < size)
		pthread_mutex_init(&forks[i], NULL);
	pthread_mutex_init(&data->mtx_write, NULL);
	pthread_mutex_init(&data->mtx_eat, NULL);
	return (forks);
}

t_data	*data_init(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		error("Malloc error\n");
	data->fork_size = ft_atoll(av[1]);
	data->time_die = ft_atoll(av[2]);
	data->time_eat = ft_atoll(av[3]);
	data->time_sleep = ft_atoll(av[4]);
	if (ac == 5)
		data->max_eat = -1;
	else
		data->max_eat = ft_atoll(av[5]);
	if (data->time_die < 1 || data->time_eat < 1 || data->time_sleep < 1
		|| data->fork_size < 1 || (ac == 6 && data->max_eat < 1))
	{
		free(data);
		error("Error: arguments\n");
	}
	data->forks = fork_init(ft_atoll(av[1]), data);
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
		error("Malloc error\n");
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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long long	ft_atoll(char *str)
{
	int			i;
	long long	sign;
	long long	number;

	sign = 1;
	number = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		number = (number * 10) + (str[i] - 48) * sign;
		i++;
	}
	return (number);
}

void	print_status(t_ph *ph, const char *msg)
{
	t_data	*data;

	data = ph->data;
	pthread_mutex_lock(&data->mtx_write);
	if (!(data->is_dead))
	{
		printf("%lld ", current_time() - data->ms_start);
		printf("%d %s", ph->i + 1, msg);
	}	
	pthread_mutex_unlock(&data->mtx_write);
}
