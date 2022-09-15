/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/15 17:32:02 by tpereira         ###   ########.fr       */
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
		if (&info->philos[i]->thread != NULL)
		{
			printf("attempting to join thread %d\n", i);
			pthread_join(info->philos[i]->thread, NULL);
			printf("joined thread %d\n", i);
		}
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
			break ;
		i++;
	}
	//pthread_mutex_unlock(&info->death_lock);
	pthread_mutex_destroy(&info->print_lock);
	pthread_mutex_destroy(&info->death_lock);
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
	pthread_mutex_lock(&philo->info->print_lock);
	if (!philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->print_lock);
		print_msg("is sleeping", philo, GREEN);
		usleep(sleep_time * 1000);
	}
	else
		pthread_mutex_unlock(&philo->info->print_lock);
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
	//add drop_forks()
	pthread_mutex_lock(&philo->info->death_lock);
	philo->eat_timestamp = get_timestamp();
	pthread_mutex_unlock(&philo->info->death_lock);
	if (philo->id % 2 == 0)
	{
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	else
	{
		print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		drop_forks(philo);
	}
	pthread_mutex_lock(&philo->info->death_lock);
	philo->eat_timestamp = get_timestamp();
	pthread_mutex_unlock(&philo->info->death_lock);
}

void	get_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (!philo->is_dead)
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
}

void	start_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->death_lock);
	if (philo->info->philo_died < 1)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		if (philo->info->must_eat && philo->meals != philo->info->must_eat)
		{
			get_forks(philo);
			eat(philo);
			nap(philo, philo->info->time_to_sleep);
			think(philo);
			routine(philo);
		}
	}
	else
		pthread_mutex_unlock(&philo->info->death_lock);
}

void	*routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);
	pthread_mutex_lock(&philo->info->death_lock);
	if (philo->info->num != 1 && !philo->is_dead)
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		start_routine(philo);
	}
	else
	{
		pthread_mutex_unlock(&philo->info->death_lock);
		check_death_meals(philo);
	}
	return (NULL);
}