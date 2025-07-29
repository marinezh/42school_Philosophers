/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/29 22:56:08 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*death_check(void *arg)
{
	t_data	*data;
	int		i;
	long long cur_time;
	
	data = (t_data *)arg;
	
	printf("IS RUNNING \n");
	while(1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			cur_time = get_time_ms() - data->philos[i].last_meal_time;
			//printf("XXXX%lld \n", cur_time);
			if(cur_time > data->time_to_die)
			{
				data->is_dead = 1;
				pthread_mutex_lock(&data->print_lock);
				printf("%lld Philosopher %d died\n",get_time_ms() - data->start_time,data->philos[i].id + 1);
				pthread_mutex_unlock(&data->print_lock);
				return NULL;
			}
			i++;
		}
		usleep(1);
	}
	return NULL;
}

void	pthread_destr(t_data *data, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_join(data->philos[j].thread, NULL);
		j++;
	}
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	// int philo_data[5]; // To store all the parsed values
	i = 0;
	memset(&data, 0, sizeof(data));
	// data = malloc(sizeof(t_data));
	if (ac == 5 || ac == 6)
	{
		if (!parse_arguments(ac, av, &data))
			return (1);
		if (!init_data(&data))
			return (1);
	}
	else
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage:%s <number_of_philosophers><time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n",
			av[0]);
		return (1);
	}
	while (i < data.num_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, philo_routine,
				&data.philos[i]) != 0)
		{
			pthread_destr(&data, i);
			printf("thread creation failed\n");
			// free somthing
			return (1);
		}
		i++;
	}
	if(pthread_create(&data.watcher, NULL, death_check, &data) != 0)
	{
		pthread_destr(&data, data.num_philos);
		printf("thread creation failed\n");
		//free somthing
		return (1);
	}
	i = 0;
	while (i < data.num_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	(pthread_join(data.watcher, NULL));
	//printf("////////////////////////\n");
	//print_philo_data(&data);
	//free(data);
	return (0);
}
