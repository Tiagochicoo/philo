/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/20 11:55:10 by tpereira         ###   ########.fr       */
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

// int	  get_forks(t_info *info)
// {
// 	int n;

// 	n = 0;
// 	printf("\n\nget forks()\n");
// 	printf("info->i: %d\n", info->i);
// 	// printf("info->num: %d\n", info->num);
// 	if (info->i != info->num && info->num > 2)
// 		n = info->i + 1;
// 	else if (!info->forks[info->i] && !info->forks[n])
// 	{
// 		info->forks[info->i] = 1;
// 		info->forks[n] = 1;
// 		printf("philo %d grabbed left fork\n", info->i);
// 		printf("philo %d grabbed right fork\n", info->i);
// 		return (1);
// 	}
// 	else
// 		printf("%d couldn't get forks...\n", info->i);
// 	// 	get_forks(info);
// 	return (0);
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

void	*routine(void)
{
	printf("\n\nroutine()\n");
	// // printf("info->num: %d\n", info->num);
	// if (get_forks(info))
	// 	eat(info);
	// else if (info->i > 1)
	// 	routine(info);
	return (NULL);
}

void	create_philo(t_info *info, int	num)
{
	t_philo *philo;

	printf("\n\ncreate_philo()\n");
	philo = malloc(sizeof(t_philo));
	philo->thread = (pthread_t)malloc(sizeof(pthread_t));
	philo->id = num;
	philo->left_fork = info->forks[num];
	philo->right_fork = info->forks[num + 1];
	philo->meals = 0;
	philo->eat_timestamp = 0;
	info->philos[num].thread = philo->thread;
	if (!pthread_create(&philo->thread, NULL, (void *)&routine, NULL))
		printf("Error creating thread!!\n");
}

void	create_philos(t_info *info)
{
	int i;
	
	i = 1;
	while (i <= info->num)
	{
		printf("\n\ncreate_threads()\n");
		create_philo(info, i);
		i++;
	}
	i = 1;
	while (i <= info->num)
	{
		pthread_join(info->philos[i].thread, NULL);
		i++;
	}
}

void	create_forks(t_info *info)
{
	int	i;

	i = 0;
	while(i < info->num)
	{
		if (pthread_mutex_init(&info->forks[i], NULL) != -1)
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
		{
			printf("Error! Invalid arguments\n");
			return (0);
		}
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
	}
	else
		exit(EXIT_FAILURE);
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