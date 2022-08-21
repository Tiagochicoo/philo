/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/21 11:48:03 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

// void	go_to_sleep(t_info *info)
// {
// 	printf("philo %d is sleeping\n", info->i);
// 	usleep(info->time_to_sleep);
// }

// void	release_forks(t_info *info)
// {
// 	int n;

// 	n = 0;
// 	if (info->i != info->num)
// 		n = info->i + 1;
// 	info->forks[info->i] = 0;
// 	info->forks[n] = 0;
// }

// void	*eat(t_info *info)
// {
// 	struct timeval	start;
// 	struct timeval	end;
// 	double			milliseconds;

// 	gettimeofday(&start, NULL);
// 	pthread_forks_lock(info->forks);
// 	gettimeofday(&end, NULL);
// 	milliseconds = (double)(end.tv_sec * 1000 + end.tv_usec / 1000) - (double)(start.tv_sec * 1000 + start.tv_usec / 1000);
// 	printf("%.0f %d is eating\n", milliseconds, info->i);
// 	info->i++;
// 	usleep(info->time_to_eat);
// 	release_forks(info);
// 	pthread_forks_unlock(info->forks);
// 	go_to_sleep(info);
// 	return (NULL);
// }

void	get_forks(t_philo *philo)
{
	static int n = 0;

	if (n % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork);
		pthread_mutex_lock(&philo->right_fork);
		printf("par -> Philo[%d] -> picked forks \n", n + 1);
		usleep(100000);
		pthread_mutex_unlock(&philo->left_fork);
		pthread_mutex_unlock(&philo->right_fork);
		n++;
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork);
		pthread_mutex_lock(&philo->right_fork);
		printf("impar -> Philo[%d] -> picked forks \n", n + 1);
		usleep(100000);
		pthread_mutex_unlock(&philo->left_fork);
		pthread_mutex_unlock(&philo->right_fork);
		n++;
	}
}

void	*routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->print_lock);
	printf("Created philo [%d]\n", philo->id);
	pthread_mutex_unlock(&philo->info->print_lock);
	//get_forks(philo);
	//eat(info);
	//think(info);
	return (NULL);
}

void	create_philo(t_info *info, int	i)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (error("Failed to malloc philo!\n"));
	philo->thread = malloc(sizeof(pthread_t));
	if (!philo->thread)
		return (error("Failed to malloc thread!\n"));
	philo->info = info;
	philo->id = i + 1;
	philo->eat_timestamp = (struct timeval){0};
	philo->left_fork = philo->info->forks[i];
	philo->right_fork = philo->info->forks[i + 1 ];
	philo->meals = 0;
	philo->info->philos[i].thread = philo->thread;
	gettimeofday(&philo->eat_timestamp, NULL);
	if (pthread_create(&info->philos[i].thread, NULL, (void *)&routine, philo) != 0)
		printf("Error creating philo %d!!\n", i + 1);
}

void	create_philos(t_info *info)
{
	int i;
	
	i = 0;
	while (i < info->num)
	{
		create_philo(info, i);
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		pthread_mutex_lock(&info->print_lock);
		pthread_join(info->philos[i].thread, NULL);
		printf("Joined philo %d!!\n", ++i);
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
}

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

int main(int argc, char **argv)
{
	t_info				info;

	if (argc > 4)
	{
		set_params(&info, argv);
		create_forks(&info);
		create_philos(&info);
		//routine();
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}