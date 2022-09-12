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


//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


// void	free_all(void *ptr)
// {
// 	int i;

// 	i = 0;
// 	while(ptr[i])
// 		free(ptr[i++]);
// }

void	think(t_philo *philo)
{
	//pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->info->philo_died)
	{
		//pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->meals != philo->info->must_eat)
			print_msg("is thinking", philo, YELLOW);
	}
	// else
	// 	pthread_mutex_unlock(&philo->info->death_lock);
}

void	nap(t_philo *philo, int	sleep_time)
{
	//pthread_mutex_lock(&philo->info->death_lock);
	// if (!philo->info->philo_died)
	// {
		//pthread_mutex_unlock(&philo->info->death_lock);
		//pthread_mutex_lock(&philo->info->print_lock);
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	// }
	// else
	// 	pthread_mutex_unlock(&philo->info->death_lock);
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
	//printf("philo %d eat_timestamp = %ld at -> %p\n", philo->id, philo->eat_timestamp % 1000, &philo->eat_timestamp);
	print_msg("is eating", philo, PURPLE);
	philo->meals++;
	//pthread_mutex_lock(&philo->info->death_lock);
	philo->eat_timestamp = get_timestamp();
	//pthread_mutex_unlock(&philo->info->death_lock);
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

void	*start_routine(t_philo *philo)
{
	get_forks(philo);
	eat(philo);
	nap(philo, philo->info->time_to_sleep);
	think(philo);
	if (philo->info->must_eat && philo->meals != philo->info->must_eat)
		start_routine(philo);
	pthread_mutex_lock(&philo->info->death_lock);
	philo->info->finish++; 
	pthread_mutex_unlock(&philo->info->death_lock);
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
	printf("philo [%d] routine\n", philo->id);
	return (NULL);
}