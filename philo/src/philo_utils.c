/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 00:51:13 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 00:54:53 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int forks_for_even(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->lock);
	print_status(philo, "has taken a fork");
	if (!is_alive(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		return (1);
	}
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	return (0);
}

int forks_for_odd(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	if (!is_alive(philo->data))
	{
		pthread_mutex_unlock(&philo->right_fork->lock);
		return (1);
	}
	pthread_mutex_lock(&philo->left_fork->lock);
	print_status(philo, "has taken a fork");
	return (0);
}

int meals_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->data->num_full++;
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

void release_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->right_fork->lock);
		pthread_mutex_unlock(&philo->left_fork->lock);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);
	}
}
