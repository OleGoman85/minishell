
#include "minishell.h"

/**
 * @brief Выполняет узел абстрактного синтаксического дерева (AST).
 *
 * Эта функция обрабатывает и выполняет узел AST в зависимости от его типа.
 * Возможные типы узлов включают логические операции, конвейеры, подкоманды,
 * перенаправления и команды. Функция вызывает соответствующие функции
 * для выполнения каждого типа узла.
 *
 * @param ast_node Указатель на узел AST для выполнения.
 * @param op_status Статус операции, определяющий,
	следует ли завершить работу после выполнения.
 * @param shell Указатель на структуру оболочки для управления состоянием и памятью.
 * @return Статус выполнения узла (код завершения).
 */
int	run_cmd(t_ast *ast_node, t_op_status op_status, t_shell *shell)
{
	int	result;

	result = EXIT_SUCCESS;
	if (ast_node == NULL)
		return (result);
	if (ast_node->node_type == LOGIC)
		result = process_logic(&ast_node->node_content.logic, shell);
	else if (ast_node->node_type == PIPE)
		result = process_pipeline(ast_node, shell);
	else if (ast_node->node_type == BRACE)
		result = process_brace(&ast_node->node_content.brace, shell);
	else if (ast_node->node_type == REDIR || ast_node->node_type == CMD)
	{
		resolve_ast_content(ast_node, shell);
		if (ast_node->node_type == REDIR)
			result = process_redirection(&ast_node->node_content.redir, shell);
		else if (ast_node->node_type == CMD)
			result = process_cmd(&ast_node->node_content.cmd, op_status,
					shell);
	}
	else
		exit_on_error("run", "unexpected node type", EXIT_FAILURE, shell);
	if (op_status == OP_TERMINATE)
		exit(result);
	return (result);
}


/**
 * @brief Обрабатывает завершение дочернего процесса, возвращая соответствующий
 *        статус.
 *
 * Эта функция проверяет статус завершения дочернего процесса. Если процесс
 * завершился нормально, возвращается его статус выхода. Если процесс был
 * прерван сигналом, функция записывает сообщение об этом и, если необходимо,
 * добавляет новую строку в вывод. В случае прерывания сигнала SIGQUIT или
 * SIGINT добавляется новая строка, если она еще не была добавлена.
 *
 * @param child_status Статус завершения дочернего процесса.
 * @param printed_newline Указатель на флаг, указывающий, была ли добавлена
 *                        новая строка.
 * @param shell Указатель на структуру оболочки для управления памятью и
 *              выводом.
 * @return Статус завершения процесса. В случае нормального завершения
 *         возвращается его статус выхода. В случае прерывания сигнала
 *         возвращается 128 + номер сигнала. Если статус не соответствует ни
 *         одному из этих условий, возвращается EXIT_FAILURE.
 */
int	handle_exit_signal(int child_status, bool *printed_newline, t_shell *shell) // переделать страхолюдину
{
	int		exit_signal;
	bool	need_newline;

	if (!WIFEXITED(child_status))
	{
		if (WIFSIGNALED(child_status))
		{
			exit_signal = WTERMSIG(child_status);
			need_newline = (exit_signal == SIGQUIT || exit_signal == SIGINT);
			if (exit_signal == SIGQUIT)
				write_and_track("Quit with SIGQUIT (3)", STDERR_FILENO, shell);
			if (need_newline)
			{
				if (!printed_newline || !*printed_newline)
				{
					write_and_track("\n", STDERR_FILENO, shell);
					if (printed_newline)
						*printed_newline = true;
				}
			}
			return (128 + exit_signal);
		}
		return (EXIT_FAILURE);
	}
	return (WEXITSTATUS(child_status));
}


/**
 * @brief Выполняет команду,
	вызывая встроенные функции или создавая новый процесс.
 *
 * Эта функция выполняет команду, используя встроенные функции оболочки или
 * создавая новый процесс для внешней команды. Если команда встроенная, она
 * вызывается напрямую. Если команда требует нового процесса и терминальный
 * статус установлен как OP_TERMINATE,
	функция использует execute_program для выполнения
 * команды в текущем процессе. В противном случае, функция создает новый процесс
 * с помощью create_process, устанавливает стандартные сигналы и выполняет команду в
 * дочернем процессе.
 *
 * @param cmd Указатель на структуру команды t_cmd,
	содержащую аргументы команды.
 * @param term_status Статус выполнения команды,
	определяющий завершение процесса.
 * @param shell Указатель на структуру t_shell, содержащую параметры и состояние
 *        оболочки.

	* @return Возвращает статус выполнения команды: статус выхода встроенной функции,
 *         код выхода дочернего процесса или код ошибки.
 */
int	process_cmd(t_cmd *cmd, t_op_status term_status, t_shell *shell)
{
	int			exec_status;
	t_bltn_func	function;

	if (!cmd->cmd_args[0])
		return (EXIT_SUCCESS);
	if ((function = fetch_builtin_cmd(cmd->cmd_args[0])))
		return (function(cmd, shell));
	if (term_status == OP_TERMINATE)
	{
		execute_program(find_executable_path(cmd->cmd_args[0], shell), cmd->cmd_args,
			create_ev_array(shell), shell);
		return (EXIT_FAILURE);
	}
	if (create_process(shell) == 0)
	{
		shell->is_main = false;
		signals_default();
		execute_program(find_executable_path(cmd->cmd_args[0], shell), cmd->cmd_args,
			create_ev_array(shell), shell);
		exit(EXIT_FAILURE);
	}
	wait_for_child(&exec_status, shell);
	return (handle_exit_signal(exec_status, NULL, shell));
}

/**
 * @brief Выполняет логическую операцию AND или OR между двумя командами.
 *
 * Эта функция выполняет первую команду и сохраняет ее статус. В зависимости
 * от логической операции (AND или OR) и статуса выполнения первой команды,
 * она может выполнить вторую команду и вернуть ее статус.
 *
 * @param logic Указатель на структуру логической операции,
	содержащую две команды и тип операции.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Код завершения выполнения последней команды.
 */
int	process_logic(t_logic *logic, t_shell *shell)
{
	int	cmd_status;

	cmd_status = run_cmd(logic->first, OP_COMPLETE, shell);
	shell->prev_cmd_status = cmd_status;
	if ((logic->logic_op == T_AND && cmd_status == EXIT_SUCCESS)
		|| (logic->logic_op == T_OR && cmd_status != EXIT_SUCCESS))
		cmd_status = run_cmd(logic->second, OP_COMPLETE, shell);
	return (cmd_status);
}


/**
 * @brief Выполняет команду в скобках (brace) в отдельном процессе.
 *
 * Эта функция создает дочерний процесс с помощью create_process и выполняет в нем 
 * команду, заключенную в скобки. Родительский процесс ожидает завершения 
 * дочернего процесса и обрабатывает его статус завершения.
 *
 * @param brace Указатель на структуру brace, содержащую команду.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Код завершения выполнения команды.
 */
int process_brace(t_brace *brace, t_shell *shell)
{
    int cmd_status;
    bool is_parent;

    cmd_status = EXIT_FAILURE;
    is_parent = (create_process(shell) != 0);
    if (is_parent)
    {
        wait_for_child(&cmd_status, shell);
        cmd_status = handle_exit_signal(cmd_status, NULL, shell);
    }
    else
    {
        shell->is_main = false;
        signals_default();
        run_cmd(brace->command, OP_TERMINATE, shell);
    }
    return cmd_status;
}
