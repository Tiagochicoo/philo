/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/20 10:11:23 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	go_to_sleep(t_info *info)
{
	printf("philo %d is sleeping\n", info->i);
	usleep(info->time_to_sleep);
}

void	release_forks(t_info *info)
{
	int n;

	n = 0;
	if (info->i != info->num)
		n = info->i + 1;
	info->forks[info->i] = 0;
	info->forks[n] = 0;
}

int	  get_forks(t_info *info)
{
	int n;

	n = 0;
	printf("\n\nget forks()\n");
	printf("info->i: %d\n", info->i);
	// printf("info->num: %d\n", info->num);
	if (info->i != info->num && info->num > 2)
		n = info->i + 1;
	else if (!info->forks[info->i] && !info->forks[n])
	{
		info->forks[info->i] = 1;
		info->forks[n] = 1;
		printf("philo %d grabbed left fork\n", info->i);
		printf("philo %d grabbed right fork\n", info->i);
		return (1);
	}
	else
		printf("%d couldn't get forks...\n", info->i);
	// 	get_forks(info);
	return (0);
}

void	*eat(t_info *info)
{
	struct timeval	start;
	struct timeval	end;
	double			milliseconds;

	gettimeofday(&start, NULL);
	pthread_mutex_lock(info->mutex);
	gettimeofday(&end, NULL);
	milliseconds = (double)(end.tv_sec * 1000 + end.tv_usec / 1000) - (double)(start.tv_sec * 1000 + start.tv_usec / 1000);
	printf("%.0f %d is eating\n", milliseconds, info->i);
	info->i++;
	usleep(info->time_to_eat);
	release_forks(info);
	pthread_mutex_unlock(info->mutex);
	go_to_sleep(info);
	return (NULL);
}

void	*routine(t_info *info)
{
	printf("\n\nroutine()\n");
	printf("info->i: %d\n", info->i);
	// printf("info->num: %d\n", info->num);
	if (get_forks(info))
		eat(info);
	else if (info->i > 1)
		routine(info);
	return (NULL);
}

void	create_philo(t_info *info)
{
	t_philo *philosopher;

	printf("\n\ncreate_philo()\n");
	printf("info->i = %d\n", info->i);
	philosopher = malloc(sizeof(t_philo));
	philosopher->thread = malloc(sizeof(pthread_t));
	philosopher->id = info->i;
	philosopher->left_fork = info->i;
	philosopher->right_fork = info->i + 1;
	philosopher->meals = 0;
	philosopher->eat_timestamp = 0;
	info->threads[info->i] = philosopher->thread;
	if (!pthread_create(&philosopher->thread, NULL, (void *)&routine, &info))
		printf("Error creating thread!!\n");
}

void	create_threads(t_info *info)
{
	int i;
	
	i = 0;
	while (i < info->num)
	{
		info->i = i;
		printf("\n\ncreate_threads()\n");
		printf("info->i = %d\n", info->i);
		create_philo(info);
		i++;
	}
	i = 0;
	while (i < info->num)
	{
		pthread_join(info->threads[i], NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
	int		 			i;
	t_info				info;

	if (argc > 4)
	{
		i = 0;
		info.i = i;
		info.num = atoi(argv[1]);
		info.time_to_die = atoi(argv[2]) * 1000;
		info.time_to_eat = atoi(argv[3]) * 1000;
		info.time_to_sleep = atoi(argv[4]) * 1000;
		info.forks = malloc(sizeof(int) * info.num);
		info.philos = malloc(sizeof(pthread_t) * info.num);
		pthread_mutex_init(info.mutex, NULL);

		create_threads(&info);

		pthread_mutex_destroy(info.mutex);
		free(info.philos);
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}