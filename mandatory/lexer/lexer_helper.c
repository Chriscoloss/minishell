/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 10:14:04 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	add_new_abtoken(t_list **current, char *word, char *expmap)
{
	t_token	*new_token;
	t_list	*new_node;

	new_token = create_token(word, expmap);
	update_tok_type(new_token, WORD);
	new_node = ft_lstnew(new_token);
	new_node->next = (*current)->next;
	(*current)->next = new_node;
	*current = new_node;
}

void	process_abtoken(t_list **current, t_token *cur)
{
	char	*work;
	char	**words;
	int		wordcount;
	int		i;
	t_list	*next_node;

	next_node = (*current)->next;
	work = whitespace_handler(cur->str);
	if (!work)
		return ;
	wordcount = ft_count_words(work, ' ');
	words = ft_split(work, ' ');
	free(work);
	free(cur->str);
	cur->str = words[0];
	i = 1;
	while (i < wordcount)
	{
		add_new_abtoken(current, words[i], cur->expmap);
		free(words[i]);
		i++;
	}
	free(words);
	*current = next_node;
}

int	should_process_token(t_token *t)
{
	return (!ft_strcontains(t->expmap, 'X') && ft_strcontains(t->expmap, 'E'));
}

void	afterbreakup(t_list **tok_lst)
{
	t_list	*current;
	t_token	*cur;

	current = *tok_lst;
	while (current != NULL)
	{
		cur = (t_token *)current->content;
		if (should_process_token(cur))
		{
			process_abtoken(&current, cur);
		}
		else
		{
			current = current->next;
		}
	}
}

void	detect_lexing_errors(t_minishell *m)
{
	char	*work;
	char	quote_char;
	int		in_quote;

	work = m->prompt;
	quote_char = 0;
	in_quote = 0;
	while (*work != '\0')
	{
		if (!in_quote && (*work == '\'' || *work == '\"'))
		{
			quote_char = *work;
			in_quote = 1;
		}
		else if (in_quote && *work == quote_char)
			in_quote = 0;
		work++;
	}
	if (in_quote)
		pic_err(m, 2, "unclosed quotes");
}
