/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 21:30:09 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 21:32:29 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	start_routine(t_philo *philo)
{
	while (philo->meals != philo->info->must_eat)
	{
		get_forks(philo);
		eat(philo);
		nap(philo, philo->info->time_to_sleep);
		think(philo);
		pthread_mutex_lock(&philo->info->death_lock);
		if (philo->is_dead)
		{
			pthread_mutex_unlock(&philo->info->death_lock);
			break ;
		}
		else
			pthread_mutex_unlock(&philo->info->death_lock);
		routine(philo);
	}
	pthread_mutex_lock(&philo->info->death_lock);
	philo->info->finish++;
	pthread_mutex_unlock(&philo->info->death_lock);
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	free_all(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
		free(info->philos[i++]);
	free(info->philos);
	free(info->forks);
}

void	stop_meal(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		pthread_join(info->philos[i]->thread, NULL);
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
			break ;
		i++;
	}
	pthread_mutex_destroy(&info->death_lock);
	free_all(info);
	exit (1);
}
