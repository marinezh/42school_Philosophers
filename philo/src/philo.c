/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/29 23:18:12 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_status(t_philo *philo, const char *msg)
{
	long long timestamp = get_time_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%lld Philosopher %d %s\n", timestamp, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->id % 2 == 0)
		ft_usleep(1);

	while (!philo->data->is_dead)
	{
		print_status(philo, "is thinking");

		pthread_mutex_lock(&philo->left_fork->lock);
		print_status(philo, "picked up left fork");
		print_status(philo, "picked up right fork");
		pthread_mutex_lock(&philo->right_fork->lock);
		
		philo->last_meal_time = get_time_ms();
		print_status(philo, "is eating");
		ft_usleep(philo->data->time_to_eat);

		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);

		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
	return NULL;
}
// void	*philo_routine(void *arg)
// {
// 	t_philo	*philo;
// 	long long timestamp;

// 	philo = (t_philo *)arg;
// 	philo->data->start_time = get_time_ms();
// 	timestamp = get_time_ms() - philo->data->start_time;

// 	printf("YYY %d\n", philo->data->is_dead);
// 	while (!philo->data->is_dead)
// 	{
// 		//thinking
// 		timestamp = get_time_ms() - philo->data->start_time;
// 		printf("%lld Philosopher %d is thinking\n",timestamp, philo->id + 1);
// 		
// 		pthread_mutex_lock(&philo->left_fork->lock);

// 		//pick up left fork
// 		timestamp = get_time_ms() - philo->data->start_time;
// 		printf("%lld Philosopher %d picked up left fork %d\n", timestamp, philo->id + 1, philo->left_fork->id + 1);
// 		pthread_mutex_lock(&philo->right_fork->lock);

// 		//pick up right fork
// 		timestamp = get_time_ms() - philo->data->start_time;
// 		printf("%lld Philosopher %d picked up right fork %d\n",timestamp, philo->id + 1,philo->right_fork->id + 1);

// 		philo->last_meal_time = get_time_ms();

// 		//eating
// 		timestamp = get_time_ms() - philo->data->start_time;
// 		printf("%lld Philosopher %d is eating\n",timestamp, philo->id + 1);
// 		usleep(philo->data->time_to_eat * 1000);

// 		pthread_mutex_unlock(&philo->left_fork->lock);
// 		pthread_mutex_unlock(&philo->right_fork->lock);

// 		//sleeping
// 		timestamp = get_time_ms() - philo->data->start_time;
// 		printf("%lld Philosopher %d is sleeping\n",timestamp, philo->id + 1);
// 		usleep(philo->data->time_to_sleep * 1000);
// 	}
// 	return NULL;
// }
