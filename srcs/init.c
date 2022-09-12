/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:25:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/12 23:30:07 by tpereira         ###   ########.fr       */
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
	//pthread_mutex_lock(&philo->info->time_lock);
	philo->eat_timestamp = get_timestamp();
	//printf("philo %d eat_timestamp = %ld at -> %p\n", philo->id, philo->eat_timestamp, &philo->eat_timestamp);
	//pthread_mutex_unlock(&philo->info->time_lock);
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
			pthread_mutex_lock(&info->death_lock);
			time = since_last_meal(info->philos[i]);
			if (time > info->time_to_die)
			{
				info->philos[i]->is_dead = 1;
				info->philo_died = i;
				print_msg("died", info->philos[i], RED);
				pthread_mutex_unlock(&info->death_lock);
				stop_meal(info);
				break ;
			}
			pthread_mutex_unlock(&info->death_lock);
			i--;
		}
		if (info->philo_died > 0)
			break ;
	}
}

void	check_death(t_info *info)
{
	int num;

	num = 0;
	if (info->must_eat > 0)
		num = info->num * info->must_eat;
	while (1)
	{
		pthread_mutex_lock(&info->death_lock);
		if (info->philo_died || info->finish == num)
		{
			pthread_mutex_unlock(&info->death_lock);
			join_threads(info);
			break ;
		}
		pthread_mutex_unlock(&info->death_lock);
		pthread_mutex_lock(&info->death_lock);
		if (num > 0 && info->finish == num)
			break ;
		pthread_mutex_unlock(&info->death_lock);
	}
	pthread_mutex_unlock(&info->death_lock);
}

void	create_philos(t_info *info)
{
	int i;
	
	i = 0;
	while (i < info->num)
		create_thread(info, i++);
	check_death(info);
	//checker(info);
	join_threads(info);
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
	pthread_mutex_init(&info->death_lock, NULL);
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