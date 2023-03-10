/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:41:34 by mechane           #+#    #+#             */
/*   Updated: 2023/03/10 14:44:51 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H 

# include<stdio.h>
# include<pthread.h>
# include<stdlib.h>
# include<unistd.h>
# include<sys/time.h>
# define FORK "\033[0;34mhas taken a fork\n\033[0;37m"
# define THINK "is thinking\n"
# define EAT "\033[0;32mis eating\n\033[0;37m"
# define SLEEP "\033[0;33mis sleeping\n\033[0;37m"
# define DIED "\033[0;31mdied\n\033[0;37m"
# define MALLOC_ERR "\033[0;31mAllocation Failed \n\033[0;37m"
# define ARGS_ERR "\033[0;31mInvalid Arguments \n\033[0;37m"
# define CREAT_THRD_ERR "\033[0;31mError Creating threads! \n\033[0;37m"
# define JOIN_THRD_ERR "\033[0;31mError Joining Threads! \n\033[0;37m"
# define TIME_ERR "\033[0;31mTime Error! \n\033[0;37m"
# define DATA_ERR "\033[0;31mInvalid Data! \n\033[0;37m"

typedef struct s_data
{
	int					nums_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_meal;
	int					stop_flag;
	unsigned long		ms_start;
	pthread_mutex_t		*forks;
	pthread_mutex_t		mtx_print;
	pthread_mutex_t		mtx_meal;
}	t_data;

typedef struct s_ph
{
	pthread_t		th;
	int				pos;
	t_data			*data;	
	int				nbr_meal;
	unsigned long	last_eat;

}	t_ph;

int				ft_atoi(const char *str);
int				ft_error(char *msg);
unsigned long	ft_time(void);
void			ft_print(t_ph *ph, const char *msg);
void			ft_sleep(unsigned long time_to_eat);
void			free_all(t_ph **ph);
int				ft_thread(t_ph **ph);

#endif