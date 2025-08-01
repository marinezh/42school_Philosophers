/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:14:29 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/01 13:23:04 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *death_check(void *arg)
{
	t_data *data = (t_data *)arg;
	int i;
	long long time_since_meal;

	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_lock);

			// All philosophers full?
			if (data->must_eat > 0 && data->num_full == data->num_philos)
			{
				data->is_dead = 1;
				pthread_mutex_unlock(&data->death_lock);
				return NULL;
			}

			// Skip check if philosopher is already full
			if (data->must_eat > 0 && data->philos[i].meals_eaten >= data->must_eat)
			{
				pthread_mutex_unlock(&data->death_lock);
				i++;
				continue;
			}

			// Death condition
			time_since_meal = get_time_ms() - data->philos[i].last_meal_time;
			if (time_since_meal > data->time_to_die)
			{
				data->is_dead = 1;
				pthread_mutex_lock(&data->print_lock);
				printf("%lld %d died\n",
					get_time_ms() - data->start_time,
					data->philos[i].id + 1);
				pthread_mutex_unlock(&data->print_lock);
				pthread_mutex_unlock(&data->death_lock);
				return NULL;
			}
			pthread_mutex_unlock(&data->death_lock);
			i++;
		}
		usleep(100); // Slightly higher to reduce CPU usage
	}
	return NULL;
}

// void	*death_check(void *arg)
// {
// 	t_data	*data;
// 	int		i;
// 	long long time_since_meal;
	
// 	data = (t_data *)arg;
	
// 	while(1)
// 	{
// 		i = 0;
// 		while (i < data->num_philos)
// 		{
// 			pthread_mutex_lock(&data->death_lock);
// 			time_since_meal = get_time_ms() - data->philos[i].last_meal_time;
//
// 			if (data->num_full == data->num_philos)
// 			{
// 				//pthread_mutex_lock(&data->death_lock);
// 				data->is_dead = 1;
// 				pthread_mutex_unlock(&data->death_lock);
// 				return NULL;
// 			}
// 			//pthread_mutex_lock(&data->meal_lock);
// 			if(time_since_meal > data->time_to_die)
// 			{
// 				data->is_dead = 1;
// 				pthread_mutex_lock(&data->print_lock);
// 				printf("%lld %d died\n",get_time_ms() - data->start_time,data->philos[i].id + 1);
// 				pthread_mutex_unlock(&data->print_lock);
// 				pthread_mutex_unlock(&data->death_lock);
// 				return NULL;
// 			}
// 			pthread_mutex_unlock(&data->death_lock);
// 			i++;
// 		}
// 		usleep(5);
// 	}
// 	return NULL;
// }
// void *death_check(void *arg)
// {
// 	t_data *data;
// 	int i;
// 	long long time_since_meal;
// 	data = (t_data *)arg;
	
// 	while (1)
// 	{
// 		i = 0;
// 		// First check if simulation should end (all philosophers are full)
// 		if (data->must_eat > 0)
// 		{
// 			pthread_mutex_lock(&data->death_lock);
// 			if (data->num_full == data->num_philos)
// 			{
// 				data->is_dead = 1;
// 				pthread_mutex_unlock(&data->death_lock);
// 				return NULL;
// 			}
// 			pthread_mutex_unlock(&data->death_lock);
// 		}
		
// 		// Then check each philosopher for starvation
// 		while (i < data->num_philos)
// 		{
// 			pthread_mutex_lock(&data->death_lock);
// 			time_since_meal = get_time_ms() - data->philos[i].last_meal_time;
// 		// 	printf("DEBUG: Philosopher %d, cur_time: %lld, last_meal_time: %lld\n", 
// 		// data->philos[i].id + 1, cur_time, data->philos[i].last_meal_time);
// 			if (time_since_meal > data->time_to_die)
// 			{
// 				data->is_dead = 1;
// 				pthread_mutex_lock(&data->print_lock);
// 				printf("%ld %d died\n", (long)(get_time_ms() - data->start_time), data->philos[i].id + 1);
// 				pthread_mutex_unlock(&data->print_lock);
// 				pthread_mutex_unlock(&data->death_lock);
// 				return NULL;
// 			}
// 			pthread_mutex_unlock(&data->death_lock);
// 			i++;
// 		}
		
// 		// Sleep longer to reduce CPU usage, but not so long we miss a death
// 		usleep(5);
// 	}
// 	return NULL;
// }