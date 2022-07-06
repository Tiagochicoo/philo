/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/06 21:32:30 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

typedef struct philo
{
	struct timeval	start;
	int				left_fork;
	int				right_fork;
} 				philo;

typedef struct s_data
{
	int					i;
	int					num;
	int					forks;
	double				time_to_eat;
	pthread_t			*philos;
	pthread_mutex_t		*mutex;
}			t_data;

void	*eat(t_data *data)
{
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	pthread_mutex_lock(data->mutex);
	usleep(50000);
	gettimeofday(&end, NULL);
	printf("%.0f %d is eating\n", (double)(end.tv_sec * 1000 + end.tv_usec / 1000) - (double)(start.tv_sec * 1000 + start.tv_usec / 1000), data->i);
	pthread_mutex_unlock(data->mutex);
	return (NULL);
}

int main(int argc, char **argv)
{
	int		 			i;
	t_data				data;

	if (argc > 4)
	{
		i = 0;
		data.num = atoi(argv[1]);
		data.i = i;
		data.forks = data.num;
		data.philos = malloc(sizeof(pthread_t) * data.num);
		pthread_mutex_init(data.mutex, NULL);
		while (i < data.num)
		{
			data.i = i;
			pthread_create(&data.philos[i], NULL, (void *)&eat, &data);
			i++;
		}
		i = 0;
		while (i < data.num)
		{
			data.i = i;
			pthread_join(data.philos[i], NULL);
			//gettimeofday(&data.end, NULL);
			//printf("%.0f %d finished eating\n", (double)(data.end.tv_sec * 1000 + data.end.tv_usec / 1000) - (double)(data.start.tv_sec * 1000 + data.start.tv_usec / 1000), i);
			i++;
		}
		//pthread_exit(0);
		pthread_mutex_destroy(data.mutex);
		free(data.philos);
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}