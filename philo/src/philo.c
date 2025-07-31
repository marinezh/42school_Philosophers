/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/31 22:18:59 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
// int is_full(t_data *data)
// {
// 	pthread_mutex_lock(&data->death_lock);
// 	if (data->philos->meals_eaten >= data->must_eat)
// 	{
// 		pthread_mutex_unlock(&data->death_lock);
// 		return 0;
// 	}
// 	pthread_mutex_unlock(&data->death_lock);
// 	return 1;
// }
void	check_starvation_delay(t_philo *philo)
{
	long long time_now;
	long long time_left;

	if (philo->id % 2 == 1) // or even, depending on how your forks behave
	{
		time_now = get_time_ms();
		time_left = (philo->last_meal_time + philo->data->time_to_die) - time_now;

		if (time_left > 0 && time_left < 10) // Close to starvation window
			usleep(100); // Let other threads move — reduce fork contention
	}
}
void *eat(t_philo *philo)
{
	if (philo->data->must_eat > 0 && philo->meals_eaten >= philo->data->must_eat)
		return NULL;
	
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork->lock);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->lock);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->lock);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->lock);
		print_status(philo, "has taken a fork");
	}
	
	// pthread_mutex_lock(&philo->left_fork->lock);
	// print_status(philo, "has taken a fork");
	// pthread_mutex_lock(&philo->right_fork->lock);
	// print_status(philo, "has taken a fork");

	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);
	// TO DO : CHECK if philo dies what next, do i need to unluck fork
	print_status(philo, "is eating");
	//ft_usleep(philo->data->time_to_eat);
	ft_dreaming(philo->data, philo->data->time_to_eat);
	
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_unlock(&philo->right_fork->lock);
	if (philo->data->must_eat > 0)
	{
		if (philo->data->must_eat == philo->meals_eaten)
		{
			pthread_mutex_lock(&philo->data->death_lock);
			philo->data->num_full++;
			pthread_mutex_unlock(&philo->data->death_lock);
			//return NULL;
		}
	}
	return NULL;
}

void run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->right_fork->lock);
	ft_usleep(philo->data->time_to_die + 10);
}
// void	check_time_for_odd(t_philo *philo)
// {
// 	size_t	diff;

// 	if (philo->ph_id % 2 == 1)
// 	{
// 		diff = philo->t_next_meal - get_time();
// 		if (diff > 0)
// 			ft_usleep(1, philo->arg_info);
// 	}
// }

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		run_one_philo(philo);
		return NULL;
	}
	//  if (philo->id % 2 == 0)
	//  	ft_usleep(500);
	check_starvation_delay(philo);
	while (is_alive(philo->data))
	{
		

		// long long now = get_time_ms(); // assuming this gets current time in ms
		// long long time_since_last_meal = now - philo->last_meal_time;
		// long long left_to_die = philo->data->time_to_die - time_since_last_meal;
		// //printf("						PHILO id %d, last meal time %lld\n", philo->id + 1, time_since_last_meal);
		// //printf("						left to die %lld\n", left_to_die);
		// long long safety_margin = 10;
		// if (philo->id % 2 == 1)
		// {
		// 	if (left_to_die > philo->data->time_to_eat + safety_margin)
		// 		ft_usleep(1);
		// }
		
		// else
		// {
			if (is_alive(philo->data))
				eat(philo);
		//}
		if (is_alive(philo->data))
		{
			print_status(philo, "is sleeping");
			//ft_usleep(philo->data->time_to_sleep);
			ft_dreaming(philo->data, philo->data->time_to_sleep);
			print_status(philo, "is thinking");
		}
		
	}
	return NULL;
}
