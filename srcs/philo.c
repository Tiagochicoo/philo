/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/05 22:10:27 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

typedef struct s_data
{
	int					i;
	int					num;
	int					forks;
	pthread_t			*philos;
	pthread_mutex_t		*mutex;
	struct timeval		start;
	struct timeval		end;
}			t_data;

typedef struct s_philo
{
	clock_t	start_eating;
	clock_t	finish_eating;
	clock_t	start_sleeping;
	clock_t	finish_sleeping;
	clock_t	start_thinking;
	clock_t	finish_thinking;
	int		meals;
	pthread_t	thread;
	t_data		*data;

} 				t_philo;

void	*eat(t_data *data)
{
	//printf("eat(): data->i = %d\n", data->i);
	pthread_mutex_lock(data->mutex);
	printf("Philosopher %d is eating spaghetti!\n", data->i);
	//printf("Philosopher %d put down forks!\n", data->i);
	pthread_mutex_unlock(data->mutex);
	return (NULL);
}

int main(int argc, char **argv)
{
	int				i;
	t_data			data;
	
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
			printf("pthread_create -> %d\n", data.i);
			pthread_create(&data.philos[i], NULL, (void *)&eat, &data);
			i++;
		}
		i = 0;
		while (i < data.num)
		{
			data.i = i;
			pthread_join(data.philos[i], NULL);
			//printf("joined thread -> %d\n", data.i);
			gettimeofday(&data.start, NULL);
			printf("%.0f %d finished eating\n", (double)(data.start.tv_sec * 0.0001), i);
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