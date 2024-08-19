/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:21 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:51:30 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::INVITE()
{
	unsigned int    ChannelIndex;
	std::vector<Chan>& CurrentChannels = Channel::getChannel();
	ClientInfos CurrentClients = Client::getClient();
	std::string &nick = CurrentClients[CurrentClientFD].second.first;

	if (data.second.size() != 2)
			throw std::runtime_error(": 461 " + nick + " :INVITE Not enough parameters\r\n");

	try
		{
			ChannelIndex = ChannelExist(CurrentChannels, data.second[1], nick);
			IsInChannel(CurrentChannels[ChannelIndex], CurrentClientFD, false);
			if (CurrentClients.find(CurrentClientFD) == CurrentClients.end())
				throw std::runtime_error(": 401 " + data.second[0] + " :No such nick\r\n");
			IsUserInChannel(CurrentChannels[ChannelIndex], data.second[0], false);
	}
	catch (const std::exception & e)
	{
			throw std::runtime_error(e.what());
	}
	if (CurrentChannels[ChannelIndex].getModes()['i'].first && !CurrentChannels[ChannelIndex].getMembers()[nick].second)
		throw std::runtime_error(": 482 " + nick + " :You're not channel operator\r\n");

	CurrentChannels[ChannelIndex].setInviteD(data.second[0]);
	ClientInfos clientFD = Client::getClient();
	ClientInfos::iterator it = clientFD.begin();
	ClientInfos::iterator ite = clientFD.end();
	ClientInfos::iterator t = it;
	for (; t != ite; t++)
		if (data.second[0] == t->second.second.first)
			break;
	serverReplyFormat(CurrentClientFD, CurrentClients[CurrentClientFD].second, data);
	serverReplyFormat(t->first, CurrentClients[CurrentClientFD].second, data);
}
