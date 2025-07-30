/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:40:19 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/30 23:51:03 by mzhivoto         ###   ########.fr       */
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

	gettimeofday(&tv, NULL); // think about aaing check for failing this func
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int ms)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(500);
	
}

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

// void ft_dreaming(t_data *data, int ms)
// {
// 	long long	rest;
// 	int count;
// 	int i = 1;

// 	if (ms < data->time_to_die || ms < 50)
// 	{
// 		ft_usleep(ms);
// 		return ;
// 	}
// 	count = ms/50;
// 	rest = ms;
// 	while(rest > 0 && is_alive(data))
// 	{
// 		rest -= 50;
// 		ft_usleep(50);
// 		if (i == count && ms%50 != 0)
// 		{
// 			rest = ms - 50*count;
// 		}
// 		i++;
// 	}

// void ft_dreaming(t_data *data, int ms)
// {
// 	long long elapsed = 0;

// 	if (ms <= 0)
// 		return;

// 	while (elapsed + 50 <= ms && is_alive(data))
// 	{
// 		ft_usleep(50);
// 		elapsed += 50;
// 	}

// 	// Sleep the remainder if still alive
// 	if (is_alive(data) && elapsed < ms)
// 		ft_usleep(ms - elapsed);
// }

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
        usleep(500);  // 0.5ms intervals for responsiveness
    }
}