/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:21:36 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/05 21:36:28 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_err_msg(char *msg)
{
	int	len;

	len = 0;
	if (!msg)
		return ;
	while (msg[len] != '\0')
		len++;
	write(2, "Error : ", 8);
	write(2, msg, len);
	write(2, "\n", 1);
}

void	print_status(t_philo *philo, const char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	if (!is_alive(philo->data))
	{
		pthread_mutex_unlock(&philo->data->print_lock);
		return ;
	}
	timestamp = get_time_ms() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}
// void	print_status(t_philo *philo, const char *msg, long long timestamp)
// {
// 	pthread_mutex_lock(&philo->data->print_lock);
// 	if (!is_alive(philo->data))
// 	{
// 		pthread_mutex_unlock(&philo->data->print_lock);
// 		return;
// 	}
// 	printf("%lld %d %s\n", timestamp - philo->data->start_time, philo->id + 1, msg);
// 	pthread_mutex_unlock(&philo->data->print_lock);
// }
void	print_death_status(t_philo *philo, const char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	timestamp = get_time_ms() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}
