/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:11:51 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 00:25:36 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	free_token_resources(t_token *newtok)
{
	if (newtok->str)
		free(newtok->str);
	free(newtok);
}

t_token	*create_token(char *str, char *expmap)
{
	t_token	*newtok;

	if (!validate_input(str, expmap))
		return (NULL);
	newtok = (t_token *)malloc(sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = WORD;
	newtok->is_freed = 0;
	newtok->str = ft_strdup(str);
	if (!newtok->str)
		return (free(newtok), NULL);
	newtok->expmap = ft_strdup(expmap);
	if (!newtok->expmap)
	{
		free(newtok->str);
		free(newtok);
		return (NULL);
	}
	newtok->rdrcmeta = NULL;
	newtok->rdrctarget = NULL;
	newtok->had_quote = (ft_strchr(str, '\"') || ft_strchr(str, '\''));
	return (newtok);
}

void	print_toktype(enum e_toktype token)
{
	if (token == REDIRECTION)
		ft_printf("Redirection");
	else if (token == PIPE)
		ft_printf("Pipe");
	else if (token == COMMAND)
		ft_printf("Command");
	else if (token == WORD)
		ft_printf("Word");
	else if (token == MINIFILE)
		ft_printf("File");
	else if (token == DELIMITER)
		ft_printf("Delimiter");
	else if (token == UNSET)
		ft_printf("UNSET");
	else
		ft_printf("Unknown");
}
