/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:49:45 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 11:02:28 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// Echo builtin
void	ft_echo(char **args)
{
	bool	newline;
	int		start_index;

	if (ft_strcmp(args[0], "echo") == 0)
		start_index = handle_options(args, &newline, 1);
	else
		start_index = handle_options(args, &newline, 0);
	print_output(args, start_index, newline);
}

// Returns the index of the first argument that is not an option
int	handle_options(char **args, bool *newline, int start_index)
{
	int	i;
	int	j;

	*newline = true;
	i = start_index;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			*newline = false;
			i++;
		}
		else
			break ;
	}
	return (i);
}

// Print output with or without newline
void	print_output(char **args, int start_index, bool newline)
{
	int	i;

	i = start_index;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
