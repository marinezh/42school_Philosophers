/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/03 04:19:58 by mzhivoto         ###   ########.fr       */
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

// int forks_for_even(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->left_fork->lock);
// 	print_status(philo, "has taken a fork");
// 	if (!is_alive(philo->data))
// 	{
// 		pthread_mutex_unlock(&philo->left_fork->lock);
// 		return (1);
// 	}
// 	pthread_mutex_lock(&philo->right_fork->lock);
// 	print_status(philo, "has taken a fork");
// 	return (0);
// }
int forks_for_even(t_philo *philo)
{
    // Check if critically hungry before taking forks
    long long time_left = philo->data->time_to_die - (get_time_ms() - philo->last_meal_time);
    
    // If this philosopher is critically hungry, try to get forks without yielding
    if (time_left < philo->data->time_to_eat * 1.5)
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
    
    // Not critically hungry - use a two-phase acquisition with yield
    // to reduce deadlocks and give hungrier philosophers a chance
    pthread_mutex_lock(&philo->left_fork->lock);
    print_status(philo, "has taken a fork");
    
    if (!is_alive(philo->data))
    {
        pthread_mutex_unlock(&philo->left_fork->lock);
        return (1);
    }
    
    // Small yield after getting first fork to reduce deadlocks
    usleep(10);
    
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

void *eat(t_philo *philo)
{
	int forks_result;
	
	if (philo->data->must_eat > 0 && philo->meals_ctn >= philo->data->must_eat)
		return NULL;
	if (philo->id % 2 == 0)
		forks_result = forks_for_even(philo);
	else
		forks_result = forks_for_odd(philo);
	if (forks_result != 0 || !is_alive(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);
		return NULL;
	}
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_ms();
	printf("AAA %lld", philo->last_meal_time);
	philo->meals_ctn++;
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is eating");
	ft_dreaming(philo->data, philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->lock);
	pthread_mutex_unlock(&philo->right_fork->lock);
	if (philo->data->must_eat > 0 && philo->meals_ctn == philo->data->must_eat)
		meals_eaten(philo);
	return NULL;
}

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

	if (philo->data->num_philos > 1)
    {
        if (philo->id % 2 == 1)
            ft_usleep(philo->data->time_to_eat / 2);
    }
	if (philo->data->num_philos == 1)
	{
		run_one_philo(philo);
		return NULL;
	}
	
	while (is_alive(philo->data))
	{
		check_starvation_delay(philo);
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

// void check_starvation_delay(t_philo *philo)
// {
// 	long long current_time = get_time_ms();
// 	long long time_since_last_meal = current_time - philo->last_meal_time;
// 	long long time_left = philo->data->time_to_die - time_since_last_meal;
	
// 	if (philo->id % 2 == 1)
// 	{
// 		if (time_left > philo->data->time_to_eat * 2)
// 		{
// 			usleep(1); // 1ms delay
// 		}
// 	}
// }

//  if (philo->id % 2 == 0)
	//  	ft_usleep(500);
	// if (philo->id % 2 == 1)
	// 	ft_usleep(philo->data->time_to_eat / 2);  // Delay half the eating time
	void check_starvation_delay(t_philo *philo)
{
    long long current_time = get_time_ms();
    long long time_since_last_meal = current_time - philo->last_meal_time;
    long long time_left = philo->data->time_to_die - time_since_last_meal;
    
    // Critical starvation threshold - don't delay at all
    if (time_left < philo->data->time_to_eat * 1.5)
    {
        // About to die - don't delay at all
        return;
    }
    
    // Recently ate (has more than 80% of time_to_die left)
    if (time_left > philo->data->time_to_die * 0.8)
    {
        // Recently ate, let others go first
        usleep(500); // 0.5ms delay - significant but not too long
    }
    // Has moderate time left (between 50% and 80% of time_to_die)
    else if (time_left > philo->data->time_to_die * 0.5)
    {
        // Medium priority
        usleep(200); // 0.2ms delay
    }
    // Getting hungry (between 30% and 50% of time_to_die)
    else if (time_left > philo->data->time_to_die * 0.3)
    {
        // Higher priority
        usleep(50); // 0.05ms delay - very short
    }
    // Very hungry but not critical yet - minimal delay
    else
    {
        usleep(10); // 0.01ms - almost no delay
    }
}