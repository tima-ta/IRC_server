/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:39 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:45:36 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::deleteFromChn(const std::string & nick, const unsigned int & index)
{
    std::vector<Chan>& channel = Channel::getChannel();
    std::map<int, std::string> memberS = channel[index].getMembersFromFD();
    std::map<int, std::string>::iterator ic = memberS.begin();
    std::map<int, std::string>::iterator ice = memberS.end();
    for (std::map<int, std::string>::iterator c = ic; c != ice; c++)
        serverReplyFormat(c->first, Client::getClient()[CurrentClientFD].second, data);
    channel[index].getMembersFromFD().erase(CurrentClientFD);
    channel[index].getMembers().erase(nick);
    if (channel[index].getMembers().size() == 0)
    {
        std::vector<Chan>::iterator it = channel.begin();
        std::vector<Chan>::iterator ite = channel.end();
        std::vector<Chan>::iterator t = it;
        for (unsigned int i = 0; t != ite; t++, i++)
           if (i == index)
                break ; 
        Channel::getChannel().erase(t);
    }
}

void    Cmd::QUIT()
{
    std::string & nick = Client::getClient()[CurrentClientFD].second.first;
    std::vector<Chan>& channel = Channel::getChannel();
    for (unsigned int i(0); i < channel.size(); i++)
        if (channel[i].getMembers().find(nick) != channel[i].getMembers().end())
            deleteFromChn(nick, i);
    Client::getClient().erase(CurrentClientFD);
}

void    Cmd::PART()
{
    std::vector<Chan>& channel = Channel::getChannel();
    for (unsigned int i(0); i < channel.size(); i++)
        if (channel[i].getChannelName() == data.second[0])
            deleteFromChn(Client::getClient()[CurrentClientFD].second.first, i);
}
