/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:42:27 by aisaev            #+#    #+#             */
/*   Updated: 2025/06/14 10:38:02 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    ft_atoi: Converts a string to an integer.
    - Skips leading whitespace characters.
    - Handles an optional '+' or '-' sign.
    - Converts numeric characters into an integer.
    - Stops conversion at the first non-digit character.
    - Returns the resulting integer with the correct sign.
*/
long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

// #include <stdint.h>

// int main(void)
// {
// 	printf("Test 1: %d\n", ft_atoi("42"));        // 42
//     printf("Test 2: %d\n", ft_atoi("-42"));       // -42
//     printf("Test 3: %d\n", ft_atoi("   123"));    // 123
//     printf("Test 4: %d\n", ft_atoi("+456"));      // 456
//     printf("Test 5: %d\n", ft_atoi("  -00123"));  // -123
//     printf("Test 6: %d\n", ft_atoi("999abc"));    // 999 (остановится на 'a')
//     printf("Test 7: %d\n", ft_atoi("   -+42"));   // 0 (некорректный ввод)
//     printf("Test 8: %d\n", ft_atoi("abc123"));    // 0 (начинается с буквы)
// }
