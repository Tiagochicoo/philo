/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/12 18:05:16 by tpereira         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->info->time_lock);
	philo->eat_timestamp = get_timestamp();
	pthread_mutex_unlock(&philo->info->time_lock);
	usleep(philo->info->time_to_eat * 1000);
	drop_forks(philo);
}

void	get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
		printf("Philo %d left  fork %d at %p\n", philo->id, philo->id - 1, philo->left_fork);
		printf("Philo %d right fork %d at %p\n", philo->id, philo->id, philo->right_fork);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
		printf("Philo %d right fork %d at %p\n", philo->id, philo->id, philo->right_fork);
		printf("Philo %d left  fork %d at %p\n", philo->id, philo->id - 1, philo->left_fork);
	}
}

void	start_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);
	if (philo->info->must_eat && philo->meals != philo->info->must_eat)
	{
		get_forks(philo);
		eat(philo);
		nap(philo, philo->info->time_to_sleep);
		think(philo);
		start_routine(philo);
	}
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
		stop_meal(philo->info);
	}
	return (NULL);
}