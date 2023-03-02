/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:41:34 by mechane           #+#    #+#             */
/*   Updated: 2023/02/27 14:43:39 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H 

#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/time.h>


# define UNLOCK 1
# define LOCK 0

struct s_data;;


typedef struct s_philo
{
	int				ate_times;
	int				pos;
	int				right_fork;
	int				left_fork;
	unsigned long	last_ate;
	struct s_data	*data;
	pthread_t		thread_id;
}				t_philo;


typedef struct s_data
{
	int				num_of_philos;
	int				X_to_die;
	int				X_to_eat;
	int				X_to_sleep;
	int				eat_count_max;
	int				philos_alive;
	int				max_ate;
	t_philo			*philos;
	unsigned long	start_time;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
}				t_data;


char	*ft_itoa(int n);

#endif