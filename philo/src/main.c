/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/29 17:45:34 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_arguments(int ac, char **av, t_data *data)
{
	int	i;

	i = 1;
	// Validate all arguments are digits
	while (i < ac)
	{
		if (!is_valid_input(av[i]))
		{
			printf("Error: Invalid argument: %s\n", av[i]);
			return (0);
		}
		i++;
	}
	// Convert arguments to integers
	data->num_philos = atoi(av[1]);    // number_of_philosophers
	data->time_to_die = atoi(av[2]);   // time_to_die
	data->time_to_eat = atoi(av[3]);   // time_to_eat
	data->time_to_sleep = atoi(av[4]); // time_to_sleep
	if (ac == 6)
		data->must_eat = atoi(av[5]);
	// number_of_times_each_philosopher_must_eat (optional)
	else
		data->must_eat = -1; // -1 indicates no limit
	// Check for valid values
	if (data->num_philos <= 0)
	{
		printf("Error: Need at least one philosopher\n");
		return (0);
	}
	if (data->num_philos > 200)
	{
		printf("Error: too many philos\n");
		return (0);
	}
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0)
	{
		printf("Error: Time values must be positive\n");
		return (0);
	}
	if (ac == 6 && data->must_eat <= 0)
	{
		printf("Error: Number of meals must be positive\n");
		return (0);
	}

	return (1);
}
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
		usleep(10);
	}
	return NULL;
}
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long long timestamp;

	philo = (t_philo *)arg;
	philo->data->start_time = get_time_ms();
	timestamp = get_time_ms() - philo->data->start_time;

	printf("YYY %d\n", philo->data->is_dead);
	while (!philo->data->is_dead)
	{
		//thinking
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%lld Philosopher %d is thinking\n",timestamp, philo->id + 1);
		pthread_mutex_lock(&philo->left_fork->lock);

		//pick up left fork
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%lld Philosopher %d picked up left fork %d\n", timestamp, philo->id + 1, philo->left_fork->id + 1);
		pthread_mutex_lock(&philo->right_fork->lock);

		//pick up right fork
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%lld Philosopher %d picked up right fork %d\n",timestamp, philo->id + 1,philo->right_fork->id + 1);

		philo->last_meal_time = get_time_ms();

		//eating
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%lld Philosopher %d is eating\n",timestamp, philo->id + 1);
		usleep(philo->data->time_to_eat * 1000);

		pthread_mutex_unlock(&philo->left_fork->lock);
		pthread_mutex_unlock(&philo->right_fork->lock);

		//sleeping
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%lld Philosopher %d is sleeping\n",timestamp, philo->id + 1);
		usleep(philo->data->time_to_sleep * 1000);
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
