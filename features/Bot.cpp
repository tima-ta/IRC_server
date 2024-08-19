/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araqioui <araqioui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 11:17:22 by araqioui          #+#    #+#             */
/*   Updated: 2023/12/12 09:53:03 by araqioui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include"../Server.hpp"

const char	*API_HOST = "api.weatherapi.com";
const char	*API_PATH = "/v1/current.json";
const char	*API_KEY = "bd9f83a56f2b46b6ae3142327232411";

static std::string const	getCityName(std::vector<std::string> &city)
{
	std::string	cityName;
	size_t		i = 0;

	while (i < city.size())
	{
		cityName += city[i];
		if (!city[i + 1].empty())
			cityName += "%20";
		i++;
	}
	return (cityName);
}

static std::string const	makeRequest(std::string const &cityName)
{
	std::ostringstream	request;

	request << "GET " << API_PATH << "?key=" << API_KEY << "&q=" << cityName << " HTTP/1.1\r\n";
    request << "Host: " << API_HOST << "\r\n";
    request << "Connection: close\r\n\r\n";

	return (request.str());
}

static bool	HTTPcode(std::string const &respond)
{
	std::string	line = respond.substr(0, respond.find('\r'));

	return (line.find("200") != std::string::npos);
}

static std::string	getValue(std::string const &request, std::string const &name)
{
	size_t	nameId = 0, quoteId = 0;

	nameId = request.find(name) + name.length();

	nameId += (name == "\"temp_c\":") ? 0 : 1;
	while (request[nameId + quoteId] != ',' && request[nameId + quoteId] != '\"')
		quoteId++;

	std::string	value = request.substr(nameId, quoteId);

	return (value);
}

void	Cmd::BOT()
{
	Hostent		*hostInfo;
	Sockaddr_in	server;
	int			hostSocket = 0;
	std::string	request, help, message = "";
	char		respond[BUFFER_SIZE];
	ssize_t		len;

	if (this->data.second.size())
	{
		if ((hostInfo = gethostbyname(API_HOST)) == NULL)
			return (perror(COLOR_RED "Gethostbyname " COLOR_RESET));

		if ((hostSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
			return (perror(COLOR_RED "HostSocket " COLOR_RESET));

		memset(&server, '\0', sizeof(Sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_port = htons(80);
		memcpy(&server.sin_addr, hostInfo->h_addr_list[0], hostInfo->h_length);

		if (connect(hostSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
			return (perror(COLOR_RED "Connect " COLOR_RESET));

		request = makeRequest(getCityName(this->data.second));

		if (send(hostSocket, request.c_str(), request.length(), 0) == -1)
			return (perror(COLOR_RED "SendHost " COLOR_RESET));

		while ((len = recv(hostSocket, respond, BUFFER_SIZE - 1, 0)) > 0)
		{
			respond[len] = '\0';
			std::string newstring(respond);
			help += newstring;
		}
		if (len < 0)
			return (perror(COLOR_RED "HostRecv " COLOR_RESET));

		if (HTTPcode(help))
		{
			message = ":" + getValue(help, "\"name\":") + ", " + getValue(help, "\"country\":") + "  " + getValue(help, "\"localtime\":");
			message += "  ==>  " + getValue(help, "\"temp_c\":") + "°C   " + getValue(help, "\"text\":");
		}
		else
			message = ":" + getValue(help, "\"message\":");
		/****** sending ********/
		std::string & nick = Client::getClient()[CurrentClientFD].second.first;
		std::string & user = Client::getClient()[CurrentClientFD].second.second;
		_send(CurrentClientFD, ":" + nick + "!" + user + "@" + host + " " + "NOTICE" + " " + nick + " " + message + "\r\n");
		/**********************/
		close(hostSocket);
	}
};
