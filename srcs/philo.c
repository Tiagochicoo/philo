/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 16:19:22 by tpereira          #+#    #+#             */
/*   Updated: 2022/08/22 22:20:43 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	error(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

//void	check_dead(t_philo *philo)
//{
//	int time_diff;
//
//	time_diff = philo->eat_timestamp;
//	if (philo->eat_timestamp)
//}

int	get_timestamp(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
}

int	elapsed_time(t_philo *philo)
{
	int				now;
	int				start;

	now = get_timestamp();
	start = (philo->info->start_time.tv_sec * 1000) + (philo->info->start_time.tv_usec / 1000);
	// printf("start -> %d\n", start);
	// printf("now -> %d\n", now);
	return (now - start);
}

int	print_msg(char *msg, t_philo *philo, char *color)
{
	int	timestamp;

	timestamp = elapsed_time(philo);
	// insert death_lock mutex here??
	printf("%s%d [%d] -> %s\n%s", color, timestamp, philo->id, msg, RESET);
	return (timestamp);
}

void	think(t_philo *philo)
{
	if (philo->meals != philo->info->must_eat)
		print_msg("is thinking", philo, YELLOW);
}

void	nap(t_philo *philo, int	sleep_time)
{
	pthread_mutex_lock(&philo->info->print_lock);
	print_msg("is sleeping", philo, GREEN);
	usleep(sleep_time * 1000);
}

void	eat(t_philo *philo)
{
	int timestamp;
	
	//add drop_forks()


	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->info->print_lock);
		timestamp = print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
	}
	else
	{
		pthread_mutex_lock(&philo->info->print_lock);
		timestamp = print_msg("is eating", philo, PURPLE);
		philo->meals++;
		usleep(philo->info->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->id % philo->info->num]);
	}
	philo->eat_timestamp = timestamp;
}

void	is_dead(t_philo *philo)
{
	if (philo->info->philo_died)
		check_death_meals(philo->info);
	if (elapsed_time(philo) >= philo->info->time_to_die)
	{
		philo->info->philo_died = 1;
		print_msg("has died", philo, RED);
		check_death_meals(philo->info);
	}
}

void	get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0 && philo->info->philo_died == 0)
	{
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the left fork", philo, BLUE);
		print_msg("has taken the right fork", philo, BLUE);
	}
	else if (philo->id % 2 != 0 )
	{
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id % philo->info->num]);
		is_dead(philo);
		pthread_mutex_lock(&philo->info->forks[philo->id - 1]);
		pthread_mutex_lock(&philo->info->print_lock);
		print_msg("has taken the right fork", philo, BLUE);
		print_msg("has taken the left fork", philo, BLUE);
	}
}

void	start_routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	while(philo->info->must_eat && philo->meals != philo->info->must_eat)
	{
		get_forks(philo);
		eat(philo);
		nap(philo, philo->info->time_to_sleep);
		think(philo);
		pthread_mutex_unlock(&philo->info->print_lock);
		if (!philo->info->philo_died)
			routine(philo);
	}
}

void	*routine(t_philo *philo)
{
	// !! avoid if statement beacuse of data-races !!
	if (!philo->info->philo_died)
		start_routine(philo);
	return (NULL);
}

void	create_philo(t_info *info, int	i)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (error("Failed to malloc philo!\n"));
	philo->thread = (pthread_t)malloc(sizeof(pthread_t));
	if (!philo->thread)
		return (error("Failed to malloc thread!\n"));
	philo->info = info;
	philo->id = i + 1;
	philo->eat_timestamp = 0;
	philo->left_fork = philo->info->forks[i];
	philo->right_fork = philo->info->forks[i + 1 ];
	philo->meals = 0;
	philo->info->philos[i].thread = philo->thread;
	if (pthread_create(&info->philos[i].thread, NULL, (void *)&routine, philo) != 0)
		printf("Error creating philo %d!!\n", i + 1);
}

void	check_death_meals(t_info *info)
{
	if (info->num == 1)
	{
		//nap((t_philo*)info->philos[0], info->time_to_die);
		//die(info);
		printf("%s[%d] died\n%s", RED, info->philos[0].id, RESET);
		//pthread_detach(info->philos[0].thread);
		//exit(1);
	}
	// else
	// 	while (1)
	// 	{
	// 		if (info->philo_died || info->finish == info->must_eat)
	// 			break ;
	// 	}
	//printf("die\n");
}

void	create_philos(t_info *info)
{
	int i;
	
	i = 0;
	while (i < info->num)
	{
		create_philo(info, i);
		i++;
	}
	check_death_meals(info);
	i = 0;
	while (i < info->num)
	{
		pthread_join(info->philos[i].thread, NULL);
		pthread_mutex_lock(&info->print_lock);
		printf("Joined thread %d!!\n", ++i);
		pthread_mutex_unlock(&info->print_lock);
	}
}

void	create_forks(t_info *info)
{
	int	i;

	i = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->num);
	if (!info->forks)
		return (error("Failed to malloc forks!\n"));
	while (i < info->num)
	{
		if (!pthread_mutex_init(&info->forks[i], NULL))
			i++;
		else
			error("Error!! Failed to create fork!\n");
	}
}

int	check_args(char **argv)
{
	int	i;
	int	num;

	i = 1;
	while (argv[i])
	{
		num = atoi(argv[i]);
		if (num < 1)
			return (0);
		else
			i++;
	}
	return (1);
}

void	set_params(t_info *info, char **argv)
{
	if (check_args(argv))
	{
		info->num = atoi(argv[1]);
		info->time_to_die = atoi(argv[2]);
		info->time_to_eat = atoi(argv[3]);
		info->time_to_sleep = atoi(argv[4]);
		info->finish = 0;
		if (argv[5])
			info->must_eat = atoi(argv[5]);
		else
			info->must_eat = -1;
		info->philos = malloc(sizeof(t_philo) * info->num);
		gettimeofday(&info->start_time, NULL);
	}
	else
		error("Error! Invalid arguments\n");
}

int main(int argc, char **argv)
{
	t_info				info;

	if (argc > 4)
	{
		set_params(&info, argv);
		create_forks(&info);
		create_philos(&info);
		//routine();
	}
	else
		printf("Usage: \"./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\"\n");
	return (0);
}
