/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:22:44 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:19:13 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Counts the number of words in a string, separated by a given delimiter.
 *
 * @param s The input string.
 * @param c The delimiter character.
 * @return The number of words in the string.
 */
static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}
/**
 * @brief Extracts the next word from a string based on a delimiter.
 *
 * @param s Pointer to the input string (modified to update position).
 * @param c The delimiter character.
 * @return A newly allocated string containing the next word.
 * Skip delimiter characters
 * Find the length of the next word
 * Allocate memory for the word and copy it
 * Move the string pointer forward
 */

static char	*get_next_word(const char **s, char c)
{
	size_t		len;
	char		*word;
	const char	*start;

	len = 0;
	while (**s == c && **s)
		(*s)++;
	start = *s;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, start, len);
	word[len] = '\0';
	*s += len;
	return (word);
}

static void	free_all(char **words, int index)
{
	while (index >= 0)
		free(words[index--]);
	free(words);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	words = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		words[i] = get_next_word(&s, c);
		if (!words[i])
		{
			free_all(words, i - 1);
			return (NULL);
		}
		i++;
	}
	words[i] = NULL;
	return (words);
}

// void	print_split(char **words)
// {
// 	int i = 0;
// 	while (words[i])
// 	{
// 		printf("Word %d: %s\n", i, words[i]);
// 		free(words[i]);
// 		i++;
// 	}
// 	free(words);
// }

// int main(void)
// {
// 	char **result_split;

// 	result_split = ft_split("Hello, world! How are you?", ' ');
// 	print_split(result_split);

// 	result_split = ft_split("----Hello---World----", '-');
// 	print_split(result_split);

// 	result_split = ft_split("OneWord", ' ');
// 	print_split(result_split);

// 	result_split = ft_split("", ' ');
// 	print_split(result_split);
// }
