/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpleutin <bpleutin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 11:06:13 by bpleutin          #+#    #+#             */
/*   Updated: 2023/11/09 16:15:07 by bpleutin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	if (res > INT_MAX)
		return (0);
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

void	*init_all(t_data *d, char **argv, int i)
{
	if ((!sem_unlink("forks") && !sem_unlink("write") && !sem_unlink("start")
			&& !sem_unlink("end") && !sem_unlink("die") && !sem_unlink("last")
			&& !sem_unlink("state") && !sem_unlink("meal")
			&& !sem_unlink("is_dead") && !sem_unlink("is_done")) || 1)
		d->info.gang_len = (int) ft_atoll(argv[1]);
	d->info.time_to_die = ft_atoll(argv[2]);
	d->info.time_to_eat = ft_atoll(argv[3]);
	d->info.time_to_sleep = ft_atoll(argv[4]);
	d->philo = ft_calloc(d->info.gang_len, sizeof(t_philo));
	init_sem(d);
	while (++i < d->info.gang_len)
	{
		d->philo[i].info = &d->info;
		d->philo[i].id = i;
	}
	return (d->info.start_time = get_time(), (void *)0);
}

void	end(t_data *data)
{
	int		i;
	pid_t	pid;

	if (data->info.gang_len > 1)
	{
		i = -1;
		while (++i < data->info.gang_len)
			sem_wait(data->info.is_done);
		sem_post(data->info.die);
		usleep(10000 * data->info.gang_len);
		i = -1;
		while (++i < data->info.gang_len)
			sem_post(data->info.is_done);
	}
	i = 0;
	while (i < data->info.gang_len)
	{
		pid = waitpid(-1, NULL, 0);
		if (pid > 0)
			i++;
	}
	free_all(data, 0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc < 5 || argc > 6 || argv[1][0] == '-' || argv[2][0] == '-'
		|| argv[3][0] == '-' || argv[4][0] == '-')
		return (printf("Invalid args\n"), -1);
	data.info.gang_len = 0;
	data.info.finished = 0;
	data.info.max_meals = -1;
	if (argc == 6)
		data.info.max_meals = (int) ft_atoll(argv[5]);
	i = -1;
	init_all(&data, argv, i);
	while (++i < data.info.gang_len)
		data.philo[i].last_meal = get_time();
	sem_wait(data.info.start);
	sem_wait(data.info.die);
	i = -1;
	while (++i < data.info.gang_len)
	{
		data.philo[i].pid = fork();
		if (data.philo[i].pid == 0)
			set_philo(&data.philo[i], &data);
	}
	return (sem_post(data.info.start), end(&data), 0);
}
