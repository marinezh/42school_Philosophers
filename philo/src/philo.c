/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/01 19:23:31 by mzhivoto         ###   ########.fr       */
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
// void	check_starvation_delay(t_philo *philo)
// {
// 	long long time_now;
// 	long long time_left;

// 	if (philo->id % 2 == 1) // or even, depending on how your forks behave
// 	{
// 		time_now = get_time_ms();
// 		time_left = (philo->last_meal_time + philo->data->time_to_die) - time_now;

// 		if (time_left > 0 && time_left < 10) // Close to starvation window
// 			usleep(100); // Let other threads move — reduce fork contention
// 	}
// }

void log_fork_action(t_philo *philo, t_fork *fork, const char *side, const char *action)
{
	long long timestamp = get_time_ms() - philo->data->start_time;
	if (is_alive(philo->data))
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s fork %d (%s)\n", timestamp, philo->id + 1, action, fork->id, side);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

void *eat(t_philo *philo)
{
	if (philo->data->must_eat > 0 && philo->meals_ctn >= philo->data->must_eat)
		return NULL;

	// Lock forks based on philosopher ID parity
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork->lock);
		//log_fork_action(philo, philo->left_fork, "left", "has taken");
		print_status(philo, "has taken a fork");
		if (!is_alive(philo->data))
		{
			pthread_mutex_unlock(&philo->left_fork->lock);
			//log_fork_action(philo, philo->left_fork, "left", "has released");
			return NULL;
		}

		pthread_mutex_lock(&philo->right_fork->lock);
		//log_fork_action(philo, philo->right_fork, "right", "has taken");
		print_status(philo, "has taken a fork");
		
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->lock);
		//log_fork_action(philo, philo->right_fork, "right", "has taken");
		print_status(philo, "has taken a fork");

		if (!is_alive(philo->data))
		{
			pthread_mutex_unlock(&philo->right_fork->lock);
			//log_fork_action(philo, philo->right_fork, "right", "has released");
			return NULL;
		}

		pthread_mutex_lock(&philo->left_fork->lock);
		//log_fork_action(philo, philo->left_fork, "left", "has taken");
		print_status(philo, "has taken a fork");
	}

	// Final safety check before eating
	if (!is_alive(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		//log_fork_action(philo, philo->left_fork, "left", "has released");

		pthread_mutex_unlock(&philo->right_fork->lock);
		//log_fork_action(philo, philo->right_fork, "right", "has released");
		return NULL;
	}

	// Update meal tracking
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	philo->meals_ctn++;
	pthread_mutex_unlock(&philo->data->death_lock);

	print_status(philo, "is eating");
	ft_dreaming(philo->data, philo->data->time_to_eat);

	// Release forks after eating
	pthread_mutex_unlock(&philo->left_fork->lock);
	//log_fork_action(philo, philo->left_fork, "left", "has released");
	pthread_mutex_unlock(&philo->right_fork->lock);
	//log_fork_action(philo, philo->right_fork, "right", "has released");

	// Mark philosopher as full if required
	if (philo->data->must_eat > 0 && philo->meals_ctn == philo->data->must_eat)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		philo->data->num_full++;
		pthread_mutex_unlock(&philo->data->death_lock);
	}

	return NULL;
}

// void *eat(t_philo *philo)
// {
// 	if (philo->data->must_eat > 0 && philo->meals_eaten >= philo->data->must_eat)
// 		return NULL;

// 	// Lock order depends on philosopher ID
// 	if (philo->id % 2 == 0)
// 	{
// 		pthread_mutex_lock(&philo->left_fork->lock);
// 		print_status(philo, "has taken the left fork");
// 		if (!is_alive(philo->data))
// 		{
// 			pthread_mutex_unlock(&philo->left_fork->lock);
// 			return NULL;
// 		}
// 		pthread_mutex_lock(&philo->right_fork->lock);
// 		print_status(philo, "has taken the right fork");
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&philo->right_fork->lock);
// 		print_status(philo, "has taken the right fork");
// 		if (!is_alive(philo->data))
// 		{
// 			pthread_mutex_unlock(&philo->right_fork->lock);
// 			return NULL;
// 		}
// 		pthread_mutex_lock(&philo->left_fork->lock);
// 		print_status(philo, "has taken the left fork");
// 	}

// 	// Final check before eating
// 	if (!is_alive(philo->data))
// 	{
// 		pthread_mutex_unlock(&philo->left_fork->lock);
// 		print_status(philo, "released the left fork");
// 		pthread_mutex_unlock(&philo->right_fork->lock);
// 		print_status(philo, "released the right fork");
// 		return NULL;
// 	}

// 	// Safe to update meal time
// 	pthread_mutex_lock(&philo->data->death_lock);
// 	philo->last_meal_time = get_time_ms();
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->data->death_lock);

// 	print_status(philo, "is eating");
// 	ft_dreaming(philo->data, philo->data->time_to_eat);

// 	// Release forks after eating
// 	pthread_mutex_unlock(&philo->left_fork->lock);
// 	print_status(philo, "released the left fork");
// 	pthread_mutex_unlock(&philo->right_fork->lock);
// 	print_status(philo, "released the right fork");

// 	// Optional: track full philosophers
// 	if (philo->data->must_eat > 0 && philo->meals_eaten == philo->data->must_eat)
// 	{
// 		pthread_mutex_lock(&philo->data->death_lock);
// 		philo->data->num_full++;
// 		pthread_mutex_unlock(&philo->data->death_lock);
// 	}
// 	return NULL;
// }

void run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->right_fork->lock);
	ft_usleep(philo->data->time_to_die + 10);
}

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
	// if (philo->id % 2 == 1)
	// 	ft_usleep(philo->data->time_to_eat / 2);  // Delay half the eating time

	while (is_alive(philo->data))
	{
		//check_starvation_delay(philo);

		if (!is_alive(philo->data))
			break;

		eat(philo);

		if (!is_alive(philo->data))
			break;

		print_status(philo, "is sleeping");
		ft_dreaming(philo->data, philo->data->time_to_sleep);
		print_status(philo, "is thinking");
		ft_dreaming(philo->data, 5);
	}
	return NULL;
}

void check_starvation_delay(t_philo *philo)
{
	long long current_time = get_time_ms();
	long long time_since_last_meal = current_time - philo->last_meal_time;
	long long time_left = philo->data->time_to_die - time_since_last_meal;
	
	if (philo->id % 2 == 1)
	{
		if (time_left > philo->data->time_to_eat * 2)
		{
			usleep(1); // 1ms delay
		}
	}
}