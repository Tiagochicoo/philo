/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/14 20:08:27 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	has_starved(t_philo *philo)
{
	if (since_last_meal(philo) > philo->info->time_to_die)
	{
		print_msg("died", philo, RED);
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->philo_died = 1;
		pthread_mutex_unlock(&philo->info->death_lock);
		return (1);
	}
	return (0);
}
