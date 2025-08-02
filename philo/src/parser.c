/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:21:12 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/02 19:14:15 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

static int	check_overflow(char *str)
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

static int	is_valid_input(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			print_err_msg("Non-digit character found in input");
			return (0);
		}
		i++;
	}
	if (!check_overflow(str))
	{
		print_err_msg("Number too large, would cause integer overflow");
		return (0);
	}
	return (1);
}

static int	input_check(int ac, t_data *data)
{
	if (data->num_philos <= 0)
	{
		print_err_msg("Need at least one philosopher");
		return (0);
	}
	if (data->time_to_die <= 0 || data->time_to_eat <= 0)
	{
		print_err_msg("Time values must be positive");
		return (0);
	}
	if (ac == 6 && data->must_eat <= 0)
	{
		print_err_msg("Number of meals must be positive");
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
			printf("Invalid argument: %s\n", av[i]);
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
