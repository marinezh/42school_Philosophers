/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/02 18:04:01 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pthread_destr(t_data *data, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_join(data->philos[j].thread, NULL);
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
		printf("Error: Invalid number of arguments\n");
		printf("%s\n", USAGE);
		return (1);
	}
	while (i < data.num_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, philo_routine,
				&data.philos[i]) != 0)
		{
			pthread_destr(&data, i);
			printf("thread creation failed\n");
			// free somthing
			return (1);
		}
		i++;
	}
	if(pthread_create(&data.watcher, NULL, run_watcher, &data) != 0)
	{
		pthread_destr(&data, data.num_philos);
		printf("thread creation failed\n");
		//free somthing
		return (1);
	}
	i = 0;
	while (i < data.num_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	(pthread_join(data.watcher, NULL));
	//printf("////////////////////////\n");
	//print_philo_data(&data);
	//free(data);
	return (0);
}
