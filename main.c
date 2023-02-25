#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/time.h>

pthread_t philosopher[4];
pthread_mutex_t forks[4];

void  *func(void *val)
{
	int n = (long)val;
	printf("Philosoper  %d is thinking \n",n);
	pthread_mutex_trylock(&forks[n]);
	pthread_mutex_trylock(&forks[n + 1]);
	printf("philosopher %d is eating\n",n);
	usleep(3);
	pthread_mutex_unlock(&forks[n]);
	pthread_mutex_unlock(&forks[n + 1]);
	printf("philosopher %d is going to sleep\n",n);
	usleep(10);
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
		if(pthread_join(philosopher[i], NULL) != 0)
			return(2);
		i++;
	}
	return(0);
}