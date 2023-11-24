/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:43:05 by bpleutin          #+#    #+#             */
/*   Updated: 2023/10/24 11:15:03 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_alltime	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	protected_print(t_philo *philo)
{
	pthread_mutex_lock(&philo->m_state);
	pthread_mutex_lock(&philo->info->write);
	if (philo->state == EATING)
		printf("%llu philo %d is eating\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == SLEEPING)
		printf("%llu philo %d is sleeping\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == THINKING)
		printf("%llu philo %d is thinking\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == DEAD && !philo->info->dead)
	{
		pthread_mutex_lock(&philo->info->die);
		philo->info->dead += 1;
		pthread_mutex_unlock(&philo->info->die);
		printf("%llu philo %d is dead\n",
			get_time() - philo->info->start_time, philo->id + 1);
	}
	pthread_mutex_unlock(&philo->m_state);
	pthread_mutex_unlock(&philo->info->write);
}
