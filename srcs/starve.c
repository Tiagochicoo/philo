/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/23 21:11:49 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	is_dead(t_philo *philo)
{
	if (philo->info->philo_died)
		check_death_meals(philo->info);
	if (elapsed_time(philo) >= philo->info->time_to_die)
	{
		philo->info->philo_died = 1;
		print_msg("has died", philo, RED);
		check_death_meals(philo->info);
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
		//nap((t_philo*)info->philos[0], info->time_to_die);
		//die(info);
		printf("%s[%d] died\n%s", RED, info->philos[0].id, RESET);
		//pthread_detach(info->philos[0].thread);
		//exit(1);
	}
	// else
	// 	while (1)
	// 	{
	// 		if (info->philo_died || info->finish == info->must_eat)
	// 			break ;
	// 	}
	//printf("die\n");
}