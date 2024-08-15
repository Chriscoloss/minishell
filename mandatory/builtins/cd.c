/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:10:58 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 17:43:52 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// Function to handle 'cd -' case
void    cd_oldpwd(t_envlst **envp)
{
	char	*oldpwd;
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
		return (perror("getcwd"));
	oldpwd = my_getenv("OLDPWD", *envp);
	if (!oldpwd)
		return (free(current_pwd), (void)printf(ERR_OLDPWD));
	if (chdir(oldpwd) == -1)
	{
		free(current_pwd);
		printf("🍕🚀🌈🦄🍺: cd: %s: No such file or directory\n", oldpwd);
		return ;
	}
	printf("%s\n", oldpwd);
	change_env(&(*envp), "OLDPWD", current_pwd, 0);
	free(current_pwd);
	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
		return (perror("getcwd"));
	change_env(&(*envp), "PWD", current_pwd, 0);
	free(current_pwd);
}

// Function to handle changing to a specific path
void	cd_path(char *path, t_envlst **envp)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"));
	if (chdir(path) == -1)
	{
		printf("🍕🚀🌈🦄🍺: cd: %s: No such file or directory\n", path);
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		free(oldpwd);
		return ;
	}
	change_env(&(*envp), "OLDPWD", oldpwd, 0);
	change_env(&(*envp), "PWD", pwd, 0);
	free(oldpwd);
	free(pwd);
}

// Helper function to change the current working directory to the home directory
void	cd_home(t_envlst **envp)
{
	char	*home;
	char	*oldpwd;
	char	*pwd;

	home = getenv("HOME");
	if (!home)
		return ((void)printf("cd: HOME not set\n"));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"));
	if (chdir(home) == -1)
	{
		printf("🚀: cd: %s: No such file or directory\n", home);
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), free(oldpwd));
	change_env(&(*envp), "OLDPWD", oldpwd, 0);
	change_env(&(*envp), "PWD", pwd, 0);
	free(oldpwd);
}

// Main function that changes the current working directory
void	ft_cd(int argc, char **argv, t_envlst **envp)
{
	if (argc == 1 || !ft_strcmp(argv[1], "~") || !ft_strcmp(argv[1], "--"))
	{
		if (getenv("HOME"))
			return (cd_home(envp));
		else
			return ((void)printf("🍕🚀🌈🦄🍺: cd: HOME not set\n"));
	}
	if (argc == 2 && !ft_strcmp(argv[1], "-"))
		cd_oldpwd(envp);
	else if (argc == 2)
		cd_path(argv[1], envp);
}
