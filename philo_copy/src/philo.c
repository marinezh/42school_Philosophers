/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 01:43:21 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->right_fork->lock);
	ft_usleep(philo->data->time_to_die + 10);
}
int take_forks(t_philo *philo)
{
	// Make one philosopher break the circle
	if (philo->id == philo->data->num_philos - 1)
	{
		pthread_mutex_lock(&philo->right_fork->lock);
		print_status(philo, "has taken a fork");
		if (!is_alive(philo->data))
		{
			pthread_mutex_unlock(&philo->right_fork->lock);
			return 1;
		}
		pthread_mutex_lock(&philo->left_fork->lock);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->lock);
		print_status(philo, "has taken a fork");
		if (!is_alive(philo->data))
		{
			pthread_mutex_unlock(&philo->left_fork->lock);
			return 1;
		}
		pthread_mutex_lock(&philo->right_fork->lock);
		print_status(philo, "has taken a fork");
	}
	return 0;
}

void release_forks(t_philo *philo)
{
	// Match the order above
	if (philo->id == philo->data->num_philos - 1)
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);
	}
	else
	{
		pthread_mutex_unlock(&philo->right_fork->lock);
		pthread_mutex_unlock(&philo->left_fork->lock);
	}
}

void *philo_eat(t_philo *philo)
{
	int forks_result;
	
	if (philo->data->must_eat > 0 && philo->meals_ctn >= philo->data->must_eat)
		return NULL;
	// if (philo->id % 2 == 0)
	// 	forks_result = forks_for_even(philo);
	// else
	// 	forks_result = forks_for_odd(philo);
	forks_result = take_forks(philo);
	if (forks_result != 0 || !is_alive(philo->data))
	{
		release_forks(philo);
		return NULL;
	}
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	philo->meals_ctn++;
	pthread_mutex_unlock(&philo->data->death_lock);

	print_status(philo, "is eating");
	ft_dreaming(philo->data, philo->data->time_to_eat);

	release_forks(philo);
	
	if (philo->data->must_eat > 0 && philo->meals_ctn == philo->data->must_eat)
		meals_eaten(philo);
	return NULL;
}

void philo_sleep(t_philo *philo)
{
	t_data *data = philo->data;
	long long now = get_time_ms();
	long long since_meal = now - philo->last_meal_time;
	
	if (since_meal + data->time_to_sleep < data->time_to_die)
	{
		print_status(philo, "is sleeping");
		ft_dreaming(data, data->time_to_sleep);
	}
}

void philo_think(t_philo *philo)
{
	int think_time;
	t_data *data = philo->data;

	if (data->num_philos > 50)
		think_time = 100;
	else
		think_time = 5;
	long long now = get_time_ms();
	long long since_meal = now - philo->last_meal_time;
	
	if (since_meal + think_time < data->time_to_die)
	{
		print_status(philo, "is thinking");
		ft_dreaming(data, think_time);
	}
}

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_data *data = philo->data;

	if (data->num_philos == 1)
	{
		run_one_philo(philo);
		return NULL;
	}
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat);  // Even philosophers wait initially
	while (is_alive(data))
	{
		if (!is_alive(data))
			break;
		philo_eat(philo);
		if (!is_alive(data))
			break;
		philo_sleep(philo);
		if (!is_alive(data))
			break;
		philo_think(philo);
	}
	return NULL;
}

// void check_starvation_delay(t_philo *philo)
// {
// 	long long current_time = get_time_ms();
// 	long long time_since_last_meal = current_time - philo->last_meal_time;
// 	long long time_left = philo->data->time_to_die - time_since_last_meal;
	
// 	if (philo->id % 2 == 1)
// 	{
// 		if (time_left > philo->data->time_to_eat + 10)
// 		{
// 			usleep(1); // 1ms delay
// 		}
// 	}
// }


//  if (philo->id % 2 == 0)
	//  	ft_usleep(500);
	// if (philo->id % 2 == 1)
	// 	ft_usleep(philo->data->time_to_eat / 2);  // Delay half the eating time
	// void sim_delay(t_philo *philo)
// {
// 	if (philo->id % 2 == 0)
// 		think_sleep(philo, philo->data->time_to_eat);
// 	else if (philo->id == philo->data->num_philos && philo->data->num_philos > 1)
// 		think_sleep(philo, philo->data->time_to_eat * 2);
// }


// void	*philo_routine(void *arg)
// {
// 	t_philo	*philo = (t_philo *)arg;

// 	if (philo->data->num_philos == 1)
// 	{
// 		run_one_philo(philo);
// 		return NULL;
// 	}
// 	sim_delay(philo);
// 	while (is_alive(philo->data))
// 	{
// 		//check_starvation_delay(philo);

// 		eat(philo);
// 		print_status(philo, "is sleeping");
		
// 		ft_dreaming(philo->data, philo->data->time_to_sleep);
// 		print_status(philo, "is thinking");
// 		//ft_usleep(5);
// 		if (philo->data->num_philos > 50)
//     		ft_usleep(100);  // Minimal thinking time
// 		else
//     		ft_usleep(5);
// 	}
// 	return NULL;
// }