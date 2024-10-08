/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:43 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 15:23:42 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// Function to remove an environment variable from envp
void	ft_unset(t_envlst **envp, char **argv)
{
	t_envlst	*tmp;
	t_envlst	*prev;
	int			i;

	i = 1;
	while (argv[i])
	{
		tmp = *envp;
		prev = NULL;
		while (tmp)
		{
			if (ft_strcmp(tmp->name, argv[i]) == 0)
			{
				if (prev)
					prev->next = tmp->next;
				else
					*envp = tmp->next;
				free_envlst_node(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}

// Function to free the memory of an envlst node
void	free_envlst_node(t_envlst *node)
{
	if (ft_strcmp(node->name, "OLDPWD") == 0)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}
