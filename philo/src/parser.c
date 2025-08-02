/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:21:12 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/02 18:16:33 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_overflow(char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i])
	{
		if (result > (INT_MAX - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

int	is_valid_input(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
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

int	input_check(int ac, t_data *data)
{
	if (data->num_philos <= 0)
	{
		printf("Error: Need at least one philosopher\n");
		return (0);
	}
	if (data->time_to_die <= 0 || data->time_to_eat <= 0)
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

int	parse_arguments(int ac, char **av, t_data *data)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_valid_input(av[i]))
		{
			printf("Error: Invalid argument: %s\n", av[i]);
			return (0);
		}
		i++;
	}
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = -1;
	if (!input_check(ac, data))
		return (0);
	return (1);
}
