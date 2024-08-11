/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/11 21:42:42 by jeberle          ###   ########.fr       */
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
