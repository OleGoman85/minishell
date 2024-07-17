
#include "minishell.h"

/**
 * @brief Выполняет последовательность команд, связанных в конвейер.
 *
 * Функция создает конвейер, выполняет каждую команду из списка команд и
 * ожидает завершения всех дочерних процессов. Она использует файловые
 * дескрипторы для передачи данных между командами в конвейере.
 *
 * @param pipe_lst Указатель на список команд для выполнения в конвейере.
 * @param shell Указатель на структуру оболочки для управления процессами.
 * @return Код завершения последней команды в конвейере.
 */
int	execute_command_chain(t_list *pipe_lst, t_shell *shell)
{
	int		pipe_fds[2];
	int		last_pipe_fd;
	pid_t	last_exec_pid;
	t_list	*current;

	last_exec_pid = 0;
	last_pipe_fd = -1;
	current = pipe_lst;
	while (current)
	{
		create_pipe(pipe_fds, shell);
		last_exec_pid = run_pipe_cmd(current, last_pipe_fd, pipe_fds,
				shell);
		if (last_pipe_fd != -1)
			close_file(last_pipe_fd, shell);
		close_file(pipe_fds[PIPE_OUTPUT], shell);
		last_pipe_fd = pipe_fds[PIPE_INPUT];
		current = current->next;
	}
	return (collect_child_statuses(last_exec_pid, ft_lstsize(pipe_lst), shell));
}
//переделать
/**
 * @brief Собирает и возвращает итоговый статус выхода всех процессов конвейера.
 *
 * Функция ожидает завершения всех процессов конвейера, обрабатывает их
 * статус выхода и возвращает статус выхода последнего выполняемого процесса.
 * При обработке статусов сигналов обеспечивается вывод дополнительных
 * сообщений и перевод строки при необходимости.
 *
 * @param last_exec_pid PID последнего выполняемого процесса.
 * @param pipe_count Количество процессов в конвейере.
 * @param shell Указатель на структуру shell для управления памятью и сигналами.
 * @return Итоговый статус выхода последнего выполняемого процесса.
 */
int	collect_child_statuses(pid_t last_exec_pid, int pipe_count, t_shell *shell)
{
	bool	need_newline;
	int		sig_status;
	int		exit_status;
	int		result;
	pid_t	current_pid;

	need_newline = false;
	result = 0;
	while (pipe_count > 0)
	{
		current_pid = wait_for_child(&sig_status, shell);
		exit_status = handle_exit_signal(sig_status, &need_newline, shell);
		if (current_pid == last_exec_pid)
			result = exit_status;
		pipe_count--;
	}
	return (result);
}

/**
 * @brief Обрабатывает и выполняет цепочку команд в конвейере.
 *
 * Эта функция создает список команд из переданного AST узла и
 * вызывает функцию для выполнения цепочки команд. Возвращает
 * статус выполнения конвейера.
 *
 * @param ast_node Указатель на AST узел, представляющий конвейер.
 * @param shell Указатель на структуру оболочки, используемую для
 * управления памятью и состоянием.
 * @return Статус выполнения конвейера.
 */
int	process_pipeline(t_ast *ast_node, t_shell *shell)
{
	t_list	*commands;
	t_ast	*current;
	int		status;

	commands = NULL;
	current = ast_node;
	while (current && current->node_type == PIPE)
	{
		lstadd_front_tracked(current->node_content.pipe.output_side, &commands,
			COMMAND_TRACK, shell);
		current = current->node_content.pipe.input_side;
	}
	if (current)
		lstadd_front_tracked(current, &commands, COMMAND_TRACK, shell);
	status = execute_command_chain(commands, shell);
	return (status);
}

/**
 * @brief Выполняет одну команду в конвейере (pipeline).
 *
 * Эта функция выполняет одну команду в конвейере, создавая новый процесс
 * с помощью create_process. В дочернем процессе перенаправляются потоки ввода/вывода
 * для создания пайпов, затем выполняется команда. Родительский процесс
 * возвращает PID дочернего процесса.
 *
 * @param pipe_lst Указатель на список команд конвейера.
 * @param last_pipe_fd Дескриптор чтения из предыдущего пайпа.
 * @param pipe_fds Массив из двух дескрипторов для текущего пайпа.
 * @param shell Указатель на структуру shell для управления состоянием.
 * @return PID дочернего процесса, выполняющего команду.
 */
pid_t	run_pipe_cmd(t_list *pipe_lst, int last_pipe_fd,
		int pipe_fds[2], t_shell *shell)
{
	pid_t	last_exec_pid;
	t_ast	*current_ast;
	bool	is_not_last_command;

	current_ast = (t_ast *)pipe_lst->content;
	last_exec_pid = create_process(shell);
	if (last_exec_pid == 0)
	{
		shell->is_main = false;
		signals_default();
		if (last_pipe_fd != -1)
		{
			duplicate_fd(last_pipe_fd, STDIN_FILENO, shell);
			close_file(last_pipe_fd, shell);
		}
		is_not_last_command = (pipe_lst->next != NULL);
		if (is_not_last_command)
			duplicate_fd(pipe_fds[PIPE_OUTPUT], STDOUT_FILENO, shell);
		close_file(pipe_fds[PIPE_INPUT], shell);
		close_file(pipe_fds[PIPE_OUTPUT], shell);
		run_cmd(current_ast, OP_TERMINATE, shell);
	}
	return (last_exec_pid);
}
