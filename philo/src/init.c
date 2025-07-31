/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:42:32 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/31 19:00:13 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_forks(t_data *data)
{
	int i;
	int j;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
	{
		printf("memory allocation failed\n");
		return 0;
	}
	i = 0;
	while (i < data->num_philos)
	{
		data->forks[i].id = i;
		if(pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
		{
			printf("Failed to initialize fork mutex %d\n", i);
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(&data->forks[j].lock);
				j++;
			}
			free(data->forks);
			return (0);
		}
		i++;
	}
	return 1;
}

int init_philos(t_data *data)
{
	int i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
	{
		printf("memory allocation failed\n");
		return 0;
	}
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_time_ms();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		// printf("Philosopher %d initialized | Left fork: %d | Right fork: %d\n",
		// 	data->philos[i].id, i, (i + 1) % data->num_philos);
		i++;
	}
	return (1);
}

void cleanup_resources(t_data *data, int level)
{
	int i;

	if (level >= 3 && data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i].lock);
			i++;
		}
		free(data->forks);
	}
	if (level >= 2)
		pthread_mutex_destroy(&data->death_lock);
	if (level >= 1)
		pthread_mutex_destroy(&data->print_lock);
	if (level >= 4 && data->philos)
		free(data->philos);
}
int	init_data(t_data *data)
{
	data->start_time = get_time_ms();
	data->philos = NULL;
	data->forks = NULL;
	data->is_dead = 0;
	data->num_full = 0;

	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		printf("Failed to initialize print mutex\n");
		return (0);
	}
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
	{
		printf("Failed to initialize death mutex\n");
		cleanup_resources(data, 1); // Clean up previously allocated mutex
		return (0);
	}
	
	if (!init_forks(data))
	{
		cleanup_resources(data, 2);
		return (0);
	}

	if (!init_philos(data))
	{
		cleanup_resources(data, 3);
		return (0);
	}
	return (1);
}
