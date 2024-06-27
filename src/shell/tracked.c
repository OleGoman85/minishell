
#include "minishell.h"

void	*manage_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell)
{
	if (!mem_ptr)
		exit_on_error("Memory allocation", strerror(errno), EXIT_FAILURE, shell);
	if (tracker != UNTRACKED)
		track_memory(mem_ptr, tracker, shell);
	return (mem_ptr);
}

void	*calloc_tracked(size_t num_elements, size_t el_size, 
						t_mem_trackers tracker, t_shell *shell)
{
	return (manage_memory(ft_calloc(num_elements, el_size), tracker, shell));
}

void	track_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell)
{
	t_list	*mem_entry;

	mem_entry = ft_lstnew(mem_ptr);
	alloc_check(mem_entry, mem_ptr, shell);
	if (!mem_entry)
	{
		free(mem_ptr);
		exit_on_error("Memory allocation", strerror(errno), EXIT_FAILURE, shell);
	}
	ft_lstadd_front(&shell->mem_tracker[tracker], mem_entry);
}

char	*strjoin_tracked(char const *s1, char const *s2, t_mem_trackers tracker,
	t_shell *shell)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	
	return (manage_memory(joined, tracker, shell));
}

char	**split_tracked(char const *str, char c, t_mem_trackers tracker, 
						t_shell *shell)
{
	char	**split_arr;
	int		i;

	i = 0;
	split_arr = ft_split(str, c);
	manage_memory(split_arr, tracker, shell);
	while (split_arr && split_arr[i])
		manage_memory(split_arr[i++], tracker, shell);
	return (split_arr);
}

char	*strdup_tracked(char const *str, t_mem_trackers tracker, t_shell *shell)
{
	char	*dup;

	dup = ft_strdup(str);
	return (manage_memory(dup, tracker, shell));
}

ssize_t	write_ext(const char *str, int fd, t_shell *shell)
{
	ssize_t	result;

	if (!str)
		exit_on_sys_error("write", EINVAL, shell);
	result = write(fd, str, strlen(str));
	if (result == -1)
		exit_on_sys_error("write", errno, shell);
	return (result);
}

void	lstadd_front_tracked(void *content, t_list **lst, t_mem_trackers tracker,
		t_shell *shell)
{
	t_list	*node;

	node = ft_lstnew(content);
	alloc_check(node, content, shell);
	ft_lstadd_front(lst, node);
	track_memory(node, tracker, shell);
}

void	lstadd_back_tracked(void *content, t_list **lst, t_mem_trackers tracker,
		t_shell *shell)
{
	t_list	*node;

	node = ft_lstnew(content);
	alloc_check(node, content, shell);
	ft_lstadd_back(lst, node);
	track_memory(node, tracker, shell);
}

void	alloc_check(t_list *node, void *mem_ptr, t_shell *shell)
{
	if (!node)
	{
		free(mem_ptr);
		exit_on_error("Memory allocation", strerror(errno), EXIT_FAILURE, shell);
	}
}


void	del_node(t_list **target, t_list **lst_head,
		void (*free_data)(void *), bool should_free)
{
	t_list	*deleting;

	if (!target || !*target || !lst_head)
		return ;
	deleting = *target;
	if (deleting->prev)
		deleting->prev->next = deleting->next;
	else
		*lst_head = deleting->next;
	if (deleting->next)
		deleting->next->prev = deleting->prev;
	if (should_free)
		ft_lstdelone(deleting, free_data);
}
