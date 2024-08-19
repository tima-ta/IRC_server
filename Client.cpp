/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:55 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:46:27 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"

ClientInfos Client::client;

Client::Client()
{

}

Client::~Client()
{

}

void Client::setClient(int fd, std::string nick, std::string user)
{
    client[fd] = std::pair<int, std::pair<std::string, std::string> >(0, std::pair<std::string, std::string>(nick, user));
}

ClientInfos & Client::getClient()
{
    return client;
}
