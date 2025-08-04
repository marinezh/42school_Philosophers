/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/03 21:11:03 by mzhivoto         ###   ########.fr       */
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

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	i = 0;
	memset(&data, 0, sizeof(data));
	// data = malloc(sizeof(t_data));
	if (ac == 5 || ac == 6)
	{
		if (!parse_arguments(ac, av, &data))
			return (1);
		if (!init_data(&data))
			return (1);
	}
	else
	{
		print_err_msg("Invalid number of arguments");
		printf("%s\n", USAGE);
		return (1);
	}
	while (i < data.num_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, philo_routine,
				&data.philos[i]) != 0)
		{
			// Join already created threads
			for (int j = 0; j < i; j++)
				pthread_join(data.philos[j].thread, NULL);
			
			printf("thread creation failed\n");
			mutex_destroy(&data);
			return (1);
		}
		i++;
	}
	 if(pthread_create(&data.watcher, NULL, run_watcher, &data) != 0)
	{
		// Join all philosopher threads
		for (i = 0; i < data.num_philos; i++)
			pthread_join(data.philos[i].thread, NULL);
			
		printf("thread creation failed\n");
		mutex_destroy(&data);
		return (1);
	}
	pthread_join(data.watcher, NULL);
	
	//Once watcher is done, ensure all philosophers know it's time to exit
	pthread_mutex_lock(&data.death_lock);
	data.is_dead = 1;
	pthread_mutex_unlock(&data.death_lock);
	
	// Now join all philosopher threads
	for (i = 0; i < data.num_philos; i++)
		pthread_join(data.philos[i].thread, NULL);
	
	// Clean up resources
	mutex_destroy(&data);
	return (0);
}
