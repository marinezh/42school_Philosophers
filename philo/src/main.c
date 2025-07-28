/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/28 18:08:50 by mzhivoto         ###   ########.fr       */
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
	data->num_philos = atoi(av[1]); // number_of_philosophers
	data->time_to_die = atoi(av[2]); // time_to_die
	data->time_to_eat = atoi(av[3]); // time_to_eat
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
	if (data->num_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0)
	{
		printf("Error: Time values must be positive\n");
		return (0);
	}
	if (ac == 6 && data->must_eat <= 0)
	{
		printf("Error: Number of meals must be positive\n");
		return (0);
	}
	printf("XXXXXXXXXXXXXX\n");
	if (data->time_to_die < data->time_to_eat)
	{
		data->time_to_eat = data->time_to_die;
		//printf("CHECK %d\n", data->time_to_eat);
	}
	if (data->time_to_die < data->time_to_sleep)
	{
		data->time_to_sleep = data->time_to_die;
		//printf("CHECK %d\n", data->time_to_sleep);
	}
		
	return (1);
}
void philosopher(int id)
{
	while(1)
	{
		printf("Philosopher %d is thinking.\n", id);
		sleep(1);
		printf("Philosopher %d is eating.\n", id);
		sleep(1);
		printf("Philosopher %d is sleeping.\n", id);
		sleep(1);
	}
}

int	main(int ac, char **av)
{
	//int philo_data[5]; // To store all the parsed values
	t_data data;
	memset(&data, 0, sizeof(data));
	//data = malloc(sizeof(t_data));
	if (ac == 5 || ac == 6)
	{
		if (!parse_arguments(ac, av, &data))
			return 1;
		if (!init_data(&data))
			return (1);
		// printf("Number of philosophers: %d\n", data.num_philos);
		// printf("Time to die: %d ms\n", data.time_to_die);
		// printf("Time to eat: %d ms\n", data.time_to_eat);
		// printf("Time to sleep: %d ms\n", data.time_to_sleep);
		// if (philo_data[4] != -1)
		// 	printf("Number of times each philosopher must eat: %d\n",
		// 		philo_data[4]);
		// else
		// 	printf("Philosophers will eat until simulation stops\n");
		// Continue with the rest of your program here...
	}
	else
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage:%s <number_of_philosophers><time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n",
			av[0]);
		return 1;
	}
	//printf("////////////////////////\n");
	//print_philo_data(&data);

	//free(data);
	philosopher(1);
	philosopher(2);
	return 0;
}
