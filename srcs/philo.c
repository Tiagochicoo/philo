/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/07 16:30:14 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

typedef struct s_philo
{
	struct timeval	start;
	int				left_fork;
	int				right_fork;
} 				t_philo;

typedef struct s_info
{
	int					i;
	int					num;
	int					forks;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	pthread_t			*philos;
	pthread_mutex_t		*mutex;
}			t_info;

void	*eat(t_info *info)
{
	struct timeval	start;
	struct timeval	end;
	double			milliseconds;

	get_forks(t_info *info);
	gettimeofday(&start, NULL);
	pthread_mutex_lock(info->mutex);
	usleep(info->time_to_eat);
	gettimeofday(&end, NULL);
	milliseconds = (double)(end.tv_sec * 1000 + end.tv_usec / 1000) - (double)(start.tv_sec * 1000 + start.tv_usec / 1000);
	printf("%.0f %d is eating\n", milliseconds, info->i);
	pthread_mutex_unlock(info->mutex);
	return (NULL);
}

int main(int argc, char **argv)
{
	int		 			i;
	t_info				info;

	if (argc > 4)
	{
		i = 0;
		info.num = atoi(argv[1]);
		info.time_to_die = atoi(argv[2]) * 1000;
		info.time_to_eat = atoi(argv[3]) * 1000;
		info.time_to_sleep = atoi(argv[4]) * 1000;
		info.i = i;
		info.forks = info.num;
		info.philos = malloc(sizeof(pthread_t) * info.num);
		pthread_mutex_init(info.mutex, NULL);
		while (i < info.num)
		{
			info.i = i;
			pthread_create(&info.philos[i], NULL, (void *)&eat, &info);
			i++;
		}
		i = 0;
		while (i < info.num)
		{
			info.i = i;
			pthread_join(info.philos[i], NULL);
			i++;
		}
		pthread_mutex_destroy(info.mutex);
		free(info.philos);
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}