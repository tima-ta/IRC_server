/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:52 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:54:00 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include"Client.hpp"

typedef std::map<std::string , std::pair<int, bool> > memberInfo;
typedef std::map<char, std::pair<bool, std::string> > modeInfo;

class Chan
{
private:
        std::string                     channelName;
        std::string                     topic;
        memberInfo                      members;
        std::map<int, std::string>      LookForUser;  
        modeInfo                        modesStat;
        unsigned int                    ChannelLimit;
        std::vector<std::string>        InviteD;

public:
        Chan(std::string CN, const std::string & name, int & fd);
        ~Chan();

        const std::string&           getChannelName() const;
        const std::string&           getTopic() const;
        const unsigned int &    getLimit() const;
        std::map<int, std::string>&       getMembersFromFD();
        modeInfo&               getModes();
        memberInfo&             getMembers();
        std::vector<std::string>&         getInviteD();

        void    setChannelName(const std::string & N);
        void    setChannelLimit(const int & L);
        void    setTopic(const std::string & T);
        void    setMember(const std::string & mem, const int & fd);
        void    setModesStat(char index, bool flg, std::string param);
        void    setChannelOper(std::string & mmbr, bool Set);
        void    setInviteD(const std::string & name);
        void    removeClient(std::string & nm);
};

class Channel
{
private:
        static std::vector<Chan> channel;

public:
        Channel();
        ~Channel();

        static void setChannel(Chan & add);
        static std::vector<Chan> & getChannel();
};

/************************ Get Channels Data && Checking *************************/

unsigned int    ChannelExist(std::vector<Chan>& CurrentChannels, std::string & Name, std::string & client);
void            IsInChannel(Chan& CurrentChannel, int fd, bool flg);
void            IsUserInChannel(Chan& CurrentChannel, std::string & Name, bool flg);
