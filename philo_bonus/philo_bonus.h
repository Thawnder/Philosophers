/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:33:21 by bpleutin          #+#    #+#             */
/*   Updated: 2023/11/09 14:11:10 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# define START 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define DEAD 4

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <signal.h>
# include <pthread.h>
# include <semaphore.h>

typedef unsigned long long int	t_alltime;

typedef struct s_info
{
	int				gang_len;
	int				max_meals;
	int				finished;
	t_alltime		time_to_die;
	t_alltime		time_to_eat;
	t_alltime		time_to_sleep;
	t_alltime		start_time;
	sem_t			*forks;
	sem_t			*write;
	sem_t			*start;
	sem_t			*end;
	sem_t			*is_done;
	sem_t			*is_dead;
	sem_t			*die;
	sem_t			*last;
	sem_t			*meal;
	sem_t			*s_state;
	pthread_t		all_done;
}		t_info;

typedef struct s_philo
{
	int				id;
	pid_t			pid;
	t_alltime		last_meal;
	int				state;
	int				is_dead;
	int				is_done;
	int				meals;
	pthread_t		test_death;
	pthread_t		timer;
	t_info			*info;
}		t_philo;

typedef struct s_data
{
	t_info		info;
	t_philo		*philo;
}		t_data;

int			nostop(t_philo *philo);
void		init_sem(t_data *d);
void		*set_philo(t_philo *philo, t_data *d);
void		*timer(void *ptr);
t_alltime	get_time(void);
void		protected_print(t_philo *philo, int state);
void		free_all(t_data *data, int flag);
void		test_sem(t_data *d, sem_t *sem, int flag);
void		*all_done(void *ptr);

#endif
