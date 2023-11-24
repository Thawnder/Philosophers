/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 11:06:13 by bpleutin          #+#    #+#             */
/*   Updated: 2023/10/25 10:32:56 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_alltime	ft_atoll(char *nptr)
{
	int			i;
	int			sign;
	t_alltime	res;

	i = 0;
	sign = 1;
	res = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - 48;
		i++;
	}
	return ((int)res * sign);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	res;
	size_t	i;

	res = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (res / size != nmemb || res / nmemb != size)
		return (NULL);
	ptr = malloc(res);
	i = 0;
	if (ptr)
	{
		while (i < res)
		{
			*(unsigned char *)(ptr + i) = 0;
			i++;
		}
	}
	return (ptr);
}

void	init_all(t_data *data, char **argv, int i)
{
	data->info.time_to_die = ft_atoll(argv[2]);
	data->info.time_to_eat = ft_atoll(argv[3]);
	data->info.time_to_sleep = ft_atoll(argv[4]);
	pthread_mutex_init(&data->info.die, NULL);
	pthread_mutex_init(&data->info.start, NULL);
	data->philo = ft_calloc(data->info.gang_len, sizeof(t_philo));
	while (++i < data->info.gang_len)
	{
		data->philo[i].info = &data->info;
		data->philo[i].id = i;
		pthread_mutex_init(&data->philo[i].m_state, NULL);
		pthread_mutex_init(&data->philo[i].meal, NULL);
		pthread_mutex_init(&data->philo[i].last, NULL);
		pthread_mutex_init(&data->info.forks[i], NULL);
	}
	i = -1;
	while (++i < data->info.gang_len)
	{
		if (i == 0)
			data->philo[i].left = &data->info.forks[data->info.gang_len - 1];
		else
			data->philo[i].left = &data->info.forks[i - 1];
		data->philo[i].right = &data->info.forks[i];
	}
	data->info.start_time = get_time();
}

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	pthread_mutex_unlock(&data->info.start);
	while (++i < data->info.gang_len)
		pthread_join(data->philo[i].tid, NULL);
	pthread_join(data->timer_tid, NULL);
	i = -1;
	while (++i < data->info.gang_len)
	{
		pthread_mutex_destroy(&data->philo[i].info->forks[i]);
		pthread_mutex_destroy(&data->philo[i].m_state);
		pthread_mutex_destroy(&data->philo[i].meal);
		pthread_mutex_destroy(&data->philo[i].last);
	}
	pthread_mutex_destroy(&data->info.write);
	pthread_mutex_destroy(&data->info.start);
	pthread_mutex_destroy(&data->info.end);
	pthread_mutex_destroy(&data->info.die);
	free(data->info.forks);
	free(data->philo);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc < 5 || argc > 6 || argv[1][0] == '-' || argv[2][0] == '-'
		|| argv[3][0] == '-' || argv[4][0] == '-')
		return (printf("Invalid args\n"), -1);
	data.info.dead = 0;
	data.info.finished = 0;
	data.info.max_meals = -1;
	data.info.gang_len = (int) ft_atoll(argv[1]);
	data.info.forks = ft_calloc(data.info.gang_len, sizeof(pthread_mutex_t));
	if (argc == 6)
		data.info.max_meals = (int) ft_atoll(argv[5]);
	i = -1;
	pthread_mutex_init(&data.info.write, NULL);
	pthread_mutex_init(&data.info.end, NULL);
	init_all(&data, argv, i);
	while (++i < data.info.gang_len)
		data.philo[i].last_meal = get_time();
	pthread_mutex_lock(&data.info.start);
	pthread_create(&data.timer_tid, NULL, &timer, &data);
	i = -1;
	while (++i < data.info.gang_len)
		pthread_create(&data.philo[i].tid, NULL, &set_philo, &data.philo[i]);
	return (free_all(&data), 0);
}
