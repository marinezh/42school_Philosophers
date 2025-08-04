/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:11:16 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 21:13:08 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_resources(t_data *data, int level)
{
	int	i;

	if (level >= CLEANUP_PHILOS && data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	if (level >= CLEANUP_FORKS && data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i].lock);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	if (level >= CLEANUP_DEATH)
		pthread_mutex_destroy(&data->death_lock);
	if (level >= CLEANUP_PRINT)
		pthread_mutex_destroy(&data->print_lock);
}

void	mutex_destroy(t_data *data)
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

void	join_and_cleanup(t_data *data)
{
	int	i;

	pthread_join(data->watcher, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	mutex_destroy(data);
}
