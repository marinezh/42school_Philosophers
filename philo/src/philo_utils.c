/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 00:51:13 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 20:39:07 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meal_update(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	philo->meals_ctn++;
	pthread_mutex_unlock(&philo->data->death_lock);
}

int	forks_for_even(t_philo *philo, int *locked_left, int *locked_right)
{
	if (pthread_mutex_lock(&philo->left_fork->lock) != 0)
		return (1);
	*locked_left = 1;
	print_status(philo, "has taken a fork");
	if (!is_alive(philo->data))
		return (1);
	if (pthread_mutex_lock(&philo->right_fork->lock) != 0)
		return (1);
	*locked_right = 1;
	print_status(philo, "has taken a fork");
	return (0);
}

int	forks_for_odd(t_philo *philo, int *locked_left, int *locked_right)
{
	if (pthread_mutex_lock(&philo->right_fork->lock) != 0)
		return (1);
	*locked_right = 1;
	print_status(philo, "has taken a fork");
	if (!is_alive(philo->data))
		return (1);
	if (pthread_mutex_lock(&philo->left_fork->lock) != 0)
		return (1);
	*locked_left = 1;
	print_status(philo, "has taken a fork");
	return (0);
}

void	release_locked_forks(t_philo *philo, int locked_left, int locked_right)
{
	if (locked_left)
		pthread_mutex_unlock(&philo->left_fork->lock);
	if (locked_right)
		pthread_mutex_unlock(&philo->right_fork->lock);
}

int	meals_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->data->num_full++;
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}
