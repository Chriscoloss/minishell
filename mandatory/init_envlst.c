/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 07:39:23 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	increment_shlvl(char **value)
{
	int		i;
	int		carry;
	char	*new_value;

	i = ft_strlen(*value) - 1;
	carry = 1;
	while (i >= 0 && carry)
	{
		if ((*value)[i] < '9')
		{
			(*value)[i] += 1;
			carry = 0;
		}
		else
			(*value)[i] = '0';
		i--;
	}
	if (carry)
	{
		new_value = malloc(strlen(*value) + 2);
		new_value[0] = '1';
		ft_strcpy(new_value + 1, *value);
		free(*value);
		*value = new_value;
	}
}

void	init_env_list(char **envp, t_minishell *m)
{
	char	*name;
	char	*value;

	while (*envp != NULL)
	{
		name = NULL;
		value = NULL;
		extract_name_value(*envp, &name, &value);
		if (ft_strcmp(name, "SHLVL") == 0)
			increment_shlvl(&value);
		if (ft_strcmp(name, "OLDPWD") != 0)
			add_env_node(&m->env_list, name, value);
		else
			ft_multifree(name, value, NULL, NULL);
		envp++;
	}
	add_env_node(&m->env_list, ft_strdup("OLDPWD"), NULL);
}

void	extract_name_value(char *arg, char **name, char **value)
{
	char	*equal_sign;
	char	*plus_sign;
	char	*trimmed_value;

	plus_sign = ft_strchr(arg, '+');
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		if (plus_sign && plus_sign < equal_sign)
			*name = ft_strndup(arg, plus_sign - arg);
		else
			*name = ft_strndup(arg, equal_sign - arg);
		trimmed_value = ft_strtrim(equal_sign + 1, "\"");
		*value = ft_strdup(trimmed_value);
		free(trimmed_value);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
		free(arg);
	}
}

void	add_env_node(t_envlst **env_list, char *name, char *value)
{
	t_envlst	*new_node;
	t_envlst	*temp;

	new_node = malloc(sizeof(t_envlst));
	if (new_node == NULL)
	{
		free(name);
		free(value);
		return ((void)ft_printf(MEM_ERR));
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (*env_list == NULL)
		*env_list = new_node;
	else
	{
		temp = *env_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}

t_envlst	*find_env_var(t_envlst *head, const char *name)
{
	t_envlst	*current;

	current = head;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
