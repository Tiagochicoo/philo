/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:25:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 18:46:22 by tpereira         ###   ########.fr       */
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
	philo->eat_timestamp = get_timestamp();
	philo->left_fork = &philo->info->forks[i];
	philo->right_fork = &philo->info->forks[philo->id % info->num];
	philo->meals = 0;
	philo->info->philos[i] = philo;
	if (pthread_create(&philo->thread, NULL, (void *)&routine, philo))
		printf("Error creating philo %d!!\n", i + 1);
	else
	{
		pthread_mutex_lock(&philo->info->death_lock);
		philo->info->finish++;
		pthread_mutex_unlock(&philo->info->death_lock);
	}
	printf("%d finish\n", philo->id);
}

void	kill_all(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		info->philos[i]->is_dead = 1;
		i++;
	}
}

int	is_philo_dead(t_info *info)
{
	int i;

	i = 0;
	while (i < info->num && !info->philo_died)
	{
		pthread_mutex_lock(&info->death_lock);
		if (since_last_meal(info->philos[i]) > info->time_to_die)
		{
			kill_all(info);
			info->philos[i]->is_dead = 1;
			print_msg("has died", info->philos[i], RED);
			pthread_mutex_unlock(&info->death_lock);
			return (1);
		}
		if (info->finish == info->num)
		{
			info->philo_died = 1;
			pthread_mutex_unlock(&info->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&info->death_lock);
		i++;
	}
	return (0);
}

void	checker(t_info *info)
{
	if (info->num == 1)
	{
		pthread_mutex_lock(&info->forks[0]);
		print_msg("has taken the left fork", info->philos[0], BLUE);
		usleep(info->time_to_die * 1000);
		print_msg("has died", info->philos[0], RED);
		pthread_mutex_lock(&info->death_lock);
		info->philo_died = 1;
		pthread_mutex_unlock(&info->death_lock);
		pthread_mutex_unlock(&info->forks[0]);
	}
	else
	{
		while (1)
			if (is_philo_dead(info))
				break ;
	}
	stop_meal(info);
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
	checker(info);
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
		if (pthread_mutex_init(&info->forks[i], NULL))
		{
			error("Error!! Failed to create fork!\n");
			break ;
		}
		i++;
	}
	pthread_mutex_init(&info->death_lock, NULL);
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
		info->philos = malloc(sizeof(t_philo *) * info->num);
		gettimeofday(&info->start_time, NULL);
	}
	else
		error("Error! Invalid arguments\n");
}