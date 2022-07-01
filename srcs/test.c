/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 19:18:13 by tpereira          #+#    #+#             */
/*   Updated: 2022/07/01 18:28:33 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	mails = 0;

void* routine(void)
{
	for (int i = 0; i < 10000; i++)
		mails++;
	return (NULL);
}

int main(void)
{
	pthread_t   t1;
	pthread_t   t2;

	if (pthread_create(&t1, NULL, (void*)routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, (void*)routine, NULL) != 0)
		return (2);
	if (pthread_join(t1, NULL) != 0)
		return (3);
	if (pthread_join(t2, NULL) != 0)
		return (4);
	printf("Number of mails: %d\n", mails);
	return (0);
}