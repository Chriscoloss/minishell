/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:16:46 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 10:47:12 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static char	*find_executable(char **paths, int pathcount, char *command)
{
	int		i;
	char	*joined;
	char	*full_path;

	i = 0;
	while (i < pathcount)
	{
		full_path = ft_strjoin(paths[i], "/");
		if (full_path == NULL)
			return (NULL);
		joined = ft_strjoin(full_path, command);
		free(full_path);
		if (joined == NULL)
			return (NULL);
		if (access(joined, X_OK) == 0)
			return (joined);
		free(joined);
		i++;
	}
	return (NULL);
}

void	newfree_token(t_token *token)
{
	if (token)
	{
		if (token->str)
		{
			free(token->str);
			token->str = NULL;
		}
		if (token->expmap)
		{
			free(token->expmap);
			token->expmap = NULL;
		}
		if (token->rdrcmeta)
		{
			free(token->rdrcmeta);
			token->rdrcmeta = NULL;
		}
		if (token->rdrctarget)
		{
			free(token->rdrctarget);
			token->rdrctarget = NULL;
		}
		free(token);
		token = NULL;
	}
}

char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	**paths;
	int		pathcount;
	char	*result;

	if (ft_strcmp("", command) == 0)
		return (NULL);
	if (ft_strchr(command, '/') && access(command, F_OK) == 0)
		return (ft_strdup(command));
	pathline = my_getenv("PATH", m->env_list);
	if (pathline != NULL)
	{
		pathcount = ft_count_words(pathline, ':');
		paths = ft_split(pathline, ':');
		if (paths == NULL)
			return (NULL);
		result = find_executable(paths, pathcount, command);
		ft_array_l_free(paths, pathcount);
		return (result);
	}
	return (NULL);
}

void	execute_command(t_minishell *m, char *executable, char **argv)
{
	char	**own;

	own = own_env(m->env_list);
	if (is_builtin(executable))
		execute_builtin(m, executable, argv, ft_array_length(argv));
	else
	{
		execve(executable, argv, own);
		perror("execve failed");
		free(executable);
		write(1, "cmds\n", 5);
		exit(EXIT_FAILURE);
	}
}

int	keep_for_exec(t_token *token)
{
	if (token->token == WORD || token->token == COMMAND || token->token == PIPE)
		return (1);
	return (0);
}
