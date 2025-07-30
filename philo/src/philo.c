/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:31 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/31 00:18:13 by mzhivoto         ###   ########.fr       */
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
        usleep(500);
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
// void print_status(t_philo *philo, const char *msg)
// {
// 	long long timestamp = get_time_ms() - philo->data->start_time;
// 	pthread_mutex_lock(&philo->data->print_lock);
// 	if (is_alive(philo->data))
// 		printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
// 	pthread_mutex_unlock(&philo->data->print_lock);
// }
int eat(t_philo *philo)
{
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
			return 1; // philo is done eaing
		}
	}
	return 0; // philo is continue eating
}

void run_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->right_fork->lock);
	print_status(philo, "has taken a fork");
	 ft_usleep(philo->data->time_to_die + 10); 
	pthread_mutex_unlock(&philo->right_fork->lock);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		run_one_philo(philo);
		return NULL;
	}
	 if (philo->id % 2 == 0)
	 	ft_usleep(10);

	while (is_alive(philo->data))
	{
		print_status(philo, "is thinking");
		if (is_alive(philo->data))
			if(eat(philo) == 1)
				break;
			eat(philo);
		if (is_alive(philo->data))
		{
			print_status(philo, "is sleeping");
			//ft_usleep(philo->data->time_to_sleep);
			ft_dreaming(philo->data, philo->data->time_to_sleep);
		}
	}
	return NULL;
}
