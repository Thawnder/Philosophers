/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:43:05 by bpleutin          #+#    #+#             */
/*   Updated: 2023/11/09 16:15:30 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_alltime	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	protected_print(t_philo *philo, int state)
{
	sem_wait(philo->info->s_state);
	philo->state = state;
	sem_wait(philo->info->write);
	if (philo->state == EATING)
		printf("%llu philo %d is eating\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == SLEEPING)
		printf("%llu philo %d is sleeping\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == THINKING)
		printf("%llu philo %d is thinking\n",
			get_time() - philo->info->start_time, philo->id + 1);
	else if (philo->state == DEAD)
	{
		sem_wait(philo->info->is_dead);
		philo->is_dead = 1;
		sem_post(philo->info->is_dead);
		sem_post(philo->info->die);
		printf("%llu philo %d is dead\n",
			get_time() - philo->info->start_time, philo->id + 1);
		sem_post(philo->info->forks);
		sem_post(philo->info->forks);
	}
	sem_post(philo->info->write);
	sem_post(philo->info->s_state);
}

void	*timer(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	sem_wait(p->info->start);
	sem_post(p->info->start);
	while (!sem_wait(p->info->is_dead) && !p->is_dead
		&& !sem_post(p->info->is_dead)
		&& !sem_wait(p->info->end) && !p->info->finished)
	{
		sem_post(p->info->end);
		if (nostop(p) && !sem_wait(p->info->last)
			&& get_time() - p->last_meal > p->info->time_to_die + 1)
			return (sem_post(p->info->last), protected_print(p, DEAD),
				(void *)0);
		sem_post(p->info->last);
	}
	sem_post(p->info->is_dead);
	sem_post(p->info->end);
	sem_post(p->info->forks);
	sem_post(p->info->forks);
	return ((void *)0);
}

void	init_sem(t_data *d)
{
	if (d->info.gang_len - 1 > 0)
		d->info.forks = sem_open("forks", O_CREAT | O_EXCL,
				0644, d->info.gang_len - 1);
	else
		d->info.forks = sem_open("forks", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.forks, d->info.gang_len);
	d->info.write = sem_open("write", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.write, 1);
	d->info.start = sem_open("start", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.start, 1);
	d->info.end = sem_open("end", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.end, 1);
	d->info.die = sem_open("die", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.die, 1);
	d->info.is_done = sem_open("is_done", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.is_done, 0);
	d->info.is_dead = sem_open("is_dead", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.is_dead, 1);
	d->info.last = sem_open("last", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.last, 1);
	d->info.s_state = sem_open("state", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.s_state, 1);
	d->info.meal = sem_open("meal", O_CREAT | O_EXCL, 0644, 0);
	test_sem(d, d->info.meal, 1);
}
