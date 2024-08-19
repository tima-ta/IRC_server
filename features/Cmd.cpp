/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:05 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/12 10:09:04 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

Cmd::Cmd(cmdInfos& obj, int& fd, std::string &passwd, std::string &hst) : data(obj) , CurrentClientFD(fd), pass(passwd), host(hst)
{

}

Cmd::~Cmd()
{

}

const cmdInfos& Cmd::getCmdInfos() const
{
    return data;
}

void Cmd::BeginExec(int i)
{
    cmdFunc f[] = {&Cmd::PASS, &Cmd::NICK, &Cmd::USER, &Cmd::JOIN, &Cmd::KICK, &Cmd::INVITE, &Cmd::TOPIC, &Cmd::MODE, &Cmd::PRIVMSG, &Cmd::QUIT, &Cmd::PART, &Cmd::NOTICE, &Cmd::BOT};
    try
    {
        (this->*f[i])();
        if (Client::getClient()[CurrentClientFD].first < 3 && i != 9 && i != 10)
            ++Client::getClient()[CurrentClientFD].first;
    }
    catch(const std::exception& e)
    {
        _send(CurrentClientFD, e.what());
    }
}

void    Cmd::executeCmd(const std::string & nick)
{
    ClientInfos& CurrentClient = Client::getClient();
    std::string Which[] = {"PASS", "NICK", "USER", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PRIVMSG", "QUIT", "PART", "NOTICE", "BOT"};

    if (data.first.empty())
    {
        data.first = "QUIT";
        std::vector<std::string> keeP;
        keeP.push_back("Leaving...");
        data.second = keeP;
        BeginExec(9);
        return ;
    }

    if (CurrentClient.find(CurrentClientFD) == CurrentClient.end())
        Client::setClient(CurrentClientFD, "", "");

    int j = CurrentClient[CurrentClientFD].first;
    if (j < 3 && Which[j] == data.first)
        BeginExec(j);
    else if (j < 3 && Which[j] != data.first)
        _send(CurrentClientFD, ": 451 :You have not registered in the right process\r\n");
    else
    {
        for (int i = 0; i < 13; i++)
        {
            if (data.first == Which[i])
            {
                BeginExec(i);
                break ;
            }
            if (i + 1 == 13)
                _send(CurrentClientFD, ": 421 " + nick + " :Unknown command\r\n");
        }
    }
}
