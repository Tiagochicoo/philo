/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/14 19:27:22 by tpereira         ###   ########.fr       */
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
		stop_meal(philo->info);
		return (1);
	}
	return (0);
}
void	is_dead(t_philo *philo)
{
	int last_meal;

	if (philo->info->philo_died)
		check_death_meals(philo);
	last_meal = since_last_meal(philo);
	//printf("last_meal -> %ld\n", philo->eat_timestamp);
	if (last_meal >= philo->info->time_to_die)
	{
		pthread_mutex_lock(&philo->info->death_lock);
		philo->is_dead = 1;
		philo->info->philo_died = philo->id;
		pthread_mutex_unlock(&philo->info->death_lock);
		check_death_meals(philo);
	}
}

void	starve(t_info *info)
{
	pthread_mutex_lock(&info->death_lock);
	info->philo_died = 1;
	print_msg("ignore for now --> has died", info->philos[0], RED);
	usleep(3000);
	pthread_mutex_unlock(&info->death_lock);
}

void	check_death_meals(t_philo *philo)
{
	printf("philo->info->philo_died [%d]\n", philo->info->philo_died);
	print_msg("died", philo, RED);
	stop_meal(philo->info);
}
