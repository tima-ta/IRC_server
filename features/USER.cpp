/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:44 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:51:30 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::USER()
{
    try
    {
        ClientInfos save = Client::getClient();
        checkParamsUser(data.second, save[CurrentClientFD].second.first);
        std::string& nick = save[CurrentClientFD].second.first;
        std::string& username = data.second[0];

        if (save.find(CurrentClientFD) != save.end() && !(save[CurrentClientFD].second.second.empty()))
            throw std::runtime_error(": 462 " + nick + " :You may not reregister\r\n");
        Client::getClient()[CurrentClientFD].second.second = data.second[0];
        serverReplyFormat(CurrentClientFD, save[CurrentClientFD].second, data);
        _send(CurrentClientFD, ": 001 " + nick + " :Welcome to the Internet Relay Chat Network " + nick + "!~" + username + "@" + host +"\r\n");
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
}
