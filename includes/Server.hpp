/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:12:47 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/21 08:16:49 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_H
# define SERVER_H

# include "Socket.hpp"
# include "Poll.hpp"
# include "Request.hpp"

# include <vector>
# include <fstream>

struct SocketInfo {
	std::string	ipAddress;
	std::string	port;

	SocketInfo(const std::string& ip, std::string p)
		: ipAddress(ip), port(p) {}
};


class Server
{
	private:
		HttpRequest				_request;
		std::vector<SocketInfo>	_socketsInfo;
		std::vector<Socket*>	_sockets;
		Poll					_poll;
		bool					_verbose;
		std::string				_defaultHtmlContent;

		std::vector<SocketInfo>	initializeSocketInfo(void);

	public:
		Server(void);
		~Server(void);

		void	initSockets(void);
		void	initPoll(void);
		int		run(Parser& parser);
		bool	acceptNewConnection(size_t i);
		void	processClientData(int clientSocket, Parser& parser);
		void	closeServer(void);

		void	setVerbose(bool verbose);
};

#endif
