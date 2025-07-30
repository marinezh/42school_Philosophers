/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:21:12 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/30 13:55:47 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to check if a string would cause integer overflow when converted
int	check_overflow(char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i])
	{
		// Check if adding the next digit would cause overflow
		if (result > (INT_MAX - (str[i] - '0')) / 10)
			return (0); // Would overflow
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1); // No overflow
}

int	is_valid_input(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-')
		return (0);
	if (strlen(str) > 10) // INT_MAX is 2,147,483,647 (10 digits)
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			printf("Error: Non-digit character found in input\n");
			return (0);
		}
		i++;
	}
	if (!check_overflow(str))
	{
		printf("Error: Number too large, would cause integer overflow\n");
		return (0);
	}
	return (1);
}
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
	// if (data->num_philos > 200)
	// {
	// 	printf("Error: too many philos\n");
	// 	return (0);
	// }
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
void print_philo_data(t_data *data)
{
	printf("philo num - %d\n", data->num_philos);
	printf("time to die - %d\n", data->time_to_die);
	printf("time to eat - %d\n", data->time_to_eat);
	printf("time to sleep - %d\n", data->time_to_sleep);
}