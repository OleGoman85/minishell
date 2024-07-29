/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_exec_sec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 12:07:19 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:21:31 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is a special shell token.
 *
 * This function checks if the given character is one of the special shell tokens
 * including '(', ')', '<', '>', '&', or '|'.
 *
 * @param c The character to check.
 * @return 1 if the character is a special token, otherwise 0.
 */
int	is_special_tkn(int c)
{
	if (c == '(' || c == ')' || c == '<' || c == '>' || c == '&' || c == '|')
	{
		return (1);
	}
	return (0);
}
/**
 * @brief Records a syntax error in the shell structure.
 *
 * If there is no syntax error currently recorded in the shell structure,
 * this function records the given token as the syntax error.
 *
 * @param tkn The token that caused the syntax error.
 * @param shell Pointer to the shell structure.
 * @return Always returns NULL.
 */

void	*record_synt_err(char *tkn, t_shell *shell)
{
	if (shell->syntax_error == NULL)
		shell->syntax_error = tkn;
	return (NULL);
}

/**
 * @brief Displays the recorded syntax error message.
 *
 * This function displays the syntax error message stored in the shell structure
 * and resets the syntax error field to NULL.
 *
 * @param shell Pointer to the shell structure.
 * @return Always returns 2 to indicate a syntax error.
 */
int	display_synt_err(t_shell *shell)
{
	error_msg("syntax error near unexpected token `", shell->syntax_error, "'",
		shell);
	shell->syntax_error = NULL;
	return (2);
}

/**
 * @brief Determines the token type of the input string.
 *
 * This function identifies the type of the token at the beginning of
 * the input string
 * and updates the length of the token. It handles various token types such as 
 * ampersands, pipes, redirects, and braces.
 *
 * @param input The input string to tokenize.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return The type of the token.
 */
t_tkn_type	get_tkn_type(char *input, size_t *len, t_shell *shell)
{
	if (*input == '&')
		return (get_ampersand(input, len, shell));
	else if (*input == '|')
		return (get_pipe(input, len, shell));
	else if (*input == '<' || *input == '>')
		return (get_redirect(input, *input, len, shell));
	else if (*input == '(' || *input == ')')
		return (get_braces(input, len));
	return (get_word(input, len, shell));
}
