/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:25:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 21:47:12 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	create_thread(t_info *info, int i)
{
	t_philo	*philo;

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
}

void	create_philos(t_info *info)
{
	int	i;

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
		if (!ft_isnumber(argv[i]))
			return (0);
		num = ft_atoi(argv[i]);
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
		info->num = ft_atoi(argv[1]);
		info->time_to_die = ft_atoi(argv[2]);
		info->time_to_eat = ft_atoi(argv[3]);
		info->time_to_sleep = ft_atoi(argv[4]);
		info->finish = 0;
		info->philo_died = 0;
		if (argv[5])
			info->must_eat = ft_atoi(argv[5]);
		else
			info->must_eat = -1;
		info->philos = malloc(sizeof(t_philo *) * info->num);
		gettimeofday(&info->start_time, NULL);
	}
	else
		error("Error! Invalid arguments");
}
