/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/21 17:01:23 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_info				info;

	if (argc > 6)
		printf("%sError! Too many arguments\n%s", RED, RESET);
	else if (argc > 4)
	{
		set_params(&info, argv);
		create_forks(&info);
		create_philos(&info);
	}
	else
	{
		printf("Usage: \"./philo number_of_philosophers time_to_die");
		printf("time_to_eat time_to_sleep");
		printf("[number_of_times_each_philosopher_must_eat]\"\n");
	}
	return (0);
}
