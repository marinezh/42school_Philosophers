/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:40:19 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/03 23:57:19 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

// void ft_dreaming(t_data *data, int ms)
// {
// 	long long start_time = get_time_ms();
// 	long long current_time;
	
// 	if (ms <= 0)
// 		return;
		
// 	while (is_alive(data))
// 	{
// 		current_time = get_time_ms();
// 		if (current_time - start_time >= ms)
// 			break;
			
// 		// Sleep in small chunks to be able to check is_alive frequently
// 		usleep(100);  // 0.5ms intervals for responsiveness
// 	}
// }
void ft_dreaming(t_data *data, int duration_ms)
{
	long long start = get_time_ms();
	long long now;
	long long elapsed;

	while (is_alive(data))
	{
		now = get_time_ms();
		elapsed = now - start;

		if (elapsed >= duration_ms)
			break;

		// Sleep only the remaining time or a short chunk
		int sleep_time = duration_ms - elapsed;
		if (sleep_time > 5)
			usleep(1000);  // 1ms chunks
		else
			usleep(100);   // fine-grained finish
	}
}

// void ft_dreaming(t_data *data, int ms)
// {
// 	long long start = get_time_ms();
// 	while (is_alive(data) && get_time_ms() - start < ms)
// 		usleep(500); // Short intervals
// }

int	ft_atoi(const char *str)
{
	int			i;
	long long	nbr;
	int			minus;

	i = 0;
	nbr = 0;
	minus = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		minus = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	return (nbr * minus);
}
