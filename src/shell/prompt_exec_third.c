/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_exec_third.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 12:09:45 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:18:31 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Determines the type of brace token and updates the token length.
 *
 * This function identifies if the token is a starting
 * brace '(' or an ending brace ')'
 * and updates the length of the token.
 *
 * @param input The input string containing the brace token.
 * @param len Pointer to the length of the token.
 * @return T_BRACE_START if '(', T_BRACE_END if ')'.
 */
t_tkn_type	get_braces(char *input, size_t *len)
{
	*len += 1;
	if (*input == '(')
		return (T_BRACE_START);
	return (T_BRACE_END);
}

/**
 * @brief Determines the type of ampersand token and updates the token length.
 *
 * This function identifies if the token is a single '&' or a double '&&' 
 * and updates the length of the token. If the token is invalid, it records a 
 * syntax error.
 *
 * @param input The input string containing the ampersand token.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return T_AND if '&&', T_ERR if invalid token.
 */
t_tkn_type	get_ampersand(char *input, size_t *len, t_shell *shell)
{
	while (input[*len] == '&')
		(*len)++;
	if (*len == 2)
		return (T_AND);
	record_synt_err("&", shell);
	return (T_ERR);
}
/**
 * @brief Determines the type of pipe token and updates the token length.
 *
 * This function identifies if the token is a single '|' or a double '||'
 * and updates the length of the token. If the token is invalid, it records a 
 * syntax error.
 *
 * @param input The input string containing the pipe token.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return T_PIPE if '|', T_OR if '||', T_ERR if invalid token.
 */

t_tkn_type	get_pipe(char *input, size_t *len, t_shell *shell)
{
	while (input[*len] == '|')
		(*len)++;
	if (*len == 1)
		return (T_PIPE);
	if (*len == 2)
		return (T_OR);
	record_synt_err("||", shell);
	return (T_ERR);
}

/**
 * @brief Determines the type of redirect token and updates the token length.
 *
 * This function identifies if the token is an input '<', here-doc '<<',
 * output '>', or append '>>' and updates the length of the token.
 * If the token is invalid, it records a syntax error.
 *
 * @param input The input string containing the redirect token.
 * @param c The character representing the redirect type.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return The type of the redirect token.
 */
t_tkn_type	get_redirect(char *input, char c, size_t *len, t_shell *shell)
{
	while (input[*len] == c)
		*len += 1;
	if (c == '<' && *len == 1)
		return (T_INPUT);
	else if (c == '<' && *len == 2)
		return (T_HDOC);
	else if (c == '>' && *len == 1)
		return (T_OUTPUT);
	else if (c == '>' && *len == 2)
		return (T_APPEND);
	record_synt_err(input, shell);
	return (T_ERR);
}

/**
 * @brief Handles quotes and escape characters while determining token length.
 *
 * This function processes the input string to handle quotes and escape 
 * characters,
 * updating the token length accordingly.
 *
 * @param input The input string containing the token.
 * @param len Pointer to the length of the token.
 * @param in_quote Pointer to a boolean indicating if inside a quote.
 * @param c Pointer to the current quote character.
 */
void	handle_quotes_and_escapes(char *input, size_t *len,
		bool *in_quote, char *c)
{
	while (input[*len])
	{
		if (input[*len + 1] && input[*len] == '\\')
			*len += 1;
		else if (input[*len] == '\"' || input[*len] == '\'')
		{
			if (!*in_quote)
			{
				*c = input[*len];
				*in_quote = true;
			}
			else if (*in_quote && input[*len] == *c)
				*in_quote = false;
		}
		else if ((is_special_tkn(input[*len]) || ft_isspace(input[*len]))
			&& !*in_quote)
			break ;
		*len += 1;
	}
}
