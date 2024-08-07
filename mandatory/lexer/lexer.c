/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/05 17:49:44 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	afterbreakup(t_minishell *m)
{
	t_list	*current;
	t_list	*new_node;
	t_token	*cur_content;
	t_token	*new_token;
	int		wordcount;
	int		i;
	char	*work;
	char	**words;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (!ft_strcontains(cur_content->expmap, 'X') && ft_strcontains(cur_content->expmap, 'E'))
		{
			work = whitespace_handler(cur_content->str);
			wordcount = ft_count_words(work, ' ');
			words = ft_split(work, ' ');
			free(cur_content->str);
			cur_content->str = words[0];
			i = 1;
			while (i < wordcount)
			{
				new_token = create_token(words[i], cur_content->expmap);
				update_tok_type(new_token, WORD);
				new_node = ft_lstnew(new_token);
				new_node->next = current->next;
				current->next = new_node;
				current = new_node;
				i++;
			}
			free(words);
		}
		current = current->next;
	}
}

void	detect_lexing_errors(t_minishell *m)
{
	int		quotecount;
	char	*work;

	quotecount = 0;
	work = m->prompt;
	while (*work != '\0')
	{
		if (*work == '\"' || *work == '\'')
			quotecount++;
		work++;
	}
	if (quotecount % 2 != 0)
		pic_err(m, 2, "unclosed quotes");
}


void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

	if (token)
	{
		new_element = ft_lstnew((void *)token);
		if (!new_element)
			return ;
		if (*lst == NULL)
			*lst = new_element;
		else
		{
			current = *lst;
			while (current->next != NULL)
				current = current->next;
			current->next = new_element;
		}
	}
}

void	prompt_to_token(t_minishell *m)
{
	int		current_pos;
	int		in_single_quotes;
	int		in_double_quotes;
	int		escape_next;
	int		current_token_size;
	t_token	*token;
	char	*ptr;
	char	*current_token;
	char	*expmap;
	char	*new_token;
	char	*new_expmap;

	current_pos = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	escape_next = 0;
	current_token_size = 10;
	current_token = malloc(current_token_size);
	expmap = malloc(current_token_size);
	ptr = m->prompt;
	if (!current_token || !expmap)
		ft_error_exit("malloc");
	while (*ptr)
	{
		if (escape_next)
		{
			current_token[current_pos] = *ptr;
			expmap[current_pos] = in_single_quotes + '0';
			current_pos++;
			escape_next = 0;
			ptr++;
			continue ;
		}
		if (*ptr == '\\')
		{
			if (in_single_quotes)
			{
				current_token[current_pos] = *ptr;
				expmap[current_pos] = '1';
				current_pos++;
			}
			else if (in_double_quotes)
			{
				if (ft_strpbrk(&(*(ptr + 1)), "$`\"\\\n"))
					escape_next = 1;
				else
				{
					current_token[current_pos] = *ptr;
					expmap[current_pos] = in_single_quotes + '0';
					current_pos++;
				}
			}
			else
				escape_next = 1;
			ptr++;
			continue ;
		}
		if (*ptr == '\'')
		{
			if (!in_single_quotes)
				in_single_quotes = 1;
			else
				in_single_quotes = 0;
			ptr++;
			continue ;
		}
		if (in_single_quotes)
		{
			current_token[current_pos] = *ptr;
			expmap[current_pos] = '1';
			current_pos++;
			ptr++;
			continue ;
		}
		if (in_double_quotes)
		{
			if (*ptr == '"')
				in_double_quotes = 0;
			else
			{
				current_token[current_pos] = *ptr;
				expmap[current_pos] = '2';
				current_pos++;
			}
			ptr++;
			continue ;
		}
		if (*ptr == '\'')
		{
			in_single_quotes = 1;
			if (*(ptr + 1) == '\0')
				ptr++;
			continue ;
		}
		if (*ptr == '"')
		{
			in_double_quotes = 1;
			ptr++;
			continue ;
		}
		if (ft_isspace(*ptr))
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(&m->tok_lst, token);
				current_pos = 0;
				current_token[0] = '\0';
				expmap[0] = '\0';
			}
			ptr++;
			continue ;
		}
		if (*ptr == '|')
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(&m->tok_lst, token);
				current_pos = 0;
				current_token[0] = '\0';
				expmap[0] = '\0';
			}
			token = create_token("|", "0");
			update_tok_type(token, PIPE);
			add_token_to_list(&m->tok_lst, token);
			ptr++;
			continue ;
		}
		if (*ptr == '>')
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(&m->tok_lst, token);
				current_pos = 0;
				current_token[0] = '\0';
				expmap[0] = '\0';
			}
			if (*(ptr + 1) == '>')
			{
				token = create_token(">>", "00");
				ptr++;
			}
			else
				token = create_token(">", "0");
			update_tok_type(token, REDIRECTION);
			add_token_to_list(&m->tok_lst, token);
			ptr++;
			continue ;
		}
		if (*ptr == '<')
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(&m->tok_lst, token);
				current_pos = 0;
				current_token[0] = '\0';
				expmap[0] = '\0';
			}
			if (*(ptr + 1) == '<')
			{
				token = create_token("<<", "00");
				ptr++;
			}
			else
				token = create_token("<", "0");
			update_tok_type(token, REDIRECTION);
			add_token_to_list(&m->tok_lst, token);
			ptr++;
			continue ;
		}
		current_token[current_pos] = *ptr;
		expmap[current_pos] = in_single_quotes + '0';
		current_pos++;
		if (current_pos >= current_token_size)
		{
			current_token_size *= 2;
			new_token = ft_realloc(current_token, current_token_size);
			if (!new_token)
			{
				free(current_token);
				free(expmap);
				perror("realloc");
				exit(EXIT_FAILURE);
			}
			new_expmap = ft_realloc(expmap, current_token_size);
			if (!new_expmap)
			{
				free(current_token);
				free(expmap);
				perror("realloc");
				exit(EXIT_FAILURE);
			}
			current_token = new_token;
			expmap = new_expmap;
		}
		ptr++;
	}
	if (current_pos > 0)
	{
		current_token[current_pos] = '\0';
		expmap[current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(&m->tok_lst, token);
	}
	free(current_token);
	free(expmap);
}

//free(cur_content->expmap);
//cur_content->expmap = NULL;
void	expand_toklst(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		expand_token(m, m->exitcode, cur_content);
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	char	*tmpp;

	tmpp = remove_chars(m->prompt, "\n");
	m->prompt = tmpp;
	detect_lexing_errors(m);
	prompt_to_token(m);
	expand_toklst(m);
	afterbreakup(m);
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENLIST:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
}