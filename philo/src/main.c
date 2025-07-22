/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:14:53 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/22 16:02:29 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
 #include <stdlib.h>

 

//  int is_valid_input(int ac, char **av)
//  {
// 	int i = 0;
// 	while (av[i])
// 	{
// 		if(av[i]);

// 	}
//  }

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