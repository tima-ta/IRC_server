/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araqioui <araqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 10:06:32 by araqioui          #+#    #+#             */
/*   Updated: 2023/12/12 09:51:53 by araqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainHeader.hpp"

static int	Port(char *str)
{
	std::stringstream	ss(str);
	int				port;
	int				i = -1;

	if (!str[i + 1])
		return (0);
	while (str[++i])
	{
		if (!isdigit(str[i]))
		{
			std::cout << COLOR_RED << "Error: Bad port format" << COLOR_RESET << std::endl;
			return (0);
		}
	}
	ss >> port;
	if (port < 1024 || port > 65535)
	{
		std::cout << COLOR_RED << "Error: Bad port format" << COLOR_RESET << std::endl;
		return (0);
	}
	return (1);
}

static int	Password(char *str)
{
	int	i = -1;

	if (!str[i + 1])
	{
		std::cout << COLOR_RED << "Error: Bad pswd format" << COLOR_RESET << std::endl;
		return (0);
	}
	while (str[++i])
	{
		if (!isalpha(str[i]))
		{
			std::cout << COLOR_RED << "Error: Bad pswd format" << COLOR_RESET << std::endl;
			return (0);
		}
	}
	return (1);
}

void	signalHandler(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		std::cout << COLOR_GREEN << "\rExit IRCSERVER ...." << COLOR_RESET << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		if (Port(av[1]) && Password(av[2]))
		{
			try {
				signal(SIGINT, signalHandler);
				signal(SIGQUIT, signalHandler);
				startServer(av[1], av[2]);
			}
			catch(int){}
		}
	}
	else
		std::cout << COLOR_RED << "Error: NB of args" << COLOR_RESET << std::endl;
	return (1);
}
