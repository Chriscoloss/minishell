/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:08:08 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 13:37:44 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	alloc_exit_buf(char **temp, char **expmap_temp, size_t res_len, size_t exit_len)
{
	*temp = ft_calloc(res_len + exit_len + 1, sizeof(char));
	*expmap_temp = ft_calloc(res_len + exit_len + 1, sizeof(char));
	if (!*temp || !*expmap_temp)
	{
		free(*temp);
		return (0);
	}
	return (1);
}

void	copy_append_str(char *dest, const char *src1, const char *src2, size_t len1, size_t len2)
{
	ft_strlcpy(dest, src1, len1 + 1);
	ft_strlcat(dest, src2, len1 + len2 + 1);
}

void	copy_append_expmap(char *dest, const char *src, const char *exit_status_str, size_t res_len)
{
	ft_strlcpy(dest, src, res_len + 1);
	ft_strfillcat(dest, exit_status_str, res_len > 0 ? src[res_len - 1] : 'E');
}

int	add_exit_status(char **res, char **expmap_res, const char *exit_status_str)
{
	t_temps	t;
	size_t	res_len;
	size_t	exit_len;

	res_len = ft_strlen(*res);
	exit_len = ft_strlen(exit_status_str);
	if (!alloc_exit_buf(&t.temp, &t.expmap_temp, res_len, exit_len))
	{
		exp_cln(res, expmap_res, NULL, NULL);
		return (0);
	}
	copy_append_str(t.temp, *res, exit_status_str, res_len, exit_len);
	copy_append_expmap(t.expmap_temp, *expmap_res, exit_status_str, res_len);
	free(*res);
	free(*expmap_res);
	*res = t.temp;
	*expmap_res = t.expmap_temp;
	return (1);
}

int	handle_exit_status(t_minishell *m, char **res, char **expmap_res)
{
	char	*exit_status_str;

	if (!are_exitinputs_valid(m, res, expmap_res))
		return (-1);
	exit_status_str = get_exit_status_str(m);
	if (!exit_status_str)
	{
		exp_cln(res, expmap_res, NULL, NULL);
		return (-1);
	}
	if (!add_exit_status(res, expmap_res, exit_status_str))
	{
		free(exit_status_str);
		return (-1);
	}
	free(exit_status_str);
	return (2);
}