/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:30:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/23 21:01:01 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

int	get_timestamp(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
}

int	elapsed_time(t_philo *philo)
{
	int				now;
	int				start;

	now = get_timestamp();
	start = (philo->info->start_time.tv_sec * 1000) + (philo->info->start_time.tv_usec / 1000);
	return (now - start);
}

void	print_msg(char *msg, t_philo *philo, char *color)
{
	int	timestamp;

	pthread_mutex_lock(&philo->info->print_lock);
	timestamp = elapsed_time(philo);
	// insert death_lock mutex here??
	printf("%s%d [%d] -> %s\n%s", color, timestamp, philo->id, msg, RESET);
	pthread_mutex_unlock(&philo->info->print_lock);
}
