/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 09:48:32 by araqioui          #+#    #+#             */
/*   Updated: 2023/12/12 10:37:08 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "MainHeader.hpp"
#include"Server.hpp"

// #define SIZE			512
#define BUFFER_SIZE		2028

typedef struct addrinfo			Addrinfo;
typedef struct pollfd			Pollfd;
typedef struct sockaddr_in		Sockaddr_in;
typedef struct hostent			Hostent;

class Server {
	private:
		std::vector<Pollfd>			Sockets;
		std::vector<std::string>	SockAddrInfo;
		std::vector<std::string>	Request;
		Addrinfo					*Address;
		void	NonBlockMode(void);

	public:
		Server(std::string const &port);
		~Server(void);
		int					getFD(size_t i);
		short				getRevents(size_t i);
		std::string				&getRequest(size_t i);
		void				Revents(void);
		void				SBind(void);
		void				SListen(void);
		size_t				getSize(void) const;
		int					SPoll(void);
		void				SAccept(void);
		void				SClose(size_t i);
		std::string const	&getIP(size_t i) const;
};
