/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 13:08:36 by araqioui          #+#    #+#             */
/*   Updated: 2023/12/12 10:38:30 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <signal.h>
# include <iostream>
# include <vector>
# include <map>
# include <list>
# include <sstream>
# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RESET "\033[0m"

void	startServer(char *port, char *pswd);

void    Bot(std::string const &request);
