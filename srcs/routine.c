/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/27 19:54:10 by tpereira         ###   ########.fr       */
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
	pthread_mutex_destroy(&info->print_lock);
	pthread_mutex_destroy(&info->death_lock);
	//free(info->forks);
	//free(info->philos);
	//free(info);
	exit (1);
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
	//add drop_forks()
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	else
	{
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	philo->eat_timestamp = get_timestamp();
}

void	get_forks(t_philo *philo)
{
	is_dead(philo);
	if (philo->id % 2 == 0)
	{
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the left fork", philo, BLUE);
		print_msg("has taken the right fork", philo, BLUE);
	}
	else
	{
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the right fork", philo, BLUE);
		print_msg("has taken the left fork", philo, BLUE);
	}
	is_dead(philo);
}

void	start_routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	if (philo->info->must_eat && philo->meals != philo->info->must_eat)
	{
		is_dead(philo);
		get_forks(philo);
		is_dead(philo);
		eat(philo);
		is_dead(philo);
		nap(philo, philo->info->time_to_sleep);
		is_dead(philo);
		think(philo);
		//pthread_mutex_unlock(&philo->info->print_lock);
		is_dead(philo);
		routine(philo);
	}
}

void	*routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	if (!philo->info->philo_died)
	{
		if (philo->info->num != 1)
			start_routine(philo);
		else
			check_death_meals(philo);
	}
	return (NULL);
}