/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:26 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 16:26:12 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::KICK()
{
    std::string& nick = Client::getClient()[CurrentClientFD].second.first;
    unsigned int sz = data.second.size();
    if (sz != 2 && sz != 3)
        throw std::runtime_error(": 461 " + nick + " :KICK Not enough parameters\r\n");
    if (sz == 3 && ValidString(data.second[2]) < 0)
        throw std::runtime_error(": 461 " + nick + " :Non valid character(s)\r\n");

    unsigned int    ChannelIndex;
    std::vector<Chan>&    CurrentChannels = Channel::getChannel();
    try
    {
        ChannelIndex = ChannelExist(CurrentChannels, data.second[0], nick);
        IsInChannel(CurrentChannels[ChannelIndex], CurrentClientFD, true);
        memberInfo& keep = CurrentChannels[ChannelIndex].getMembers();
        if (keep.find(data.second[1]) == keep.end())
            throw std::runtime_error(": 441 " + data.second[1] + " " + data.second[0] + " :They aren't on that channel\r\n");
    }
    catch (const std::exception & e)
    {
        throw std::runtime_error(e.what());
    }

    std::map<int, std::string> var = CurrentChannels[ChannelIndex].getMembersFromFD();
    std::map<int, std::string>::iterator it = var.begin();
    std::map<int, std::string>::iterator ite = var.end();

    for (std::map<int, std::string>::iterator t = it; t != ite; t++)
            serverReplyFormat(t->first, Client::getClient()[CurrentClientFD].second, data);

    CurrentChannels[ChannelIndex].removeClient(data.second[1]);
}
