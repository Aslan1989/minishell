/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_comms.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/15 17:26:09 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the memory allocated for a command structure and its linked commands.
 *
 * This function recursively frees each command in the linked list, including
 * their commands and path strings. It also handles nested commands (next_b).
 *
 * @param comms Pointer to the first command in the linked list.
 */
void ft_free_comms(t_cmd *comms)
{
	t_cmd *temp;

	while (comms)
	{
		temp = comms;
		comms = comms->next_a;
		if (temp->commands)
		{
			ft_free_split(temp->commands);
			temp->commands = NULL;
		}
		if (temp->path)
		{
			free(temp->path);
			temp->path = NULL;
		}
		if (temp->next_b)
			ft_free_comms(temp->next_b);
		free(temp);
	}
}

t_cmd *ft_tokenize(char *line)
{
	(void)line; // Placeholder for future implementation
	return NULL; // Return NULL for now, as this function is not implemented
}

void ft_generate_commands(char *line, t_cmd *comms)
{
	ft_free_comms(comms);
	(void)line; // Placeholder for future implementation
	(void)comms; // Placeholder for future implementation
}
