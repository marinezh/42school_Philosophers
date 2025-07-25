#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;

void *add_odd_numbers(void *arg)
{
	int i = 0;
	while (i < 100)
	{
		pthread_mutex_lock(&lock);   // Lock the counter
		if (i%2 != 0)
		{
			printf("ODD value: %d\n", counter);
		}
		counter++;
		pthread_mutex_unlock(&lock); // Unlock the counter
		i++;
	}
	return NULL;
}

void *add_even_numbers(void *arg)
{
	int i = 0;
	while (i < 100)
	{
		pthread_mutex_lock(&lock);   // Lock the counter
		if (i%2 == 0)
		{
			printf("EVEN value: %d\n", counter);
		}
		counter++;

		pthread_mutex_unlock(&lock); // Unlock the counter
		i++;
	}
	return NULL;
}
int main()
{
	pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);  // Initialize tclearhe mutex

    // Create two threads
    pthread_create(&t1, NULL, add_odd_numbers, NULL);
    pthread_create(&t2, NULL, add_even_numbers, NULL);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Final counter value should be 20000
    printf("Final counter value: %d\n", counter);

    pthread_mutex_destroy(&lock);     // Clean up

    return 0;
}