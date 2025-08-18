/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export_sort.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 22:50:00 by marvin            #+#    #+#             */
/*   Updated: 2025/08/18 12:47:30 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_option_arg(const char *s)
{
	if (!s)
		return (0);
	if (s[0] == '-' && s[1] != '\0')
		return (1);
	return (0);
}

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

static void	print_env_formatted(char **envp)
{
	int		i;
	char	*eq;

	i = 0;
	while (envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			*eq = '\0';
			ft_putstr_fd(envp[i], STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(eq + 1, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
			*eq = '=';
		}
		else
			ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
}

void	print_sorted_env(char **envp)
{
	char	**copy;

	copy = copy_env_cat(envp, CAT_MEM);
	if (!copy)
		return ;
	bubble_sort(copy);
	print_env_formatted(copy);
	free_gc_cat(CAT_MEM);
}
