/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/19 15:54:03 by tpereira         ###   ########.fr       */
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
		if (pthread_join(info->philos[i]->thread, NULL))
			break ;
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
	printf("exit\n");
	exit (1);
}

void	think(t_philo *philo)
{
	if (!philo->is_dead)
	{
		if (philo->meals != philo->info->must_eat)
			print_msg("is thinking", philo, YELLOW);
	}
}

void	nap(t_philo *philo, int	sleep_time)
{
	if (!philo->is_dead)
	{
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	}
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

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	philo->eat_timestamp = get_timestamp();
	pthread_mutex_unlock(&philo->info->death_lock);
	print_msg("is eating", philo, PURPLE);
	philo->meals++;
	usleep(philo->info->time_to_eat * 1000);
	drop_forks(philo);
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
	print_msg("has taken a fork", philo, BLUE);
	print_msg("has taken a fork", philo, BLUE);
}

void	start_routine(t_philo *philo)
{
	while (philo->meals != philo->info->must_eat)
	{
		get_forks(philo);
		eat(philo);
		nap(philo, philo->info->time_to_sleep);
		think(philo);
		routine(philo);
	}
	pthread_mutex_lock(&philo->info->death_lock);
	philo->info->finish++;
	pthread_mutex_unlock(&philo->info->death_lock);
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