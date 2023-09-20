/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:12:47 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/20 19:37:57 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Parser.hpp"
# include "Socket.hpp"
# include "Poll.hpp"

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
		Parser					_parser;
		std::vector<Socket*>	_sockets;
		Poll					_poll;
		bool					_verbose;
		std::string				_defaultHtmlContent;

	public:
		Server(void);
		~Server(void);

		void	initParser(const char *configFile);
		void	initSockets(void);
		void	initPoll(void);
		int		run(void);
		bool	acceptNewConnection(size_t i);
		void	processClientData(int clientSocket);
		void	closeServer(void);

		void	setVerbose(bool verbose);
};

