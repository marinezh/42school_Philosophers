/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:42:32 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/27 18:50:39 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_forks(t_data *data)
{
	int i;

	printf("forks init\n");
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		printf("memory allocation failed\n");
	}
	printf("CHECK FORKS %d\n", data->num_philos);
	i = 0;
	while (i < data->num_philos)
	{
		if(pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Failed to initialize fork mutex %d\n", i);
			return (0);
		}
		printf("Fork %d initialized\n", i);
		i++;
	}
	return 1;
}