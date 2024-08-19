/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:58 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:46:27 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include"MainHeader.hpp"

typedef std::map<int, std::pair<int , std::pair<std::string, std::string> > > ClientInfos;

class Client
{
private:
        static ClientInfos client;

public:
        Client();
        ~Client();

        static void setClient(int fd, std::string nick, std::string user);
        static ClientInfos & getClient();
};
