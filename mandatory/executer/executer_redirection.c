/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:29:16 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 10:44:54 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run_in_redirection(t_token *token, t_fd *fd)
{
	if (fd->last_input != fd->input)
		close(fd->last_input);
	fd->last_input = open(token->rdrcmeta, O_RDONLY);
	if (fd->last_input == -1)
	{
		ft_fprintf(2, "Error input file: %s\n", token->rdrcmeta);
		return ;
	}
}

void	run_out_redirection(t_token *token, t_fd *fd)
{
	int	flags;

	if (fd->last_output != fd->output)
		close(fd->last_output);
	if (ft_strcmp(token->str, ">") == 0)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd->last_output = open(token->rdrctarget, flags, 0644);
	if (fd->last_output == -1)
	{
		ft_fprintf(2, "Error output file: %s\n", token->rdrctarget);
		return ;
	}
}

void	run_redirection(t_token *t, t_fd *fd)
{
	if (ft_strcmp(t->str, "<<") == 0)
		run_heredoc(t, fd);
	else if (ft_strcmp(t->str, "<") == 0)
		run_in_redirection(t, fd);
	else if (ft_strcmp(t->str, ">") == 0 || ft_strcmp(t->str, ">>") == 0)
		run_out_redirection(t, fd);
}

void	run_heredoc(t_token *t, t_fd *fd)
{
	int	hd[2];

	if (pipe(hd) == -1)
	{
		ft_fprintf(2, "Error creating pipe for heredoc\n");
		return ;
	}
	write(hd[1], t->rdrcmeta, ft_strlen(t->rdrcmeta));
	close(hd[1]);
	if (fd->last_input != fd->input)
		close(fd->last_input);
	fd->last_input = hd[0];
}

void	run_command(t_minishell *m, char **args)
{
	char	*path;

	path = get_executable(m, args[0]);
	if (path)
	{
		execute_command(m, path, args);
		free(path);
	}
	ft_fprintf(2, "bash: %s: command not found\n", args[0]);
}
