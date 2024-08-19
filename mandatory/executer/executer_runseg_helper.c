/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:31:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 10:47:15 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	add_argument(t_minishell *m, char *arg)
{
	int	i;

	i = 0;
	while (i < 1024)
	{
		if (m->args[i] == NULL)
		{
			m->args[i] = arg;
			break ;
		}
		i++;
	}
	if (i == 1024)
		ft_fprintf(2, "Error: Maximum number of arguments reached\n");
}

void	exec_builtin_cmd(t_minishell *m, char **args, int arg_count, t_fd *fd)
{
	if (fd->last_input != fd->input)
		dup2(fd->last_input, STDIN_FILENO);
	if (fd->last_output != fd->output)
		dup2(fd->last_output, STDOUT_FILENO);
	execute_builtin(m, args[0], args, arg_count);
}

void	execute_command_in_child(char *e, char **args, t_minishell *m, t_fd *fd)
{
	setup_child_process(fd);
	if (execve(e, args, m->envp) == -1)
	{
		ft_fprintf(2, "execve failed\n");
		exit(1);
	}
}

void	execute_external_command(t_minishell *m, char **args, t_fd *fd)
{
	pid_t	pid;
	char	*tmp;

	tmp = get_executable(m, args[0]);
	if (!tmp)
	{
		ft_fprintf(2, "bash: %s: Command not found\n", args[0]);
		m->exitcode = 127;
		return ;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		execute_command_in_child(tmp, args, m, fd);
	else if (pid < 0)
	{
		ft_fprintf(2, "Fork failed\n");
		m->exitcode = 1;
	}
	else
	{
		run_parent_process(m, pid);
	}
	free(tmp);
}

void	process_tok(t_minishell *m, t_list *exec_lst, t_fd *fd)
{
	t_list	*current;
	t_token	*token;
	char	*dup_str;

	current = exec_lst;
	m->argc = 0;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == REDIRECTION)
			run_redirection(token, fd);
		else if (token->token == COMMAND || token->token == WORD)
		{
			dup_str = ft_strdup(token->str);
			if (!dup_str)
			{
				ft_fprintf(2, "Error: Memory allocation failed\n");
				return ;
			}
			add_argument(m, dup_str);
			m->argc++;
		}
		current = current->next;
	}
}
