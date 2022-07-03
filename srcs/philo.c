/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/03 21:20:58 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

typedef struct s_data
{
	int				i;
	int				num;
	int				forks;
	pthread_t		*philos;
	pthread_mutex_t	*lock;

}			t_data;

void	*eat(t_data *data)
{
	//printf("Philosopher %d is waiting!\n", data->i);
	pthread_mutex_lock(data->lock);
	printf("Philosopher %d is eating spaghetti!\n", data->i);
	printf("Philosopher %d put down forks!\n", data->i);
	pthread_mutex_unlock(data->lock);
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
		pthread_mutex_init(data.lock, NULL);
		while (i < data.num)
		{
			data.i = i;
			pthread_create(&data.philos[i], NULL, (void *)&eat, &data);
			pthread_detach(data.philos[i]);
			i++;
		}
		// while (i < data.num)
		// {
		// 	pthread_join(data.philos[i], NULL);
		// 	i++;
		// }
		pthread_exit(0);
		pthread_mutex_destroy(data.lock);
		free(data.philos);
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	
	return (0);
}