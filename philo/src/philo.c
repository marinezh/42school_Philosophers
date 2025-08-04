/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 22:26:45 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->right_fork->lock);
	ft_usleep(philo->data->time_to_die + 10);
}

static void	*philo_eat(t_philo *philo)
{
	int	forks_result;
	int	locked_left;
	int	locked_right;

	locked_left = 0;
	locked_right = 0;
	if (philo->data->must_eat > 0 && philo->meals_ctn >= philo->data->must_eat)
		return (NULL);
	if (philo->id % 2 == 0)
		forks_result = forks_for_even(philo, &locked_left, &locked_right);
	else
		forks_result = forks_for_odd(philo, &locked_left, &locked_right);
	if (forks_result != 0 || !is_alive(philo->data))
	{
		release_locked_forks(philo, locked_left, locked_right);
		return (NULL);
	}
	meal_update(philo);
	print_status(philo, "is eating");
	ft_dreaming(philo->data, philo->data->time_to_eat);
	release_locked_forks(philo, 1, 1);
	if (philo->data->must_eat > 0 && philo->meals_ctn == philo->data->must_eat)
		meals_eaten(philo);
	return (NULL);
}

/**
 * philo_sleep - Handles the sleeping state of a philosopher
 * @philo: Pointer to philosopher structure
 *

	* This function makes a philo sleep for the specified time_to_sleep dur-n,
 * but only if the philo will not starve during sleep. It first calculates
 * the time since the philosopher's last meal, and only allows sleeping if the
 * philosopher will still have time left before starvation after sleeping.
 * Safety check prevents philosophers from sleeping when they should be eating.
 */
static void	philo_sleep(t_philo *philo)
{
	t_data		*data;
	long long	now;
	long long	since_meal;

	data = philo->data;
	now = get_time_ms();
	since_meal = now - philo->last_meal_time;
	if (!is_alive(data))
		return ;
	if (since_meal + data->time_to_sleep < data->time_to_die)
	{
		print_status(philo, "is sleeping");
		ft_dreaming(data, data->time_to_sleep);
	}
}

/**
 * philo_think - Handles the thinking state of a philosopher
 * @philo: Pointer to philosopher structure
 *
 * This function makes a philosopher think for a short period. The thinking time
 * is adjusted based on the number of philosophers to optimize performance:
 * - For larger numbers of philosophers (>50): longer thinking time (100ms)
 * - For smaller numbers: minimal thinking time (5ms)
 *
 * Like sleep, it includes a safety check to ensure the philosopher won't starve

	* during thinking. If the time since the last meal plus thinking 
 * time would exceed the time_to_die limit,
 * the philosopher will skip thinking to prioritize eating.
 */

static void	philo_think(t_philo *philo)
{
	int			think_time;
	t_data		*data;
	long long	now;
	long long	since_meal;

	data = philo->data;
	if (!is_alive(data))
		return ;
	if (philo->data->time_to_die - (philo->data->time_to_sleep + philo->data->time_to_eat) < 10)
		think_time = 0;
	else if (data->num_philos > 50)
		think_time = 100;
	else
		think_time = 5;
	now = get_time_ms();
	since_meal = now - philo->last_meal_time;
	if (since_meal + think_time < data->time_to_die)
	{
		print_status(philo, "is thinking");
		ft_dreaming(data, think_time);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->num_philos == 1)
	{
		run_one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (is_alive(data))
	{
		if (!is_alive(data))
			break ;
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		ft_usleep(1 + (philo->id % 5));
	}
	return (NULL);
}
