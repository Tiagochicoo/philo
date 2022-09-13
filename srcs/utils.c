/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:54:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/12 23:28:20 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	stop_meal(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
		pthread_detach(info->philos[i++]->thread);
	i = 0;
	while (i < info->num)
	{
		if (pthread_mutex_destroy(&info->forks[i]))
			break ;
        i++;
	}
	//pthread_mutex_unlock(&info->print_lock);
	//pthread_mutex_unlock(&info->death_lock);
	//pthread_mutex_destroy(&info->print_lock);
	//pthread_mutex_destroy(&info->death_lock);
	free(info->forks);
	//free(info->philos);
	//free(info);
	exit (1);
}

void	join_threads(t_info *info)
{
	int i;

	i = 0;
	while (i < info->num)
	{
		if (info->philos[i]->thread)
			pthread_join(info->philos[i]->thread, NULL);
		i++;
	}
}

void	detach_threads(t_info *info)
{
	int i;

	i = 0;
	while (i < info->num)
	{
		pthread_detach(info->philos[i]->thread);
		printf("Detached thread %d!!\n", ++i);
	}
}