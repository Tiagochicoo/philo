/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/04 22:20:28 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// typedef struct philo
// {
// 	clock_t	start_eating;
// 	clock_t	finish_eating;
// 	int		left_fork;
// 	int		right_fork;
// } 				philo;

void	*eat(t_data *data)
{
	printf("data->i = %d\n", data->i);
	//printf("Philosopher %d is waiting!\n", data->i);
	pthread_mutex_lock(data->mutex);
	printf("Philosopher %d is eating spaghetti!\n", data->i);
	//printf("Philosopher %d put down forks!\n", data->i);
	pthread_mutex_unlock(data->mutex);
	return (NULL);
}

int main(int argc, char **argv)
{
	int		 	i;
	t_data		data;

	if (argc > 4)
	{
		i = 0;
		data.num = atoi(argv[1]);
		data.forks = data.num;
		data.philos = malloc(sizeof(pthread_t) * data.num);
		pthread_mutex_init(data.mutex, NULL);
		while (i < data.num)
		{
			printf("1 - i = %d\n", i);
			data.i = i;
			printf("2 - data.i = %d\n", data.i);
			pthread_create(&data.philos[i], NULL, (void *)&eat, &data);
			printf("3 - i = %d\n", i);
			i++;
		}
		i = 0;
		while (i < data.num)
		{
			pthread_join(data.philos[i], NULL);
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