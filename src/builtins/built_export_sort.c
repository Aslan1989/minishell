/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export_sort.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 22:50:00 by marvin            #+#    #+#             */
/*   Updated: 2025/08/05 22:50:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	bubble_sort(char **arr)
{
	int		count;
	int		j;
	int		k;
	char	*tmp;

	count = 0;
	while (arr[count])
		count++;
	j = 0;
	while (j < count - 1)
	{
		k = 0;
		while (k < count - j - 1)
		{
			if (ft_strcmp(arr[k], arr[k + 1]) > 0)
			{
				tmp = arr[k];
				arr[k] = arr[k + 1];
				arr[k + 1] = tmp;
			}
			k++;
		}
		j++;
	}
}

void	print_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		ft_printf("declare -x %s\n", envp[i]);
		i++;
	}
}

void	print_sorted_env(char **envp)
{
	char	**copy;

	copy = copy_env(envp);
	bubble_sort(copy);
	print_env(copy);
}
