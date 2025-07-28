/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:18:52 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/07/28 17:39:31 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
# include <sys/time.h>

# define USAGE "./philo philo_number time_to_die time_to_eat time_to\
_sleep [number_of_meals]"

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	lock;
}	t_fork;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long long		start_time;
	t_fork			*forks;
	pthread_mutex_t	print_lock;
	t_philo			*philos;
	int				simulation_running;
}					t_data;

int	ft_isdigit(int c);
int	parse_arguments(int ac, char **av, t_data *data);
int	is_valid_input(char *str);
long long	get_time_ms(void);


int	init_data(t_data *data);


//DELETE

void print_philo_data(t_data *data);
int init_forks(t_data *data);