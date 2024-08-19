/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araqioui <araqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 09:50:49 by araqioui          #+#    #+#             */
/*   Updated: 2023/12/12 09:55:38 by araqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string const &port) : Address(NULL)
{
	
	Addrinfo	hints;
	Pollfd		help;
	int			error;

	memset(&hints, '\0', sizeof(struct addrinfo));

	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((error = getaddrinfo(NULL, port.c_str(), &hints, &Address)))
	{
		std::cout << COLOR_RED << gai_strerror(error) << COLOR_RESET << std::endl;
		throw (-1);
	}
	help.fd = socket(Address->ai_family, Address->ai_socktype, Address->ai_protocol);
	if (help.fd < 0)
	{
		perror(COLOR_RED "Socket " COLOR_RESET);
		throw -1;
	}
	help.events = POLLIN;
	help.revents = 0;
	Sockets.push_back(help);
	this->NonBlockMode();
}

Server::~Server(void)
{
	size_t	i = 0;

	while (i < Sockets.size())
		close(Sockets[i++].fd);
	if (Address)
		freeaddrinfo(Address);
}

void	Server::Revents(void)
{
	size_t	i = 0;

	while (i < Sockets.size())
		Sockets[i++].revents = 0;
}

void	Server::NonBlockMode(void)
{
	if (fcntl(Sockets[0].fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror(COLOR_RED "fcntl " COLOR_RESET);
		throw (-1);
	}
}

int Server::getFD(size_t i)
{
	return (Sockets[i].fd);
}

short	Server::getRevents(size_t i)
{
	return (Sockets[i].revents);
}

std::string	&Server::getRequest(size_t i)
{
	return (Request[i - 1]);
}

size_t	Server::getSize(void) const
{
	return (Sockets.size());
}

void	Server::SBind(void)
{
	int	reuseaddr = 1;

	if (setsockopt(Sockets[0].fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)))
	{
		perror(COLOR_RED "setsockopt " COLOR_RESET);
		throw (-1);
	}

	if (bind(Sockets[0].fd, Address->ai_addr, Address->ai_addrlen) < 0)
	{
		perror(COLOR_RED "Bind " COLOR_RESET);
		throw (-1);
	}
}

void	Server::SListen(void)
{
	if (listen(Sockets[0].fd, SOMAXCONN) < 0)
	{
		perror(COLOR_RED "Listen " COLOR_RESET);
		throw (-1);
	}
}

int	Server::SPoll(void)
{
	this->Revents();
	return (poll(Sockets.data(), Sockets.size(), -1));
}

void	Server::SAccept(void)
{
	Sockaddr_in	inData;
	socklen_t	sizeStruct = sizeof(Sockaddr_in);
	int			newSocket;
	Pollfd		help;

	newSocket = accept(Sockets[0].fd, reinterpret_cast<struct sockaddr *>(&inData), &sizeStruct);
	if (newSocket < 0)
	{
		perror(COLOR_RED "Accept " COLOR_RESET);
		throw (-1);
	}
	help.fd = newSocket;
	std::string	IPaddr(inet_ntoa(inData.sin_addr));
	help.events = POLLIN;
	help.revents = 0;
	Sockets.push_back(help);
	SockAddrInfo.push_back(IPaddr);
	Request.push_back("");
}

void	Server::SClose(size_t i)
{
	close(Sockets[i].fd);
	Sockets.erase(Sockets.begin() + i);
	SockAddrInfo.erase(SockAddrInfo.begin() + i - 1);
	Request.erase(Request.begin() + i - 1);
}

std::string const	&Server::getIP(size_t i) const
{
	return (SockAddrInfo[i - 1]);
}
