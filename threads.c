/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:50:07 by bpleutin          #+#    #+#             */
/*   Updated: 2023/10/26 14:03:16 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	nostop(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->die);
	pthread_mutex_lock(&philo->meal);
	if (!philo->info->dead && (philo->info->max_meals == -1
			|| philo->meals < philo->info->max_meals))
		return (pthread_mutex_unlock(&philo->info->die),
			pthread_mutex_unlock(&philo->meal),
			1);
	pthread_mutex_lock(&philo->info->end);
	if (!philo->info->dead && !(philo->info->max_meals == -1
			&& philo->meals < philo->info->max_meals))
		philo->info->finished += 1;
	pthread_mutex_unlock(&philo->info->end);
	return (pthread_mutex_unlock(&philo->info->die),
		pthread_mutex_unlock(&philo->meal),
		0);
}

void	*timer(void *ptr)
{
	t_data	*d;
	int		i;

	d = (t_data *)ptr;
	pthread_mutex_lock(&d->info.start);
	pthread_mutex_unlock(&d->info.start);
	while (!pthread_mutex_lock(&d->info.end)
		&& d->info.finished < d->info.gang_len)
	{
		pthread_mutex_unlock(&d->info.end);
		i = -1;
		while (++i < d->info.gang_len && !pthread_mutex_lock(&d->philo[i].last))
		{
			if (nostop(&d->philo[i]) && get_time()
				- d->philo[i].last_meal > d->info.time_to_die + 1)
				return (pthread_mutex_lock(&d->philo[i].m_state),
					d->philo[i].state = DEAD,
					pthread_mutex_unlock(&d->philo[i].m_state),
					protected_print(&d->philo[i]),
					pthread_mutex_unlock(&d->philo[i].last), (void *)0);
			pthread_mutex_unlock(&d->philo[i].last);
		}
	}
	pthread_mutex_unlock(&d->info.end);
	return ((void *)0);
}

void	uber_fork(t_philo *philo, pthread_mutex_t *a, pthread_mutex_t *b, int f)
{
	if (f && (!pthread_mutex_unlock(a) && (a != b && !pthread_mutex_unlock(b))))
		return ;
	pthread_mutex_lock(a);
	if (!nostop(philo) && !pthread_mutex_unlock(a))
		return ;
	pthread_mutex_lock(&philo->info->write);
	printf("%llu philo %d has taken a fork\n",
		get_time() - philo->info->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->info->write);
	if (a != b)
		pthread_mutex_lock(b);
	if ((a == b && !pthread_mutex_unlock(a)) || (!nostop(philo)
			&& !pthread_mutex_unlock(a) && !pthread_mutex_unlock(b)))
		return ;
	pthread_mutex_lock(&philo->info->write);
	printf("%llu philo %d has taken a fork\n",
		get_time() - philo->info->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->info->write);
}

void	uber_eat(t_philo *philo)
{
	if (philo->id % 2 == 1)
		uber_fork(philo, philo->left, philo->right, 0);
	else
		uber_fork(philo, philo->right, philo->left, 0);
	if (philo->left == philo->right || !nostop(philo))
		return ;
	pthread_mutex_lock(&philo->m_state);
	philo->state = EATING;
	pthread_mutex_unlock(&philo->m_state);
	if (!nostop(philo) && (!pthread_mutex_unlock(philo->left) && (philo->right
				!= philo->left && !pthread_mutex_unlock(philo->right))))
		return ;
	pthread_mutex_lock(&philo->last);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->last);
	protected_print(philo);
	usleep(philo->info->time_to_eat * 1000);
	if (philo->id % 2 == 1)
		uber_fork(philo, philo->left, philo->right, 1);
	else
		uber_fork(philo, philo->right, philo->left, 1);
	pthread_mutex_lock(&philo->meal);
	philo->meals++;
	pthread_mutex_unlock(&philo->meal);
}

void	*set_philo(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&philo->info->start);
	pthread_mutex_unlock(&philo->info->start);
	while (nostop(philo))
	{
		uber_eat(philo);
		if (philo->left == philo->right || !nostop(philo))
			break ;
		pthread_mutex_lock(&philo->m_state);
		philo->state = SLEEPING;
		pthread_mutex_unlock(&philo->m_state);
		protected_print(philo);
		usleep(philo->info->time_to_sleep * 1000);
		if (!nostop(philo))
			break ;
		pthread_mutex_lock(&philo->m_state);
		philo->state = THINKING;
		pthread_mutex_unlock(&philo->m_state);
		protected_print(philo);
	}
	return ((void *)0);
}
