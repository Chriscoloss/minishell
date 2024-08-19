/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:50:42 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 09:29:12 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	init_tokenizer_state(t_tokenizer_state *state, char *p)
{
	*state = (t_tokenizer_state){
		.ptr = p,
		.current_pos = 0,
		.quote_level = 0,
		.escape_next = 0,
		.current_token_size = 10
	};
	state->current_token = ft_calloc(state->current_token_size, sizeof(char));
	state->expmap = ft_calloc(state->current_token_size, sizeof(char));
	if (!state->current_token || !state->expmap)
	{
		free(state->current_token);
		free(state->expmap);
		ft_error_exit("malloc");
	}
}

void	process_char(t_minishell *m, t_tokenizer_state *s)
{
	if (s->escape_next)
	{
		handle_escape_char(s);
		s->escape_next = 0;
	}
	else if (*s->ptr == '\\')
		handle_backslash(s);
	else if (*s->ptr == '\'' && s->quote_level != 2)
		handle_single_quote(s);
	else if (*s->ptr == '"' && s->quote_level != 1)
		handle_double_quote(s);
	else if (ft_isspace(*s->ptr) && s->quote_level == 0)
		handle_space(m, s);
	else if (*s->ptr == '|' && s->quote_level == 0)
		handle_pipe(m, s);
	else if ((*s->ptr == '>' || *s->ptr == '<') && s->quote_level == 0)
		handle_redirection(m, s);
	else
		handle_regular_char(s);
}

void	cleanup_tokenizer_state(t_tokenizer_state *state)
{
	if (state->current_token)
		free(state->current_token);
	if (state->expmap)
		free(state->expmap);
}

void	handle_error_and_cleanup(t_tokenizer_state *state, t_minishell *m)
{
	cleanup_tokenizer_state(state);
	mlstclear(m->tok_lst);
}

void	prompt_to_token(t_minishell *m)
{
	t_tokenizer_state	state;
	t_token				*token;

	init_tokenizer_state(&state, m->prompt);
	while (*state.ptr)
	{
		process_char(m, &state);
		if (state.current_pos >= state.current_token_size - 1)
			resize_token_buffers(&state);
	}
	if (state.current_pos > 0)
	{
		state.current_token[state.current_pos] = '\0';
		state.expmap[state.current_pos] = '\0';
		token = create_token(state.current_token, state.expmap);
		if (!token)
		{
			cleanup_tokenizer_state(&state);
			ft_error_exit("Error while create token\n");
		}
		update_tok_type(token, WORD);
		add_token_to_list(&(m->tok_lst), token);
	}
	cleanup_tokenizer_state(&state);
}
