/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:54:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/14 19:09:24 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	printf("finished Joining threads\n");
}

void	detach_thread(pthread_t thread)
{
	pthread_detach(thread);
	printf("Detached thread!!\n");
}