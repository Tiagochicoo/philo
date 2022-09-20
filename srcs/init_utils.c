/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 21:24:04 by tpereira          #+#    #+#             */
/*   Updated: 2022/09/20 21:45:32 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	kill_all(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		info->philos[i]->is_dead = 1;
		i++;
	}
}

int	is_philo_dead(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num && !info->philo_died)
	{
		pthread_mutex_lock(&info->death_lock);
		if (info->finish >= info->num)
		{
			kill_all(info);
			info->philos[i]->is_dead = 1;
			pthread_mutex_unlock(&info->death_lock);
			return (1);
		}
		if (since_last_meal(info->philos[i]) > info->time_to_die)
		{
			kill_all(info);
			info->philos[i]->is_dead = 1;
			print_msg("died", info->philos[i], RED);
			pthread_mutex_unlock(&info->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&info->death_lock);
		i++;
	}
	return (0);
}

void	checker(t_info *info)
{
	if (info->num == 1)
	{
		pthread_mutex_lock(&info->forks[0]);
		print_msg("has taken the left fork", info->philos[0], BLUE);
		usleep(info->time_to_die * 1000);
		print_msg("died", info->philos[0], RED);
		pthread_mutex_lock(&info->death_lock);
		info->philo_died = 1;
		pthread_mutex_unlock(&info->death_lock);
		pthread_mutex_unlock(&info->forks[0]);
	}
	else
	{
		while (1)
			if (is_philo_dead(info))
				break ;
	}
	stop_meal(info);
}

int	ft_atoi(const char *str)
{
	int			sign;
	long long	num;

	sign = 1;
	num = 0;
	while (*str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num *= 10;
		num += (sign * (*(str++) - '0'));
		if (num > 2147483647)
			return (-1);
		if (num < -2147483648)
			return (0);
	}
	return (num);
}

int	ft_isnumber(char *str)
{
	int		isnumber;

	isnumber = 0;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			isnumber = 1;
		else
			return (0);
		str++;
	}	
	return (isnumber);
}
