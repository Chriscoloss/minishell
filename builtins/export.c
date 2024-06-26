/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/06/26 13:44:18 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Recreates the expport behavior from bash
void	ft_export(t_envlst ***envp, int argc, char **argv)
{
	int		i;
	char	**envp_export;

	i = 0;
	if (argc == 1)
	{
		envp_export = copy_envp(**envp);
		sort_envp(envp_export);
		while (envp_export[i])
		{
			print_env_variable(envp_export[i]);
			free(envp_export[i++]);
		}
		free(envp_export);
	}
	else
		export(&(**envp), argv);
}

// Adds or updates the env variables in the envp list
void	export(t_envlst **envp, char **argv)
{
	int		i;
	char	*name;
	char	*value;

	i = 0;
	while (argv[i])
	{
		extract_name_value(argv[i], &name, &value);
		if (name)
			add_or_update_env_node(envp, name, value);
		i++;
	}
}

void	add_or_update_env_node(t_envlst **env_list, char *name, char *value)
{
	t_envlst	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			free(name);
			free(value);
			return ;
		}
		current = current->next;
	}
	add_env_node(env_list, name, value);
}

// Sorts a char ** array of env variables alphabetically
void	sort_envp(char **envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
}

// copies the envp array to a new array to avoid modifying the original
char	**copy_envp(t_envlst *envp)
{
	int		i;
	int		count;
	char	**envp_copy;
	char	*name_equals;

	i = 0;
	count = count_env_list(envp);
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
		return (NULL);
	while (envp)
	{
		if (envp->value)
		{
			name_equals = ft_strjoin(envp->name, "=");
			envp_copy[i] = ft_strjoin(name_equals, envp->value);
			free(name_equals);
		}
		else
			envp_copy[i] = ft_strdup(envp->name);
		envp = envp->next;
		i++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}
