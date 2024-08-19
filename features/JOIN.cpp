/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:23 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 15:34:03 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::JOIN()
{
    unsigned int    sz;
    std::string          s, holder1, holder2;
    ClientInfos save = Client::getClient();
    std::string&     nick = save[CurrentClientFD].second.first;

    try
    {
        sz = checkChannel(data.second, nick);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }

    std::vector<Chan>& CurrentChannels = Channel::getChannel();
    unsigned int ChannelIndex;
    bool flg = 0;
    try
    {
        ChannelIndex = ChannelExist(CurrentChannels, data.second[0], nick);
    }
    catch(const std::exception& e)
    {
        flg = 1;
    }

    if (flg)
    {
        Chan obj(data.second[0], nick, CurrentClientFD);
        Channel::setChannel(obj);
        serverReplyFormat(CurrentClientFD, save[CurrentClientFD].second, data);
        _send(CurrentClientFD, ": MODE " + data.second[0] + " -iklot" + "\r\n");
        _send(CurrentClientFD, ": 353 " + nick + " @ " + data.second[0] + " :" + "@" + nick + "\r\n");
        _send(CurrentClientFD, ": 366 " + nick + " " + data.second[0] + " :End of /NAMES list.\r\n");
        return ;
    }

    Chan& currentChannel = CurrentChannels[ChannelIndex];
    modeInfo& keep = CurrentChannels[ChannelIndex].getModes();

    if ((keep['k'].first == true && sz == 1) || (sz == 2 && keep['k'].first == true && keep['k'].second != data.second[1]) || (sz == 2 && keep['k'].first == false))
        throw std::runtime_error(": 475 " + nick + " :Cannot join channel (+k)\r\n");
    if (keep['i'].first && find(currentChannel.getInviteD().begin(), currentChannel.getInviteD().end(), nick) == currentChannel.getInviteD().end())
        throw std::runtime_error(": 473 " + nick + " :This channel is invite only\r\n");
    if (find(currentChannel.getInviteD().begin(), currentChannel.getInviteD().end(), nick) == currentChannel.getInviteD().end() && (keep['l'].first && currentChannel.getMembersFromFD().size() >= currentChannel.getLimit()))
        throw std::runtime_error(": 471 " + nick + " :Channel is full\r\n");

    currentChannel.setMember(nick, CurrentClientFD);
    serverReplyFormat(CurrentClientFD, save[CurrentClientFD].second, data);
    memberInfo trav = currentChannel.getMembers();
    std::map<int, std::string> var = currentChannel.getMembersFromFD();
    std::map<int, std::string>::iterator it = var.begin();
    std::map<int, std::string>::iterator ite = var.end();
    for (std::map<int, std::string>::iterator t = it; t != ite; t++)
        if (t->first != CurrentClientFD)
            serverReplyFormat(t->first, save[CurrentClientFD].second, data);
    for (std::map<int, std::string>::iterator t = it; t != ite; t++)
    {
        if (t == it)
            s += nick + " ";
        if (t->second == nick)
            continue ;
        if (trav[t->second].second)
            s += "@";
        s += t->second + " ";
    }
    if (!currentChannel.getTopic().empty())
        _send(CurrentClientFD, ": 332 " + nick + " " + data.second[0] + " :" + currentChannel.getTopic() + "\r\n");
    modeInfo& ModE = currentChannel.getModes();
    modeInfo::iterator it0 = ModE.begin();
    modeInfo::iterator ite0 = ModE.end();
    for (modeInfo::iterator t0 = it0; t0 != ite0; t0++)
        if (t0->second.first)
            holder2.push_back(t0->first);
    if (!holder2.empty())
        holder1 = "+" + holder2;
    holder2.clear();
    for (modeInfo::iterator t0 = it0; t0 != ite0; t0++)
        if (!t0->second.first)
            holder2.push_back(t0->first);
    if (!holder2.empty())
        holder1 += "-" + holder2;
    _send(CurrentClientFD, ": MODE " + data.second[0] + " " + holder1 + "\r\n");
    _send(CurrentClientFD, ": 353 " + nick + " @ " + data.second[0] + " :" + s + "\r\n");
    _send(CurrentClientFD, ": 366 " + nick + " " + data.second[0] + " :End of /NAMES list.\r\n");

    std::vector<std::string>& sv =  currentChannel.getInviteD();
    if (keep['i'].first)
        sv.erase(find(sv.begin(), sv.end(), nick));
}
