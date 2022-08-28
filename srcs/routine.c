/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:32:00 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/28 18:56:21 by tpereira         ###   ########.fr       */
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

	printf("stop meal\n");
	i = info->num - 1;
	while (i)
	{
		if (pthread_join(info->philos[i]->thread, NULL))
			break ;
		i--;
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
	philo->eat_timestamp = get_timestamp();
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
	if (philo->info->philo_died == 0)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
			pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
			pthread_mutex_lock(&philo->info->print_lock);
			print_msg("has taken the left fork", philo, BLUE);
			print_msg("has taken the right fork", philo, BLUE);
		}
		else
		{
			pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
			pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
			pthread_mutex_lock(&philo->info->print_lock);
			print_msg("has taken the right fork", philo, BLUE);
			print_msg("has taken the left fork", philo, BLUE);
		}
	}
}

void	start_routine(t_philo *philo)
{
	if (philo->info->philo_died < 1)
	{
	// !! avoid if statement beacuse of data-races !!
		if (philo->info->must_eat && philo->meals != philo->info->must_eat)
		{
			if (philo->info->philo_died == 0)
				get_forks(philo);
			if (philo->info->philo_died == 0)	
				eat(philo);
			if (philo->info->philo_died == 0)	
				nap(philo, philo->info->time_to_sleep);
			if (philo->info->philo_died == 0)	
				think(philo);
			if (philo->info->philo_died == 0)	
				routine(philo);
		}
	}
}

void	*routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	if (philo->info->num != 1 && philo->info->philo_died == 0)
		start_routine(philo);
	else
		check_death_meals(philo);
	return (NULL);
}