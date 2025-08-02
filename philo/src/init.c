/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:42:32 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/02 19:47:46 by mzhivoto         ###   ########.fr       */
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

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
	{
		print_err_msg("memory allocation failed");
		return (0);
	}
	i = 0;
	while (i < data->num_philos)
	{
		data->forks[i].id = i;
		if (pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
		{
			printf("Failed to initialize fork mutex %d\n", i);
			cleanup_resources(data, CLEANUP_FORKS);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	if (!data->forks)
		return (print_err_msg("Cannot init philosophers: no forks."), 0);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (print_err_msg("memory allocation failed"), 0);
	i = 0;
	while (i < data->num_philos)
	{
		memset(&data->philos[i], 0, sizeof(t_philo));
		data->philos[i].id = i;
		data->philos[i].meals_ctn = 0;
		data->philos[i].last_meal_time = get_time_ms();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	data->start_time = get_time_ms();
	data->philos = NULL;
	data->forks = NULL;
	data->is_dead = 0;
	data->num_full = 0;
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (print_err_msg("Failed to initialize print mutex"), 0);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
	{
		cleanup_resources(data, CLEANUP_PRINT);
		return (print_err_msg("Failed to initialize death mutex"), 0);
	}
	if (!init_forks(data))
	{
		cleanup_resources(data, CLEANUP_DEATH);
		return (0);
	}
	if (!init_philos(data))
	{
		cleanup_resources(data, CLEANUP_FORKS);
		return (0);
	}
	return (1);
}
