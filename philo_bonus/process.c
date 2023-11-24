/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:50:07 by bpleutin          #+#    #+#             */
/*   Updated: 2023/11/09 16:24:19 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	nostop(t_philo *philo)
{
	sem_wait(philo->info->end);
	sem_wait(philo->info->is_dead);
	sem_wait(philo->info->meal);
	if (!philo->is_dead && !philo->info->finished
		&& (philo->info->max_meals == -1
			|| philo->meals < philo->info->max_meals))
		return (sem_post(philo->info->end),
			sem_post(philo->info->is_dead),
			sem_post(philo->info->meal), 1);
	if (!philo->is_dead && !philo->info->finished
		&& !(philo->info->max_meals == -1
			&& philo->meals < philo->info->max_meals))
		philo->info->finished += 1;
	return (sem_post(philo->info->end),
		sem_post(philo->info->is_dead),
		sem_post(philo->info->meal), 0);
}

void	uber_fork(t_philo *philo, sem_t *s, int f)
{
	if (f && (!sem_post(s) && (philo->info->gang_len > 1 && !sem_post(s))))
		return ;
	sem_wait(s);
	if (!nostop(philo) && !sem_post(s))
		return ;
	sem_wait(philo->info->write);
	if (nostop(philo))
		printf("%llu philo %d has taken a fork\n",
			get_time() - philo->info->start_time, philo->id + 1);
	sem_post(philo->info->write);
	if (philo->info->gang_len > 1)
		sem_wait(s);
	if ((philo->info->gang_len == 1 && !sem_post(s)) || (!nostop(philo)
			&& !sem_post(s) && !sem_post(s)))
		return ;
	sem_wait(philo->info->write);
	if (nostop(philo))
		printf("%llu philo %d has taken a fork\n",
			get_time() - philo->info->start_time, philo->id + 1);
	sem_post(philo->info->write);
}

void	uber_eat(t_philo *philo)
{
	uber_fork(philo, philo->info->forks, 0);
	if (philo->info->gang_len == 1 || (!nostop(philo)
			&& (!sem_post(philo->info->forks)
				&& (philo->info->gang_len > 1
					&& !sem_post(philo->info->forks)))))
		return ;
	sem_wait(philo->info->last);
	philo->last_meal = get_time();
	sem_post(philo->info->last);
	protected_print(philo, EATING);
	usleep(philo->info->time_to_eat * 1000);
	uber_fork(philo, philo->info->forks, 1);
	sem_wait(philo->info->meal);
	philo->meals++;
	sem_post(philo->info->meal);
}

void	*death(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	sem_wait(philo->info->die);
	sem_wait(philo->info->is_dead);
	philo->is_dead = 1;
	sem_post(philo->info->is_dead);
	sem_post(philo->info->die);
	return ((void *)0);
}

void	*set_philo(t_philo *p, t_data *d)
{
	pthread_create(&p->timer, NULL, &timer, p);
	pthread_detach(p->timer);
	sem_wait(p->info->start);
	sem_post(p->info->start);
	if (p->id % 2 == 1 && p->info->time_to_eat * 1000 > 10000)
		usleep(p->info->time_to_eat * 1000);
	else if (p->id % 2 == 1)
		usleep(10000);
	pthread_create(&p->test_death, NULL, &death, p);
	while (nostop(p))
	{
		uber_eat(p);
		if (p->info->gang_len == 1 || !nostop(p))
			break ;
		protected_print(p, SLEEPING);
		usleep(p->info->time_to_sleep * 1000);
		if (!nostop(p))
			break ;
		protected_print(p, THINKING);
	}
	if (p->info->gang_len == 1)
		return (pthread_join(p->test_death, NULL), free_all(d, 1), (void *)0);
	return (usleep(p->info->gang_len * 5000), sem_post(p->info->is_done),
		sem_wait(p->info->is_done), sem_post(p->info->is_done),
		pthread_join(p->test_death, NULL), usleep(p->info->gang_len * 4000), free_all(d, 1), (void *)0);
}
