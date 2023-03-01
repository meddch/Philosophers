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


typedef struct s_philo
{
    int id;
    int X_to_die;
    int X_to_eat;
    int X_to_sleep;
}   t_philo;




#endif