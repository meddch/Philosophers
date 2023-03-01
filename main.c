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

pthread_t philosopher[4];
pthread_mutex_t forks[4];

void  *func(void *val)
{
	int n = (long)val;
		while(1)
		{
		if (n % 2)
			usleep(2);
		printf("Philosoper  %d is thinking \n",n);
		usleep(100);
		pthread_mutex_lock(&forks[n]);
		printf("Philosoper  %d has taken a fork \n",n);
		pthread_mutex_trylock(&forks[n + 1]);
		printf("Philosoper  %d has taken a fork \n",n);
		printf("Philosopher %d is eating\n",n);
		usleep(1000);
		pthread_mutex_unlock(&forks[n]);
		pthread_mutex_unlock(&forks[n + 1]);
		printf("Philosopher %d is going to sleep\n",n);
		usleep(10000);
		}
	return(NULL);
}
int main()
{
	int i = 0;

	
	while (i < 4)
	{
		pthread_mutex_init(&forks[i],NULL);
		i++;
	}
	i = 0;
	while(i < 4)
	{
		if(pthread_create(&philosopher[i], NULL, &func, (void*)(long)i) != 0)
			return(perror("Failed to create thread !!"),1);
		i++;
	}
	i = 0;
	while(i < 4)
	{
		if(pthread_detach(philosopher[i]) != 0)
			return(2);
		i++;
	}
	while(1);
	return(0);
}