/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/31 18:42:12 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void *death_check(void *arg)
{
	t_data *data;
	int i;
	long long cur_time;
	data = (t_data *)arg;
	
	while (1)
	{
		i = 0;
		// First check if simulation should end (all philosophers are full)
		if (data->must_eat > 0)
		{
			pthread_mutex_lock(&data->death_lock);
			if (data->num_full == data->num_philos)
			{
				data->is_dead = 1;
				pthread_mutex_unlock(&data->death_lock);
				return NULL;
			}
			pthread_mutex_unlock(&data->death_lock);
		}
		
		// Then check each philosopher for starvation
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_lock);
			cur_time = get_time_ms() - data->philos[i].last_meal_time;
		// 	printf("DEBUG: Philosopher %d, cur_time: %lld, last_meal_time: %lld\n", 
		// data->philos[i].id + 1, cur_time, data->philos[i].last_meal_time);
			if (cur_time > data->time_to_die)
			{
				data->is_dead = 1;
				pthread_mutex_lock(&data->print_lock);
				printf("%ld %d died\n", (long)(get_time_ms() - data->start_time), data->philos[i].id + 1);
				pthread_mutex_unlock(&data->print_lock);
				pthread_mutex_unlock(&data->death_lock);
				return NULL;
			}
			pthread_mutex_unlock(&data->death_lock);
			i++;
		}
		
		// Sleep longer to reduce CPU usage, but not so long we miss a death
		usleep(5);
	}
	return NULL;
}

void print_status(t_philo *philo, const char *msg)
{
	long long timestamp = get_time_ms() - philo->data->start_time;
	
	if (is_alive(philo->data))
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}  // Only print if simulation is still running
}


int is_alive(t_data *data)
{
	pthread_mutex_lock(&data->death_lock);
	if (data->is_dead == 1)
	{
		pthread_mutex_unlock(&data->death_lock);
		return 0;
	}
	pthread_mutex_unlock(&data->death_lock);
	return 1;
}
int	eat(t_philo *philo)
{
	// Lock forks based on philosopher ID (deadlock prevention)
	// if (philo->id % 2 == 0)
	// {
	// 	pthread_mutex_lock(&philo->right_fork->lock);
	// 	print_status(philo, "has taken a fork");

	// 	pthread_mutex_lock(&philo->left_fork->lock);
	// 	print_status(philo, "has taken a fork");
	// }
	// else
	// {
	// 	pthread_mutex_lock(&philo->left_fork->lock);
	// 	print_status(philo, "has taken a fork");

	// 	pthread_mutex_lock(&philo->right_fork->lock);
	// 	print_status(philo, "has taken a fork");
	// }
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(&philo->left_fork->lock);
	print_status(philo, "has taken a fork");
	// Update meal time safely
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	//printf("philo is %d lm time - %lld\n", philo->id + 1, philo->last_meal_time);
	philo->meals_eaten++;
	// time for next meal in ms
	pthread_mutex_unlock(&philo->data->death_lock);

	print_status(philo, "is eating");
	ft_dreaming(philo->data, philo->data->time_to_eat);

	// Release forks
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_unlock(&philo->right_fork->lock);

	// Handle must_eat logic
	if (philo->data->must_eat > 0 && philo->meals_eaten == philo->data->must_eat)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		philo->data->num_full++;
		pthread_mutex_unlock(&philo->data->death_lock);
		return 1; // philosopher is done eating
	}
	return 0; // continue looping
}


void run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->right_fork->lock);
	ft_usleep(philo->data->time_to_die + 10); 
}
void	check_time_for_odd(t_philo *philo)
{
	size_t	diff;

	if (philo->id % 2 == 1)
	{
		diff = philo->last_meal_time - get_time_ms();
		if (diff > 0)
			ft_usleep(2);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		run_one_philo(philo);
		return NULL;
	}
	 if (philo->id % 2 == 1)
	 	ft_usleep(500);
	//ft_usleep(philo->id * 100);

	while (is_alive(philo->data))
	{
		print_status(philo, "is thinking");
		if (is_alive(philo->data))
		{
			// if (philo->id%2 != 0)
			// 	ft_usleep(1);
			// check_time_for_odd(philo);
			eat(philo);
			// if(eat(philo) == 1)
			// 	break;
		}
		if (is_alive(philo->data))
		{
			print_status(philo, "is sleeping");
			//ft_usleep(philo->data->time_to_sleep);
			ft_dreaming(philo->data, philo->data->time_to_sleep);
		}
	}
	return NULL;
}
