/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:18:52 by mzhivoto          #+#    #+#             */
/*   Updated: 2025/08/05 21:36:53 by mzhivoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define CLEANUP_PRINT 1
#define CLEANUP_DEATH 2
#define CLEANUP_FORKS 3
#define CLEANUP_PHILOS 4
#define USAGE "./philo philo_number time_to_die \
	time_to_eat time_to_sleep [number_of_meals]"

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	lock;
}					t_fork;

typedef struct s_philo
{
	int				id;
	int				meals_ctn;
	long long		last_meal_time;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				num_full;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				is_dead;
	long long		start_time;
	t_fork			*forks;
	t_philo			*philos;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	death_lock;
	pthread_t		watcher;
}					t_data;

// parser
int					parse_arguments(int ac, char **av, t_data *data);

// print
void				print_err_msg(char *msg);
void	print_status(t_philo *philo, const char *msg);
//void				print_status(t_philo *philo, const char *msg, long long timestamp);
void				print_death_status(t_philo *philo, const char *msg);

// init
void				cleanup_resources(t_data *data, int level);
int					init_data(t_data *data);

// utils
bool				is_alive(t_data *data);
long long			get_time_ms(void);
void				ft_usleep(int ms);
void				ft_dreaming(t_data *data, int ms);
int					ft_atoi(const char *str);

// philo utils
void				meal_update(t_philo *philo);
int					forks_for_even(t_philo *philo, int *locked_left, int *locked_right);
int					forks_for_odd(t_philo *philo, int *locked_left, int *locked_right);
void				release_locked_forks(t_philo *philo, int locked_left, int locked_right);
int					meals_eaten(t_philo *philo);

// watcher
void				*run_watcher(void *arg);

// philo
void				*philo_routine(void *arg);

//cleanup
void				cleanup_resources(t_data *data, int level);
void				cleanup_simulation(t_data *data);
void				join_and_cleanup(t_data *data);
