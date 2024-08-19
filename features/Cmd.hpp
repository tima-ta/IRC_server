/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:09 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 16:49:37 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include"../MainHeader.hpp"
#include"../Client.hpp"
#include"../Channel.hpp"

typedef std::pair<std::string, std::vector<std::string> > cmdInfos;

/************* Command holder *************/

class Cmd
{
private:
        cmdInfos        data;
        int             CurrentClientFD;
        std::string          &pass;
        std::string          &host;

public:
        Cmd(cmdInfos& obj, int& fd, std::string &passwd, std::string &hst);
        ~Cmd();

        const cmdInfos& getCmdInfos() const;
        void            executeCmd(const std::string & nick);
        void            BeginExec(int i);
        void            deleteFromChn(const std::string & nick, const unsigned int & index);
        void            serverReplyFormat(const int &fd, const std::pair<std::string, std::string>& userInfo, const cmdInfos& params);

        void    PASS();
        void    USER();
        void    NICK();
        void    JOIN();
        void    KICK();
        void    INVITE();
        void    TOPIC();
        void    MODE();
        void    PRIVMSG();
        void    QUIT();
        void    PART();
        void    NOTICE();
        void    BOT();
};

typedef void (Cmd::*cmdFunc)();

/************* Helper Functions *************/

void            checkParamsUser(const std::vector<std::string> & vc, const std::string & nick);
void            checkParamsNick(const std::vector<std::string> & vc);
void            toLowerString(std::string & s);
void            placeCmds(std::string cmd, int fd, const char * passwd, std::string IP);
void            _send(int fd, std::string mess);
void            serverReplyFormat(const int &fd, const std::pair<std::string, std::string>& userInfo, const cmdInfos& params);
void            checkLimit(std::string limit, const std::string & nick);
unsigned int    checkChannel(const std::vector<std::string> & vc, const std::string & nick);
unsigned int    checkTopic(const std::vector<std::string> & vc, const std::string & nick);
int             ValidString(const std::string s);
cmdInfos        placeParams(std::string & cmd, const std::string & nick);
