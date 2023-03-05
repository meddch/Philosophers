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
#define MALLOC_ERR "\033[0;31mAllocation Failed \n"
#define ARGS_ERR "\033[0;31mInvalid Arguments \n"
#define CREAT_THRD_ERR "\033[0;31mError Creating threads! \n"
#define JOIN_THRD_ERR "\033[0;31mError Joining Threads! \n"


typedef struct s_data
{
	int					philo_count;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_eat;
	int					is_dead;
	int					is_all_eaten;
	unsigned long int	ms_start;
	pthread_mutex_t		*forks;
	pthread_mutex_t		mtx_print;
	pthread_mutex_t		mtx_meal;
}	t_data;

typedef struct s_ph
{
	pthread_t	th;
	int			i;
	t_data		*data;	
	int			nbr_eat;	
	unsigned long	last_eat_t;

}	t_ph;


#endif