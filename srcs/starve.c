/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:27:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/24 16:48:25 by tpereira         ###   ########.fr       */
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
	print_msg("ignore for now --> has died", &info->philos[0], RED);
	usleep(30000);
}

void	check_death_meals(t_info *info)
{
	if (info->num == 1)
		printf("%s[%d] died\n%s", RED, info->philos[0].id, RESET);
}
