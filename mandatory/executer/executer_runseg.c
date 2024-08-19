/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:46:24 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 09:20:25 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run(t_minishell *m, char **args, int arg_count, t_fd *fd)
{
	if (is_builtin(args[0]))
		exec_builtin_cmd(m, args, arg_count, fd);
	else
		execute_external_command(m, args, fd);
}

void	cleanup_fds(t_fd *fd)
{
	if (fd->last_input != fd->input)
		close(fd->last_input);
	if (fd->last_output != fd->output)
		close(fd->last_output);
}

void	run_seg(t_minishell *m, int i, int input_fd, int output_fd)
{
	t_fd	fd;

	init_fd(&fd, input_fd, output_fd);
	if (m->pipes == 0)
		process_tok(m, m->exec_lst, &fd);
	else
		process_tok(m, m->exec_seqs[i], &fd);
	if (m->args[0])
		run(m, m->args, m->argc, &fd);
	reset_minishell_args(m);
	cleanup_fds(&fd);
}
