/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 19:24:23 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/20 13:34:39 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
	int				id;
	int 			meals;
	struct timeval	eat_timestamp;
}				t_philo;

typedef struct s_info
{
	int					num;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	struct timeval		start_time;
    t_philo             *philos;
	pthread_mutex_t		*forks;
}				t_info;

#endif