/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/14 19:18:36 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->info->philo_died)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->meals != philo->info->must_eat)
			print_msg("is thinking", philo, YELLOW);
	}
	else
	 	pthread_mutex_unlock(&philo->info->death_lock);
}

void	nap(t_philo *philo, int	sleep_time)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->info->philo_died)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->info->philo_died)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		philo->eat_timestamp = get_timestamp();
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	*get_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->info->philo_died)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
		}
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
	return (NULL);
}

void	*start_routine(t_philo *philo)
{
	if (philo->info->must_eat && philo->meals != philo->info->must_eat)
	{
		get_forks(philo);
		eat(philo);
		nap(philo, philo->info->time_to_sleep);
		think(philo);
		start_routine(philo);
	}
	else
	{
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->finish++;
		pthread_mutex_unlock(&philo->info->death_lock);
	}
	return (NULL);
}

void	*routine(t_philo *philo)
{
	if (philo->info->num != 1)
		start_routine(philo);
	else
	{
		pthread_mutex_lock(&philo->info->forks[0]);
		print_msg("has taken a fork", philo, BLUE);
		usleep(philo->info->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		print_msg("died", philo, RED);
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->philo_died = 1;
		pthread_mutex_unlock(&philo->info->death_lock);
	}
	return (NULL);
}