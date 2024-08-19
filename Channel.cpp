/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:47 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 14:16:09 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Channel.hpp"
#include"features/Cmd.hpp"

std::vector<Chan> Channel::channel;

Channel::Channel()
{

}

Channel::~Channel()
{

}

void Channel::setChannel(Chan & add)
{
    channel.push_back(add);
}

std::vector<Chan> & Channel::getChannel()
{
    return channel;
}

Chan::Chan(std::string CN, const std::string & name, int & fd)
{
    toLowerString(CN);
    channelName = CN;
    members[name] = std::pair<int, bool>(fd, true);
    LookForUser[fd] = name;
    modesStat['i'] = std::pair<bool, std::string>(false, "");
    modesStat['t'] = std::pair<bool, std::string>(false, "");
    modesStat['k'] = std::pair<bool, std::string>(false, "");
    modesStat['o'] = std::pair<bool, std::string>(false, "");
    modesStat['l'] = std::pair<bool, std::string>(false, "");
    ChannelLimit = 0;
}

Chan::~Chan()
{

}

const std::string&           Chan::getChannelName() const
{
    return channelName;
}

const std::string&           Chan::getTopic() const
{
    return topic;
}

modeInfo&         Chan::getModes()
{
    return modesStat;
}

memberInfo&  Chan::getMembers()
{
    return members;
}

const unsigned int &    Chan::getLimit() const
{
    return ChannelLimit;
}

std::map<int, std::string>& Chan::getMembersFromFD()
{
    return LookForUser;
}

std::vector<std::string>& Chan::getInviteD()
{
    return InviteD;
}

void    Chan::setChannelName(const std::string & N)
{
    channelName = N; 
}

void    Chan::setTopic(const std::string & T)
{
    topic = T;
}

void    Chan::setMember(const std::string & mem, const int & fd)
{
    members[mem] = std::pair<int, bool>(fd, false);
    LookForUser[fd] = mem;
}

void    Chan::setModesStat(char index, bool flg, std::string param)
{
    modesStat[index] = std::pair<bool, std::string>(flg, param);
}

void    Chan::setChannelLimit(const int & L)
{
    ChannelLimit = L;
}

void    Chan::setInviteD(const std::string & name)
{
    InviteD.push_back(name);
}

void    Chan::setChannelOper(std::string & mmbr, bool Set)
{
    members[mmbr].second = Set;
}

void Chan::removeClient(std::string & nm)
{
    members.erase(members.find(nm));
    std::map<int, std::string>::iterator t = LookForUser.begin();
    std::map<int, std::string>::iterator te = LookForUser.end();
    for (std::map<int, std::string>::iterator i = t; i != te; i++)
    {
        if (nm == i->second)
        {
            LookForUser.erase(i);
            return ;
        }
    }
}

unsigned int    ChannelExist(std::vector<Chan>& CurrentChannels, std::string & Name, std::string & client)
{
    unsigned int c(0), sz = CurrentChannels.size();

    toLowerString(Name);
    if (!sz)
        throw std::runtime_error(": 403 " + client + " " + Name + " :No such channel\r\n");
    for (; c < sz; c++)
    {
        if (CurrentChannels[c].getChannelName() == Name)
            break ;
        if (c == sz - 1)
            throw std::runtime_error(": 403 " + client + " " + Name + " :No such channel\r\n");
    }
    return c;
}

void    IsInChannel(Chan& CurrentChannel, int fd, bool flg)
{
    std::map<int, std::string> &trav = CurrentChannel.getMembersFromFD();
    ClientInfos client = Client::getClient();

    if (trav.find(fd) == trav.end())
        throw std::runtime_error(": 442 " + client[fd].second.first + " :You're not on that channel\r\n");

    if (flg)
    {
        memberInfo meM = CurrentChannel.getMembers();
        if (!(meM[trav[fd]].second))
            throw std::runtime_error(": 482 " + trav[fd] + " :You're not channel operator\r\n");
    }
}

void    IsUserInChannel(Chan& CurrentChannel, std::string & Name, bool flg)
{
    memberInfo trav = CurrentChannel.getMembers();

    if (trav.find(Name) == trav.end())
    {
        if (flg)
            throw std::runtime_error(": 441 " + Name + " :They are not on that channel\r\n");
    }
    else
    {
        if (!flg)
            throw std::runtime_error(": 443 " + Name + " :is already on channel\r\n");
    }
}
