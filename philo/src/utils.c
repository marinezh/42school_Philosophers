/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:40:19 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/04 20:59:17 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_alive(t_data *data)
{
	bool	alive;

	pthread_mutex_lock(&data->death_lock);
	alive = !data->is_dead;
	pthread_mutex_unlock(&data->death_lock);
	return (alive);
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

void	ft_dreaming(t_data *data, int ms)
{
	long long	start;

	start = get_time_ms();
	while (is_alive(data) && get_time_ms() - start < ms)
		usleep(100);
}

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
