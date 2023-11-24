/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:05:56 by bpleutin          #+#    #+#             */
/*   Updated: 2023/11/09 16:15:13 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	test_sem(t_data *d, sem_t *sem, int flag)
{
	if (sem == SEM_FAILED)
		free_all(d, 1);
	else if (flag != 0)
		sem_post(sem);
}

void	free_all(t_data *data, int flag)
{
	if (data->info.write)
		sem_close(data->info.write);
	if (data->info.start)
		sem_close(data->info.start);
	if (data->info.end)
		sem_close(data->info.end);
	if (data->info.die)
		sem_close(data->info.die);
	if (data->info.is_done)
		sem_close(data->info.is_done);
	if (data->info.is_dead)
		sem_close(data->info.is_dead);
	if (data->info.last)
		sem_close(data->info.last);
	if (data->info.s_state)
		sem_close(data->info.s_state);
	if (data->info.meal)
		sem_close(data->info.meal);
	if (data->info.forks)
		sem_close(data->info.forks);
	free(data->philo);
	if (flag == 1)
		exit(EXIT_FAILURE);
}
