#include "minishell.h"

int	is_space(int c) // to libft
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r'
		|| c == '\f')
		return (1);
	return (0);
}

int	is_special_tkn(int c)
{
	if (c == '(' || c == ')' || c == '<' || c == '>' || c == '&' || c == '|')
	{
		return (1);
	}
	return (0);
}
void	*record_synt_err(char *tkn, t_shell *shell)
{
	if (shell->syntax_error == NULL)
		shell->syntax_error = tkn;
	return (NULL);
}

int	display_synt_err(t_shell *shell)
{
	error_msg("syntax error near unexpected token `", shell->syntax_error, "'",
		shell);
	shell->syntax_error = NULL;
	return (2);
}

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

t_tkn_type	get_braces(char *input, size_t *len)
{
	*len += 1;
	if (*input == '(')
		return (T_BRACE_START);
	return (T_BRACE_END);
}

t_tkn_type	get_ampersand(char *input, size_t *len, t_shell *shell)
{
	while (input[*len] == '&')
		(*len)++;
	if (*len == 2)
		return (T_AND);
	record_synt_err("&", shell);
	return (T_ERR);
}
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

t_tkn_type	get_word(char *input, size_t *len, t_shell *shell)
{
	char	c;
	bool	in_quote;

	in_quote = false;
	while (input[*len])
	{
		if (input[*len + 1] && input[*len] == '\\')
			*len += 1;
		else if (input[*len] == '\"' || input[*len] == '\'')
		{
			if (!in_quote)
			{
				c = input[*len];
				in_quote = true;
			}
			else if (in_quote && input[*len] == c)
				in_quote = false;
		}
		else if ((is_special_tkn(input[*len]) || is_space(input[*len]))
			&& !in_quote)
			break ;
		*len += 1;
	}
	if (in_quote)
		return (record_synt_err("unclosed quote", shell), T_ERR);
	return (T_TEXT);
}

static int	add_token(char *input, size_t len, t_tkn_type type, t_list **tokens,
		t_shell *shell)
{
	t_tkn	*token;

	token = create_token(type, input, len, shell);
	if (!token)
		return (EXIT_FAILURE);
	lstadd_back_tracked(token, tokens, COMMAND_TRACK, shell);
	return (EXIT_SUCCESS);
}

int	tokenize_input(char *input_str, t_list **tokens, t_shell *shell)
{
	size_t		i;
	size_t		token_length;
	t_tkn_type	token_type;

	i = 0;
	*tokens = NULL;
	while (input_str[i])
	{
		while (is_space(input_str[i]))
			i++;
		token_length = 0;
		token_type = get_tkn_type(input_str + i, &token_length, shell);
		if (token_type == T_TEXT)
			token_type = get_word(input_str + i, &token_length, shell);
		if (token_type == T_ERR)
			return (display_synt_err(shell));
		if (add_token(input_str + i, token_length, token_type, tokens,
				shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		i += token_length;
	}
	return (EXIT_SUCCESS);
}

t_tkn	*create_token(t_tkn_type tkn_type, char *tkn_value, size_t tkn_len,
		t_shell *shell)
{
	t_tkn	*new_tkn;

	new_tkn = calloc_tracked(1, sizeof(t_tkn), COMMAND_TRACK, shell);
	new_tkn->type = tkn_type;
	new_tkn->value = manage_memory(ft_substr(tkn_value, 0, tkn_len),
			COMMAND_TRACK, shell);
	return (new_tkn);
}

int	process_input(char *input_str, t_shell *shell)
{
	t_list	*tokens;
	t_ast	*parse_tree;
	int		result;

	tokens = NULL;
	parse_tree = NULL;
	result = tokenize_input(input_str, &tokens, shell);
	if (result != EXIT_SUCCESS || tokens == NULL)
		return (result);
	result = parse_tokens(tokens, &parse_tree, shell);
	if (result != EXIT_SUCCESS || parse_tree == NULL)
		return (result);
	// result = process_hdoc(parse_tree, shell);
	// if (result != EXIT_SUCCESS)
	// 	return (result);
	result = run_cmd(parse_tree, OP_COMPLETE, shell);
	return (result);
}
