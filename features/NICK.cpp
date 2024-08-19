/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:32 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 16:25:57 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::NICK()
{
    ClientInfos save = Client::getClient();
    try
    {
        checkParamsNick(data.second);
        ClientInfos::iterator it = save.begin();
        ClientInfos::iterator ite = save.end();
        for (ClientInfos::iterator t = it; t != ite; t++)
        {
            if (CurrentClientFD != t->first && ((t->second).second.first == data.second[0]))
                throw std::runtime_error(": 433 " + (t->second).second.second + " :Nickname is already in use\r\n");
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }

    std::string& nick = Client::getClient()[CurrentClientFD].second.first;
    if (!nick.empty())
    {
        std::vector<Chan>& channel = Channel::getChannel();
        for (unsigned int i(0); i < channel.size(); i++)
        {
            memberInfo& saving = channel[i].getMembers();
            if (saving.find(nick) != saving.end())
            {
                saving[data.second[0]] = saving.find(nick)->second;
                saving.erase(nick);
                channel[i].getMembersFromFD()[CurrentClientFD] = data.second[0];
                std::map<int, std::string> keep = channel[i].getMembersFromFD();
                std::map<int, std::string>::iterator ic = keep.begin();
                std::map<int, std::string>::iterator ice = keep.end();
                for (std::map<int, std::string>::iterator c = ic; c != ice; c++)
                        serverReplyFormat(c->first, Client::getClient()[CurrentClientFD].second, data);
                                memberInfo ss = channel[i].getMembers();
            }
        }
    }
    Client::getClient()[CurrentClientFD].second.first = data.second[0];
}
