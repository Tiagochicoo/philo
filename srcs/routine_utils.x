/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 18:13:33 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 18:15:59 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
