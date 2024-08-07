/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/08 16:53:00 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	*prepare_executable_and_message(t_minishell *m, char *command)
{
	char	*executable;
	char	*mes;

	mes = ft_strjoin(ft_strjoin("bash: ", command), ": command not found");
	if (access(command, X_OK) == 0)
		executable = command;
	else
		executable = get_executable(m, command);
	if (!executable)
	{
		free(mes);
		return (NULL);
	}
	return (executable);
}

void	execute_command(t_minishell *m, char *executable, char **argv)
{
	int	pid;
	int	status;

	pid = fork();
	signal(SIGINT, handle_child_process);
	signal(SIGQUIT, handle_child_process);
	if (pid == -1)
		return (free(executable), pic_err(m, 1, "Fork failed"));
	else if (pid == 0)
	{
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		m->exitcode = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		m->exitcode = 128 + WTERMSIG(status);
	signal(SIGINT, handle_main_process);
	signal(SIGQUIT, handle_main_process);
	free(executable);
}
