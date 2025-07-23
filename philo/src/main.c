/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/23 23:51:49 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Function to check if a string would cause integer overflow when converted
int check_overflow(char *str)
{
	long result = 0;
	int i = 0;

	while (str[i])
	{
		// Check if adding the next digit would cause overflow
		if (result > (INT_MAX - (str[i] - '0')) / 10)
			return 0;  // Would overflow

		result = result * 10 + (str[i] - '0');
		i++;
	}

	return 1;  // No overflow
}
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
 int is_valid_input(char *str)
 {
	int i = 0;

	if (!str || !str[0])
		return 0;
	if (str[0] == '-')
		return 0;
	if (strlen(str) > 10)  // INT_MAX is 2,147,483,647 (10 digits)
 		return 0;
	while (str[i])
	{
		if(!ft_isdigit(str[i]))
		{
			 printf("Error: Non-digit character found in input\n");
			return 0;
		}
		i++;
	}
	if (!check_overflow(str))
	{
		printf("Error: Number too large, would cause integer overflow\n");
		return 0;
	}
	return 1;
 }

int parse_arguments(int ac, char **av, int *philo_data)
{
	int i = 1;
	
	// Validate all arguments are digits
	while (i < ac)
	{
		if (!is_valid_input(av[i]))
		{
			printf("Error: Invalid argument: %s\n", av[i]);
			return 0;
		}
		i++;
	}
	// Convert arguments to integers
	philo_data[0] = atoi(av[1]); // number_of_philosophers
	philo_data[1] = atoi(av[2]); // time_to_die
	philo_data[2] = atoi(av[3]); // time_to_eat
	philo_data[3] = atoi(av[4]); // time_to_sleep
	if (ac == 6)
		philo_data[4] = atoi(av[5]); // number_of_times_each_philosopher_must_eat (optional)
	else
		philo_data[4] = -1; // -1 indicates no limit
	// Check for valid values
	if (philo_data[0] <= 0)
	{
		printf("Error: Need at least one philosopher\n");
		return 0;
	}
	if (philo_data[1] <= 0 || philo_data[2] <= 0 || philo_data[3] <= 0)
	{
		printf("Error: Time values must be positive\n");
		return 0;
	}
	if (ac == 6 && philo_data[4] <= 0)
	{
		printf("Error: Number of meals must be positive\n");
		return 0;
	}
	return 1;
}

// int main(int ac, char **av)
// {
// 	if (ac == 5 || ac == 6)
// 	{
// 		char **input = &av[1];
// 		// while (input[i])
// 		// {
// 		// 	printf("CHECK %s\n", input[i]);
// 		// 	i++;
// 		// }

// 		int number_of_philos = 0;
// 		int time_to_die = 0;
// 		int time_to_eat = 0;
// 		int time_to_sleep = 0;
// 		int number_of_how_much_eat = 0;

// 		number_of_philos = atoi(av[1]);
// 		time_to_die = atoi(av[2]);
// 		time_to_eat = atoi(av[3]);
// 		time_to_sleep = atoi(av[4]);
// 		number_of_how_much_eat = atoi(av[5]);

// 		if(!is_valid_input(*input))
// 			return 0;
// 		printf("1: %d\n", number_of_philos);
// 		printf("2: %d\n", time_to_die);
// 		printf("3: %d\n", time_to_eat);
// 		printf("4: %d\n", time_to_sleep);
// 		printf("5: %d\n", number_of_how_much_eat);
// 	}
// 	else
// 	{
// 		printf("enter correct quintity of args\n");
// 		return 1;
// 	}
// 	return 0;
// }
int main(int ac, char **av)
{
	int philo_data[5]; // To store all the parsed values
	
	if (ac == 5 || ac == 6)
	{
		if (!parse_arguments(ac, av, philo_data))
			return 1;
		
		printf("Number of philosophers: %d\n", philo_data[0]);
		printf("Time to die: %d ms\n", philo_data[1]);
		printf("Time to eat: %d ms\n", philo_data[2]);
		printf("Time to sleep: %d ms\n", philo_data[3]);
		if (philo_data[4] != -1)
			printf("Number of times each philosopher must eat: %d\n", philo_data[4]);
		else
			printf("Philosophers will eat until simulation stops\n");
		
		// Continue with the rest of your program here...
	}
	else 
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: %s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", av[0]);
		return 1;
	}
	
	return 0;
}

