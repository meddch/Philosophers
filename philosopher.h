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


typedef struct s_data
{
	int				time_die;
	long long int	ms_start;
	int				time_eat;
	int				time_sleep;
	int				max_eat;
	int				fork_size;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mtx_print;
	pthread_mutex_t	mtx_meal;
	int				is_dead;
	int				is_all_eaten;
}	t_data;

typedef struct s_ph
{
	pthread_t	th;
	int			i;
	t_data		*data;	
	int			nbr_eat;	
	long long	last_eat_t;

}	t_ph;



char	*ft_itoa(int n);
unsigned long	get_time(void);

#endif