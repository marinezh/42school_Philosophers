/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:40:19 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/01 17:25:39 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int ms)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(100);
	
}

void ft_dreaming(t_data *data, int ms)
{
	long long start_time = get_time_ms();
	long long current_time;
	
	if (ms <= 0)
		return;
		
	while (is_alive(data))
	{
		current_time = get_time_ms();
		if (current_time - start_time >= ms)
			break;
			
		// Sleep in small chunks to be able to check is_alive frequently
		usleep(100);  // 0.5ms intervals for responsiveness
	}
}
