/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/25 20:03:37 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	is_dead(t_philo *philo)
{
	int	last_meal;

	last_meal = since_last_meal(philo);
	printf("%d [%d] last_meal\n", last_meal, philo->id);
	if (philo->info->philo_died)
		check_death_meals(philo->info);
	if (last_meal >= philo->info->time_to_die)
	{
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->philo_died = 1;
		pthread_mutex_unlock(&philo->info->death_lock);
		check_death_meals(philo->info);
		
	}
}

void	starve(t_info *info)
{
	pthread_mutex_lock(&info->death_lock);
	info->philo_died = 1;
	print_msg("ignore for now --> has died", info->philos[0], RED);
	usleep(30000);
	pthread_mutex_unlock(&info->death_lock);
}

void	check_death_meals(t_info *info)
{
	if (info->num == 1)
	{
		pthread_mutex_lock(&info->forks[0]);
		print_msg("has taken the left fork", info->philos[0], BLUE);
		usleep(info->time_to_die * 1000);
		pthread_mutex_unlock(&info->forks[0]);
		print_msg("has died", info->philos[0], RED);
	}
	else if (info->philo_died)
	{
		print_msg("has died", info->philos[0], RED);
	}
	stop_meal(info);
}
