/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/08 18:44:58 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	if (philo->info->num == 1)
	{
		pthread_mutex_lock(&philo->info->forks[0]);
		print_msg("has taken a fork", philo, BLUE);
		usleep(philo->info->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		print_msg("died", philo, RED);
	}
	else if (philo->info->philo_died)
	{
		printf("philo->info->philo_died [%d]\n", philo->info->philo_died);
		//printf("time to die usleep(%ld)\n", elapsed_time(philo) - philo->info->time_to_die);
		//usleep(elapsed_time(philo->info->philos[philo->info->philo_died - 1]) - philo->info->time_to_die);
		print_msg("died", philo, RED);
	}
	stop_meal(philo->info);
}
