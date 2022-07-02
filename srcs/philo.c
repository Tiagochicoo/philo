/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/02 18:02:04 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

typedef struct s_data
{
	int				num;
	int				i;
	pthread_t		*philo;
	pthread_mutex_t	*lock;

}			t_data;



void	*eat(t_data *data)
{
	if (!data->lock)
		printf("Philosopher %d is waiting!\n", data->i);
	pthread_mutex_lock(data->lock);
	printf("Philosopher %d is eating spaghetti!\n", data->i);
	pthread_mutex_unlock(data->lock);
	return (NULL);
}

int main(int argc, char **argv)
{
	int		 	i;
	t_data		data;

	if (argv[1])
	{
		i = 0;
		data.num = atoi(argv[1]);
		data.philo = malloc(sizeof(pthread_t) * data.num);
	}
	if (argc > 4)
	{
		while (i <= data.num)
		{
			data.i = i;
			pthread_create(&data.philo[i], NULL, (void *)&eat, &data);
			i++;
		}
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}