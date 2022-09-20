/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:30:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 21:48:34 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	error(char *msg)
{
	printf("%s%s\n%s", RED, msg, RESET);
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
	now -= philo->eat_timestamp;
	return (now);
}

long	elapsed_time(t_philo *philo)
{
	long	now;
	long	start;

	now = get_timestamp();
	start = (philo->info->start_time.tv_sec * 1000)
		+ (philo->info->start_time.tv_usec / 1000);
	return (now - start);
}

int	print_msg(char *msg, t_philo *philo, char *color)
{
	long	timestamp;

	timestamp = elapsed_time(philo);
	printf("%s%ld %d %s\n%s", color, timestamp, philo->id, msg, RESET);
	return (timestamp);
}
