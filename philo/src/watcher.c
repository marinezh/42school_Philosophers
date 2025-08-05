/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:14:29 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/05 21:00:48 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	watch_meals(t_data *data)
{
	pthread_mutex_lock(&data->death_lock);
	if (data->must_eat > 0 && data->num_full == data->num_philos)
	{
		data->is_dead = 1;
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}

static int	watch_death(t_data *data, int i)
{
	long long	time_since_meal;
	long long	timestamp;
	
	pthread_mutex_lock(&data->death_lock);
	if (data->must_eat > 0 && data->philos[i].meals_ctn >= data->must_eat)
	{
		pthread_mutex_unlock(&data->death_lock);
		return (0);
	}
	time_since_meal = get_time_ms() - data->philos[i].last_meal_time;
	if (time_since_meal > data->time_to_die)
	{
		data->is_dead = 1;
		pthread_mutex_unlock(&data->death_lock);
		pthread_mutex_lock(&data->print_lock);
		timestamp = get_time_ms() - data->start_time;
		printf("%lld %d died\n", timestamp, data->philos[i].id + 1);
		pthread_mutex_unlock(&data->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}

void	*run_watcher(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		if (watch_meals(data))
			return (NULL);
		i = 0;
		while (i < data->num_philos)
		{
			if (watch_death(data, i))
				return (NULL);
			i++;
		}
		usleep(10);
	}
	return (NULL);
}
