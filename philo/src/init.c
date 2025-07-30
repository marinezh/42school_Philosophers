/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:42:32 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/30 22:50:12 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_forks(t_data *data)
{
	int i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
	{
		printf("memory allocation failed\n");
		return 0;
	}
	//printf("CHECK FORKS %d\n", data->num_philos);
	i = 0;
	while (i < data->num_philos)
	{
		data->forks[i].id = i;
		if(pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
		{
			printf("Failed to initialize fork mutex %d\n", i);
			return (0);
		}
		//printf("Fork %d initialized\n", i);
		i++;
	}
	return 1;
}

int init_philos(t_data *data)
{
	int i;

	//printf("Initializing philosophers...\n");
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


int	init_data(t_data *data)
{
	data->simulation_running = 1;
	data->start_time = get_time_ms();

	// printf("Simulation settings:\n");
	// printf("- Philosophers: %d\n", data->num_philos);
	// printf("- Time to die: %d ms\n", data->time_to_die);
	// printf("- Time to eat: %d ms\n", data->time_to_eat);
	// printf("- Time to sleep: %d ms\n", data->time_to_sleep);
	// printf("- Must eat count: %d\n", data->must_eat);
	// printf("Number of forks: %d\n", data->num_philos);

	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		printf("Failed to initialize print mutex\n");
		return (0);
	}
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
	{
		printf("Failed to initialize print mutex\n");
		return (0);
	}
	
	if (!init_forks(data))
		return (0);

	if (!init_philos(data))
		return (0);
	return (1);
}
