/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 19:24:23 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/28 18:59:19 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define RESET "\x1b[0m"

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				id;
	int				meals;
	int				is_dead;
	long			eat_timestamp;
	struct s_info	*info;
}				t_philo;

typedef struct s_info
{
	int					num;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	int					finish;
	int					philo_died;
	struct timeval		start_time;
	struct s_philo		**philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		death_lock;
}				t_info;

void	create_thread(t_info *info, int i);
void	create_philos(t_info *info);
void	create_forks(t_info *info);
int		check_args(char **argv);
void	set_params(t_info *info, char **argv);
void	is_dead(t_philo *philo);
void	starve(t_info *info);
void	check_death_meals(t_philo *philo);
void	error(char *msg);
long	get_timestamp(void);
long	elapsed_time(t_info *info);
int		since_last_meal(t_philo *philo);
int		print_msg(char *msg, t_philo *philo, char *color);
void	stop_meal(t_info *info);
void	nap(t_philo *philo, int sleep_time);
void	drop_forks(t_philo *philo);
void	eat(t_philo *philo);
void	get_forks(t_philo *philo);
void	*routine(t_philo *philo);
void	check_death(t_info *info);

#endif