/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/23 19:34:59 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int	stop_meal(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		if (pthread_join(info->philos[i].thread, NULL))
			return (0);
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
			return (0);
		i++;
	}
	pthread_mutex_destroy(&info->print_lock);
	pthread_mutex_destroy(&info->death_lock);
	free(info->forks);
	free(info->philos);
	free(info);
	return (1);
}

void	think(t_philo *philo)
{
	if (philo->meals != philo->info->must_eat)
		print_msg("is thinking", philo, YELLOW);
}

void	nap(t_philo *philo, int	sleep_time)
{
	pthread_mutex_lock(&philo->info->print_lock);
	print_msg("is sleeping", philo, GREEN);
	usleep(sleep_time * 1000);
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
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
	}
	else if (philo->id != 1)
	{
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
	}
	philo->eat_timestamp = get_timestamp();
}

void	get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		check_death_meals(philo->info);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the left fork", philo, BLUE);
		check_death_meals(philo->info);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		print_msg("has taken the right fork", philo, BLUE);
	}
	else if (philo->id != 1)
	{
		check_death_meals(philo->info);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the right fork", philo, BLUE);
		check_death_meals(philo->info);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		print_msg("has taken the left fork", philo, BLUE);
	}
	else
	{
		print_msg("has taken the left fork", philo, BLUE);
		check_death_meals(philo->info);
	}

}

void	*routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	while(philo->info->must_eat && philo->meals != philo->info->must_eat)
	{
		if (!philo->info->philo_died)
		{
			get_forks(philo);
			eat(philo);
			nap(philo, philo->info->time_to_sleep);
			think(philo);
			pthread_mutex_unlock(&philo->info->print_lock);
			if (!philo->info->philo_died)
				routine(philo);
		}
		else
			break ;
	}
	return (NULL)
}