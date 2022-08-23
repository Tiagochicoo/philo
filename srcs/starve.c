/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/23 22:01:53 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	is_dead(t_philo *philo)
{
	if (philo->info->philo_died)
		check_death_meals(philo->info);
	if (since_last_meal(philo) >= philo->info->time_to_die)
	{
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->philo_died = 1;
		pthread_mutex_unlock(&philo->info->death_lock);
		print_msg("has died", philo, RED);
		stop_meal(philo->info);
	}
}

void	starve(t_info *info)
{
	info->philo_died = 1;
	print_msg("ignore for now ---------------> has died", &info->philos[0], RED);
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