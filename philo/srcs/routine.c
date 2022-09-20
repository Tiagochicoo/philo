/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 21:33:16 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->meals != philo->info->must_eat)
			print_msg("is thinking", philo, YELLOW);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	nap(t_philo *philo, int sleep_time)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		pthread_mutex_lock(&philo->info->death_lock);
		philo->eat_timestamp = get_timestamp();
		pthread_mutex_unlock(&philo->info->death_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	get_forks(t_philo *philo)
{
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
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		pthread_mutex_lock(&philo->info->death_lock);
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
		pthread_mutex_unlock(&philo->info->death_lock);
	}
	else
	{
		drop_forks(philo);
		pthread_mutex_unlock(&philo->info->death_lock);
	}
}

void	*routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (philo->info->num != 1 && !philo->info->philo_died)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->id % 2 == 0)
			usleep(900);
		start_routine(philo);
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
	return (NULL);
}
