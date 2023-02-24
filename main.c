#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <unistd.h>

int i;
void  *func()
{
	printf("thread number %d\n",i);
	return(0);
}
int main()
{
	pthread_t	t[5];
	i = 0;

	while(i < 5)
	{
		if(pthread_create(t + i, NULL, &func, NULL) != 0)
			return(perror("Failed to create thread !!"),1);
		if(pthread_join(t[i], NULL) != 0)
			return(2);
		printf("thread number : %d finished \n",i);
		i++;
	}
	return(0);
}