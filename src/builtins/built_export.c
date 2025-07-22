/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:37:58 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/22 19:15:07 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a string of format "key=value" from two parts.
 *
 * This helper function is used when setting or adding environment variables.
 *
 * @param key Name of the environment variable.
 * @param value Value of the variable.
 * @return char* Newly allocated string in the form key=value.
 */
static char *make_env_string(const char *key, const char *value)
{
	size_t key_len;
	size_t val_len;
	char *env_str;
	size_t i;
	size_t j;

	key_len = ft_strlen(key);
	val_len = ft_strlen(value);
	// env_str = malloc(key_len + val_len + 2); // +1 for '=' and +1 for '\0'

	env_str = ft_gcmalloc(CAT_ENV, key_len + val_len + 2);
	i = 0;
	j = 0;
	if (!env_str)
		return NULL;
	while (i < key_len)
	{
		env_str[i] = key[i];
		i++;
	}
	env_str[i++] = '=';
	while (j < val_len)
		env_str[i++] = value[j++];
	env_str[i] = '\0';
	return env_str;
}

/**
 * @brief Replaces the value of an existing environment variable.
 *
 * This function searches for a variable by name (e.g., "PATH"), and if found,
 * frees the old value and inserts the new one.
 *
 * @param envp Array of environment strings.
 * @param key Name of the variable to replace.
 * @param value New value to assign.
 * @return int 1 if replaced, 0 if not found.
 */
static int replace_env_var(char **envp, const char *key, const char *value)
{
	size_t len;
	int i;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			// free(envp[i]);
			ft_gcfree(CAT_ENV, envp[i]);
			envp[i] = make_env_string(key, value); // key=value
			return 1;
		}
		i++;
	}
	return 0;
}

/**
 * @brief Adds a new environment variable to the shell environment.
 *
 * Allocates a new array of strings that is one element larger, copies
 * all old values into it, and appends the new key=value string.
 *
 * @param shell Pointer to shell containing envp.
 * @param key Name of the new variable.
 * @param value Value of the new variable.
 * @return int 0 on success, 1 on failure.
 */
static int add_env_var(t_shell *shell, const char *key, const char *value)
{
	int count;
	int i;
	char **new_env;

	// Count how many variables exist
	count = 0;
	while (shell->envp[count])
		count++;
	// Allocate space for old + new + NULL
	// new_env = malloc(sizeof(char *) * (count + 2));
	new_env = ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	// Copy old values
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->envp[i]; // Move pointer, not duplicating
		i++;
	}
	// Add the new variable at the end
	new_env[count] = make_env_string(key, value); // e.g., "USER=student"
	new_env[count + 1] = NULL;
	// Free the old pointer to the array (but not strings — they are moved)
	ft_gcfree(CAT_ENV, shell->envp);
	shell->envp = new_env;
	return (0);
}

/**
 * @brief Implements the `export` built-in command.
 *
 * Adds or updates environment variables in the shell's environment.
 * Example: export VAR=value
 * This function supports syntax checking and safely modifies the internal envp array.
 *
 * @param shell Pointer to the shell structure (contains envp).
 * @param args The array of strings, where args[0] = "export", args[1] = "VAR=value", etc.
 * @return int 0 on success, 1 on invalid format or allocation failure.
 */
int built_export(t_shell *shell, char **args)
{
	int i;
	const char *key;
	const char *value;
	char *equal;

	i = 1;
	while (args[i])
	{
		equal = ft_strchr(args[i], '='); // Find the '=' sign in the string
		if (!equal)
		{
			// Invalid format: no '=' in string
			write(2, "minishell: export: invalid format: ", 36);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "\n", 1);
			i++;
			continue;
		}
		// Temporarily split the string into key and value
		*equal = '\0';
		key = args[i];
		value = equal + 1;
		// Try to replace if it exists already
		if (!replace_env_var(shell->envp, key, value))
			add_env_var(shell, key, value);
		*equal = '='; // Restore the original string for safety
		i++;
	}
	return 0;
}
