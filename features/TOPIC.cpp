/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:42 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:51:30 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::TOPIC()
{
    std::string& nick = Client::getClient()[CurrentClientFD].second.first;
    unsigned int ChannelIndex, sz = checkTopic(data.second, nick);
    std::vector<Chan>&    CurrentChannels = Channel::getChannel();
    bool IsOper;

    try   
    {
        ChannelIndex = ChannelExist(CurrentChannels, data.second[0], nick);
        if ((CurrentChannels[ChannelIndex].getModes())['t'].first)
            IsOper = true;
        else
            IsOper = false;
        IsInChannel(CurrentChannels[ChannelIndex], CurrentClientFD, IsOper);
    }
    catch (const std::exception & e)
    {
        throw std::runtime_error(e.what());
    }

    if (sz == 1)
    {
        if ((CurrentChannels[ChannelIndex].getTopic()).empty())
            throw std::runtime_error(": 331 " + nick + " " + data.second[0] + " :No topic is set\r\n");
        else
            _send(CurrentClientFD, ": 332 " + nick + " " + data.second[0] + " :" + CurrentChannels[ChannelIndex].getTopic() + "\r\n");
    }
    else
    {
        if (CurrentChannels[ChannelIndex].getModes()['t'].first && !CurrentChannels[ChannelIndex].getMembers()[nick].second)
            throw std::runtime_error(": 482 " + nick + " " + data.second[0] + " :You're not channel operator\r\n");
        Channel::getChannel()[ChannelIndex].setTopic(data.second[1]);
		std::map<int, std::string> var = CurrentChannels[ChannelIndex].getMembersFromFD();
		std::map<int, std::string>::iterator it = var.begin();
		std::map<int, std::string>::iterator ite = var.end();

		for (std::map<int, std::string>::iterator t = it; t != ite; t++)
			serverReplyFormat(t->first, Client::getClient()[CurrentClientFD].second, data);
    }
}
