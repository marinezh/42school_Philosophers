/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:21:36 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/01 19:28:27 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, const char *msg)
{
	long long	timestamp;

	timestamp = get_time_ms() - philo->data->start_time;
	if (is_alive(philo->data))
	{
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}

void	print_death_status(t_philo *philo, const char *msg)
{
	long long	timestamp;

	timestamp = get_time_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}
