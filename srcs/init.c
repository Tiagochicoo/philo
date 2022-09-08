/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:25:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/08 23:00:39 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	create_thread(t_info *info, int	i)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (error("Failed to malloc philo!\n"));
	philo->info = info;
	philo->id = i + 1;
	philo->is_dead = 0;
	//printf("philo %d eat_timestamp = %ld at -> %p\n", philo->id, philo->eat_timestamp % 1000, &philo->eat_timestamp);
	pthread_mutex_lock(&philo->info->time_lock);
	philo->eat_timestamp = get_timestamp();
	printf("philo %d eat_timestamp = %ld at -> %p\n", philo->id, philo->eat_timestamp % 1000, &philo->eat_timestamp);
	pthread_mutex_unlock(&philo->info->time_lock);
	philo->left_fork = &philo->info->forks[i];
	philo->right_fork = &philo->info->forks[philo->id % info->num];
	philo->meals = 0;
	philo->info->philos[i] = philo;
	if (pthread_create(&philo->thread, NULL, (void *)&routine, philo) != 0)
		printf("Error creating philo %d!!\n", i + 1);
}

void	checker(t_info *info)
{
	int		i;
	long	time;

	time = 0;
	while (1)
	{
		i = info->num - 1;
		while (i)
		{
			time = since_last_meal(info->philos[i]);
			if (time > info->time_to_die)
			{
				pthread_mutex_lock(&info->death_lock);
				printf("\n\n\n -------------- DATA RACES ----------------\n\n\n");
				info->philos[i]->is_dead = 1;
				info->philo_died = i;
				pthread_mutex_unlock(&info->death_lock);
				print_msg("died", info->philos[i], RED);
				stop_meal(info);
				break ;
			}
			i--;
		}
		if (info->philo_died > 0)
			break ;
	}
}

void	create_philos(t_info *info)
{
	int i;
	
	i = 0;
	while (i < info->num)
	{
		create_thread(info, i);
		i++;
	}
	//pthread_mutex_lock(&info->time_lock);
	checker(info);
	//pthread_mutex_unlock(&info->time_lock);
	i = 0;
	while (i < info->num)
	{
		pthread_join(info->philos[i]->thread, NULL);
		pthread_mutex_lock(&info->print_lock);
		printf("Joined thread %d!!\n", ++i);
		pthread_mutex_unlock(&info->print_lock);
	}
}

void	create_forks(t_info *info)
{
	int	i;

	i = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->num);
	if (!info->forks)
		return (error("Failed to malloc forks!\n"));
	while (i < info->num)
	{
		if (!pthread_mutex_init(&info->forks[i], NULL))
			i++;
		else
			error("Error!! Failed to create fork!\n");
	}
	pthread_mutex_init(&info->print_lock, NULL);
	pthread_mutex_init(&info->time_lock, NULL);
}

//need better parsing here -> check "1a" arguments
int	check_args(char **argv)
{
	int	i;
	int	num;

	i = 1;
	while (argv[i])
	{
		num = atoi(argv[i]);
		if (num < 1)
			return (0);
		else
			i++;
	}
	return (1);
}

void	set_params(t_info *info, char **argv)
{
	if (check_args(argv))
	{
		info->num = atoi(argv[1]);
		info->time_to_die = atoi(argv[2]);
		info->time_to_eat = atoi(argv[3]);
		info->time_to_sleep = atoi(argv[4]);
		info->finish = 0;
		info->philo_died = 0;
		if (argv[5])
			info->must_eat = atoi(argv[5]);
		else
			info->must_eat = -1;
		info->philos = malloc(sizeof(t_philo) * info->num);
		gettimeofday(&info->start_time, NULL);
	}
	else
		error("Error! Invalid arguments\n");
}