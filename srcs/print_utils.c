/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:30:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/31 16:12:52 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

long	get_timestamp(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
}

int	since_last_meal(t_philo *philo)
{
	long	now;

	now = get_timestamp();
	return (now - philo->eat_timestamp);
}

long	elapsed_time(t_philo *philo)
{
	long	now;
	long	start;

	now = get_timestamp();
	start = (philo->info->start_time.tv_sec * 1000) + (philo->info->start_time.tv_usec / 1000);
	return (now - start);
}

int	print_msg(char *msg, t_philo *philo, char *color)
{
	printf("%s%ld %d %s\n%s", color, elapsed_time(philo), philo->id, msg, RESET);
	return (elapsed_time(philo));
}
