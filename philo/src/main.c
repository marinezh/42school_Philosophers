/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 03:10:32 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void mutex_destroy(t_data *data)
{
	int	j;

	j = 0;
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->print_lock);
	while (j < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[j].lock);
		j++;
	}
}
int initialize_simulation(int ac, char **av, t_data *data)
{
	memset(data, 0, sizeof(*data));
	
	if (!parse_arguments(ac, av, data))
		return (0);
	if (!init_data(data))
		return (0);
	return 1;
}
int create_philo_threads(t_data *data)
{
	int i;
	int j;
	
	i = 0;
	
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
					&data->philos[i]) != 0)
		{
			// Join already created threads
			j = 0;
			while (j < i)
			{
				pthread_join(data->philos[j].thread, NULL);
				j++;
			}
			print_err_msg("thread creation failed");
			return (0);
		}
		i++;
	}
	return (1);
}

int create_watcher_thread(t_data *data)
{
	int i;

	if(pthread_create(&data->watcher, NULL, run_watcher, data) != 0)
	{
		// Join all philosopher threads
		i = 0;
		while (i < data->num_philos)
		{
			pthread_join(data->philos[i].thread, NULL);
			i++;
		}
		print_err_msg("thread creation failed");
		return (0);
	}
	return (1);
}
void wait_and_cleanup(t_data *data)
{
	int i;
	
	// Wait for watcher thread to finish
	pthread_join(data->watcher, NULL);
	
	// Signal all philosophers to exit
	pthread_mutex_lock(&data->death_lock);
	data->is_dead = 1;
	pthread_mutex_unlock(&data->death_lock);
	
	// Join all philosopher threads
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	
	// Clean up resources
	mutex_destroy(data);
}
int	main(int ac, char **av)
{
	t_data	data;
	
	if (ac != 5 && ac != 6)
	{
		print_err_msg("Invalid number of arguments");
		printf("%s\n", USAGE);
		return (1);
	}
	if(!initialize_simulation(ac, av, &data))
		return (1);

	if (!create_philo_threads(&data))
	{
		mutex_destroy(&data);
		return (1);
	}
	if (!create_watcher_thread(&data))
	{
		mutex_destroy(&data);
		return (1);
	}
	wait_and_cleanup(&data);
	return (0);
}
