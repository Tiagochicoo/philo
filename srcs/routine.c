/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/07 17:14:41 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// void	free_all(void *ptr)
// {
// 	int i;

// 	i = 0;
// 	while(ptr[i])
// 		free(ptr[i++]);
// }

void	stop_meal(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		pthread_detach(info->philos[i]->thread);
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
			break ;
		i++;
	}
	pthread_mutex_unlock(&info->print_lock);
	pthread_mutex_unlock(&info->death_lock);
	//pthread_mutex_destroy(&info->print_lock);
	//pthread_mutex_destroy(&info->death_lock);
	free(info->forks);
	free(info->philos);
	//free(info);
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
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	}
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
	}
	else
	{
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->print_lock);
	if (philo->id % 2 == 0)
		print_msg("is eating", philo, PURPLE);
	else
		print_msg("is eating", philo, PURPLE);
	pthread_mutex_unlock(&philo->info->print_lock);
	philo->meals++;
	philo->eat_timestamp = get_timestamp();
	usleep(philo->info->time_to_eat * 1000);
	drop_forks(philo);
}

void	get_forks(t_philo *philo)
{
	if (!philo->is_dead)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
			pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
			pthread_mutex_lock(&philo->info->print_lock);
			print_msg("has taken a fork", philo, BLUE);
			print_msg("has taken a fork", philo, BLUE);
		}
		else
		{
			pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
			pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
			pthread_mutex_lock(&philo->info->print_lock);
			print_msg("has taken a fork", philo, BLUE);
			print_msg("has taken a fork", philo, BLUE);
		}
		pthread_mutex_unlock(&philo->info->print_lock);
	}
}

void	start_routine(t_philo *philo)
{
	if (philo->info->philo_died < 1)
	{
		if (philo->info->must_eat && philo->meals != philo->info->must_eat)
		{
			get_forks(philo);
			eat(philo);
			nap(philo, philo->info->time_to_sleep);
			think(philo);
			start_routine(philo);
		}
	}
}

void	*routine(t_philo *philo)
{
	if (philo->info->num != 1 && !philo->is_dead)
		start_routine(philo);
	else
		check_death_meals(philo);
	return (NULL);
}