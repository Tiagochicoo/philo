/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:54:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/24 16:46:45 by tpereira         ###   ########.fr       */
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
