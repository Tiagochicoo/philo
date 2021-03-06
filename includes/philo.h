/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 19:24:23 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/20 10:09:34 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	int				id;
	int 			meals;
	int				eat_timestamp;
}				t_philo;

typedef struct s_info
{
	int					i;
	int					num;
	int					*forks;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	pthread_t			*threads;
    t_philo             *philos;
	pthread_mutex_t		*mutex;
}				t_info;

#endif