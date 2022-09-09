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
	//pthread_mutex_unlock(&info->print_lock);
	//pthread_mutex_unlock(&info->death_lock);
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
	pthread_mutex_lock(&philo->info->print_lock);
	print_msg("is eating", philo, PURPLE);
	pthread_mutex_unlock(&philo->info->print_lock);
	philo->meals++;
	pthread_mutex_lock(&philo->info->death_lock);
	philo->eat_timestamp = get_timestamp();
	pthread_mutex_unlock(&philo->info->death_lock);
	usleep(philo->info->time_to_eat * 1000);
	drop_forks(philo);
}

void	get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		// printf("\nphilo %d\n", philo->id);
		// printf("left fork %d -> %p\n", philo->id - 1, philo->left_fork);
		// printf("right fork %d -> %p\n", philo->id % philo->info->num, philo->right_fork);
		// //-------
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
	}
	else
	{
		// printf("\nphilo %d\n", philo->id);
		// printf("right fork %d -> %p\n", philo->id % philo->info->num, philo->right_fork);
		// printf("left fork %d -> %p\n", philo->id - 1, philo->left_fork);
		// //-----
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		print_msg("has taken a fork", philo, BLUE);
		print_msg("has taken a fork", philo, BLUE);
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
	{
		start_routine(philo);
	}
	else
		check_death_meals(philo);
	return (NULL);
}