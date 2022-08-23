/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/23 20:56:15 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	is_dead(t_philo *philo)
{
	if (elapsed_time(philo) >= philo->info->time_to_die)
	{
		philo->info->philo_died = 1;
		print_msg("has died", philo, RED);
		usleep(100);
		routine(philo);
	}
}

void	starve(t_info *info)
{
	info->philo_died = 1;
	print_msg("has died", &info->philos[0], RED);
	usleep(30000);
}

void	check_death_meals(t_info *info)
{
	if (info->num == 1)
	{
		usleep(info->time_to_die * 1000);
		starve(info);
		stop_meal(info);
	}
	else
		while (1)
		{
			if (info->philo_died || info->finish == info->must_eat)
				break ;
		}
	printf("die\n");
}