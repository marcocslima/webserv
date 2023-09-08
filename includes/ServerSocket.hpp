/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 20:59:33 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/08 00:25:44 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_SOCKET_H
# define SERVER_SOCKET_H

# include "Logger.hpp"
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <cerrno>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>

# define BACKLOG 10

class ServerSocket
{
	private:
		int					_serverSocket;
		const char*			_port;
		struct addrinfo*	_serverInfo;
		Logger				logger;

	public:
		ServerSocket(void);
		ServerSocket(const char* port);
		ServerSocket(ServerSocket const &obj);
		~ServerSocket(void);
		ServerSocket&	operator=(ServerSocket const &obj);

	bool	createSocket(void);
	bool	bindSocket(void);
	bool	listenForConnections(void);
	int		acceptConnection(void);
	void	closeSocket(void);

	const char*			getPort(void) const;
	int					getServerSocket(void) const;
	struct addrinfo*	getServerInfo(void) const;
};

#endif
