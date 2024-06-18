/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:56 by chorst            #+#    #+#             */
/*   Updated: 2024/06/18 10:51:32 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void ft_env(char ***envp)
{
	int i = 0;
	while ((*envp)[i] != 0)
	{
		ft_printf("%s\n", (*envp)[i]);
		i++;
	}
}
