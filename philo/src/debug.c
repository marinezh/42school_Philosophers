/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 16:08:26 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/02 16:27:48 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
void print_philo_data(t_data *data)
{
	printf("philo num - %d\n", data->num_philos);
	printf("time to die - %d\n", data->time_to_die);
	printf("time to eat - %d\n", data->time_to_eat);
	printf("time to sleep - %d\n", data->time_to_sleep);
}