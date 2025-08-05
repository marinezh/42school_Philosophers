/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/05 20:52:12 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_simulation(int ac, char **av, t_data *data)
{
	memset(data, 0, sizeof(*data));
	if (!parse_arguments(ac, av, data))
		return (0);
	if (!init_data(data))
		return (0);
	return (1);
}

static void	set_death(t_data *data)
{
	pthread_mutex_lock(&data->death_lock);
	data->is_dead = 1;
	pthread_mutex_unlock(&data->death_lock);
}

int	create_philo_threads(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
		{
			j = 0;
			set_death(data);
			print_err_msg("thread creation failed");
			while (j < i)
			{
				pthread_join(data->philos[j].thread, NULL);
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

int	create_watcher_thread(t_data *data)
{
	int	i;

	if (pthread_create(&data->watcher, NULL, run_watcher, data) != 0)
	{
		i = 0;
		set_death(data);
		print_err_msg("thread creation failed");
		while (i < data->num_philos)
		{
			pthread_join(data->philos[i].thread, NULL);
			i++;
		}
		return (0);
	}
	return (1);
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
	if (!initialize_simulation(ac, av, &data))
		return (1);
	if (create_philo_threads(&data) != 1)
	{
		cleanup_simulation(&data);
		cleanup_resources(&data, CLEANUP_PHILOS);
		return (1);
	}
	if (!create_watcher_thread(&data))
	{
		cleanup_simulation(&data);
		cleanup_resources(&data, CLEANUP_PHILOS);
		return (1);
	}
	join_and_cleanup(&data);
	return (0);
}
