#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
// gcc main..c -lpthread

void *func(void *data)
{
	int n = atoi((char *)data);
	for (int i = 0; i < n; i++)
	{
		printf("thread id=%lu value i=%d\n", pthread_self(), i);
	}
	pthread_exit(0);
}
int main(int argc, char **args)
{
	int k = argc -1 ; // quantity of threads
	//pthread_t tid[k]; 
	pthread_t *tid = malloc(sizeof(pthread_t*)*k);
	for(int i = 0; i < k; i++)
	{
		pthread_create(&tid[i], NULL, func, args[i + 1]);
	}
		for(int i = 0; i < k; i++)
	{
		pthread_join(tid[i], NULL);
	}
	free(tid);
	return 0;
}