/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:21:12 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/27 16:33:58 by mzhivoto         ###   ########.fr       */
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

