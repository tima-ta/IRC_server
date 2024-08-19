/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fraqioui <fraqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:14:37 by fraqioui          #+#    #+#             */
/*   Updated: 2023/12/11 13:49:39 by fraqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Cmd.hpp"

void    Cmd::PASS()
{
    std::string & nick = Client::getClient()[CurrentClientFD].second.first;

    if (data.second.size() != 1)
        throw std::runtime_error(": 461 " + nick + " :PASS Not enough parameters\r\n");

    if (Client::getClient()[CurrentClientFD].first >= 3)
        throw std::runtime_error(": 462 " + nick + " :You may not reregister\r\n");

    if (data.second[0] != pass)
        throw std::runtime_error(": 464 " + nick + " :PASS Password incorrect\r\n");

    return ;
}
