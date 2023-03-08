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
#define TIME_ERR "\033[0;31mTime Error! \n"
#define DATA_ERR "\033[0;31mInvalid Data! \n"


typedef struct s_data
{
	int					nums_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
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
	int			pos;
	t_data		*data;	
	int			nbr_meal;
	unsigned long	last_eat;

}	t_ph;

int				ft_atoi(const char *str);
int				ft_error(char *msg);
unsigned long	ft_time(void);
void			ft_print(t_ph *ph, const char *msg);
void			ft_sleep(t_ph *ph, long long time_to_eat);
void			free_all(t_ph **ph);
int				ft_thread(t_ph **ph);

#endif