/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:33:21 by bpleutin          #+#    #+#             */
/*   Updated: 2023/10/25 10:12:11 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define START 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define DEAD 4

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef unsigned long long int	t_alltime;

typedef struct s_info
{
	int				gang_len;
	int				max_meals;
	int				dead;
	int				finished;
	t_alltime		time_to_die;
	t_alltime		time_to_eat;
	t_alltime		time_to_sleep;
	t_alltime		start_time;
	pthread_mutex_t	end;
	pthread_mutex_t	start;
	pthread_mutex_t	write;
	pthread_mutex_t	die;
	pthread_mutex_t	*forks;
}		t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	t_alltime		last_meal;
	int				state;
	int				meals;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	meal;
	pthread_mutex_t	last;
	pthread_mutex_t	m_state;
	t_info			*info;
}		t_philo;

typedef struct s_data
{
	t_info		info;
	t_philo		*philo;
	pthread_t	timer_tid;
}		t_data;

void		*set_philo(void *ptr);
void		*timer(void *ptr);
t_alltime	get_time(void);
void		protected_print(t_philo *philo);

#endif
