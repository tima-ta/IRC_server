/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:18 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/12 10:09:29 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

std::string    eliminateSpaces(std::string & str, const int & flg)
{
    std::string  ret;
    int     check(0);

    for (unsigned int i = 0; i < str.length(); i++)
    {
        if ((check == 2 && flg == 1 && str[i] == ' ') || (check == 3 && flg == -1 && str[i] == ' '))
            ;
        else if (str[i] == ' ')
        {
            while (i < str.length() && str[i] == ' ')
                i++;
            if (i !=  str.length())
                ret.push_back(' ');
            check++;
        }
        if (i !=  str.length())
            ret.push_back(str[i]);
    }
    str.clear();
    return ret;
}

cmdInfos    placeParams(std::string & cmd)
{
    cmdInfos        params;
    std::vector<std::string>  save;
    std::string          toStr;
    int             flg(0);

    std::stringstream   in(cmd);
    std::getline(in, toStr, ' ');
    if (toStr == "PRIVMSG" || toStr == "NOTICE" || toStr == "TOPIC")
        flg = 1;
    else if (toStr == "KICK")
        flg = -1;
    cmd = eliminateSpaces(cmd, flg);
    std::stringstream  instr(cmd);
    int check(0);
    for (; std::getline(instr, toStr, ' '); )
    {
        if (!check)
        {
            params.first = toStr;
            check++;
            continue ;
        }
        save.push_back(toStr);
        if ((check == 1 && flg == 1) || (check == 2 && flg == -1))
        {
            std::getline(instr, toStr);
            if (toStr[0] != ':')
                toStr.insert(0, ":");
            save.push_back(toStr);
            break ;
        }
        check++;
    }
    params.second = save;
    return params;
}

void    placeCmds(std::string cmd, int fd, const char * passwd, std::string IP)
{
    cmdInfos    obj;
    ClientInfos clients = Client::getClient();
    std::string ps(passwd);

    if (!cmd.empty())
    {
        if (cmd[cmd.size() - 1] == '\n')
            cmd.erase(cmd.size() - 1);
        if (cmd[cmd.size() - 1] == '\r')
            cmd.erase(cmd.size() - 1);
        obj = placeParams(cmd);
    }
    Cmd command(obj, fd, ps, IP);
    command.executeCmd(clients[fd].second.first);
}

/*********************************** Commands Syntax *****************************************/

int    ValidString(const std::string s)
{
    int i = 0;

    for (; i < int(s.length()); i++)
    {
        if (s[i] >= 127)
            return -1;
    }
    return i;
} 

void    checkParamsUser(const std::vector<std::string> & vc, const std::string & nick)
{
    if ((vc.size() != 1 && vc.size() != 4) || vc[0].empty())
        throw std::runtime_error(": 461 " + nick + " :USER Not enough parameters\r\n");

    unsigned int i = 0;
    for (; i < vc[0].length(); i++)
        if (!(isalnum(vc[0][i]) || vc[0][i] == '_' || vc[0][i] == '-'))
            throw std::runtime_error(": 432" + nick + " :Non valid character\r\n");

    if (i > 9)
        throw std::runtime_error(": 432 " + nick + " :Passed the valid length\r\n");
}

void    checkParamsNick(const std::vector<std::string> & vc)
{
    if (vc.size() != 1 || vc[0].empty())
        throw std::runtime_error(": 461 :NICK Not enough parameters\r\n");

    unsigned int i = 0;
    if (isdigit(vc[0][i]) || vc[0][0] == '-')
        throw std::runtime_error(": 432" + vc[0] + " :Erroneous Nickname\r\n");
    for (; i < vc[0].length(); i++)
        if (!(isalnum(vc[0][i]) || vc[0][i] == '_' || vc[0][i] == '-'))
            throw std::runtime_error(": 432 " + vc[0] + " :Erroneous Nickname\r\n");

    if (i > 9)
        throw std::runtime_error(": 432 " + vc[0] + " :Erroneous Nickname\r\n");
}

unsigned int    checkChannel(const std::vector<std::string> & vc, const std::string & nick)
{
    unsigned int sz = vc.size();
    if (sz != 1 && sz != 2)
        throw std::runtime_error(": 461 " + nick + " :JOIN Not enough parameters\r\n");

    if (vc[0].empty() || vc[0][0] != '#' || (sz == 2 && vc[1].empty()))
        throw std::runtime_error(": 403 " + vc[0] + " :No such channel\r\n");

    int i = ValidString(vc[0]);
    if (i < 0 || i > 50)
        throw std::runtime_error(": 403 " + vc[0] + " :No such channel\r\n");
    return sz;
}

unsigned int    checkTopic(const std::vector<std::string> & vc, const std::string & nick)
{
    unsigned int sz = vc.size();

    if (sz != 1 && sz != 2)
        throw std::runtime_error(": 461 " + nick + " :TOPIC Not enough parameters\r\n");

    if (vc[0].empty() || (sz == 2 && vc[1].empty()))
        throw std::runtime_error(": 461 " + nick + " :TOPIC Not enough parameters\r\n");

    if (sz == 2 && ValidString(vc[1]) < 0)
        throw std::runtime_error(": 461 " + nick + " :Non valid character(s)\r\n");
    return sz;
}

void    checkLimit(std::string limit, const std::string & nick)
{
    for (unsigned int i(0); i < limit.length(); i++)
        if (!isdigit(limit[i]))
            throw std::runtime_error(": 696 " + nick + " :MODE l " + limit + " Invalid limit mode parameter. Syntax: <limit>.\r\n");
}

void    toLowerString(std::string & s)
{
    for (unsigned int i(0); i < s.length(); i++)
        s[i] = tolower(s[i]);
}

/*****************************************************/

void _send(int fd, std::string mess)
{
    send(fd, mess.c_str(), mess.length(), 0);
}

void    Cmd::serverReplyFormat(const int &fd, const std::pair<std::string, std::string>& userInfo, const cmdInfos& params)
{
    std::string save;

    for (unsigned int i(0); i < params.second.size(); i++)
    {
        save += params.second[i];
        if (i != params.second.size() - 1)
            save += " ";
    }
    _send(fd, ":" + userInfo.first + "!" + userInfo.second + "@" + host + " " + params.first + " " + save + "\r\n");
}
