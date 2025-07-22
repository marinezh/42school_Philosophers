/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/22 18:41:53 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
 #include <stdlib.h>

 
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
	while (str[i])
	{
		if(!ft_isdigit(str[i]))
		{
			printf("error found\n");
			return 0;
		}
	}
	return 1;
 }

int main(int ac, char **av)
{
	(void)ac;
	
	//int i = 0;
	int number_of_philos = 0;
	int time_to_die = 0;
	int time_to_eat = 0;
	int time_to_sleep = 0;
	int number_of_how_much_eat = 0;

	number_of_philos = atoi(av[1]);
	time_to_die = atoi(av[2]);
	time_to_eat = atoi(av[3]);
	time_to_sleep = atoi(av[4]);
	number_of_how_much_eat = atoi(av[5]);
	is_valid_input(*av);
	printf("1: %d\n", number_of_philos);
	printf("2: %d\n", time_to_die);
	printf("3: %d\n", time_to_eat);
	printf("4: %d\n", time_to_sleep);
	printf("5: %d\n", number_of_how_much_eat);

	// printf("print ac %d\n", ac);
	// while(av[i])
	// {
	// 	printf("print av :%s\n", av[i]);
	// 	i++;
	// }
	return 0;
}