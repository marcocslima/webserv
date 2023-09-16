/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 20:59:33 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 03:31:12 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_H
# define SOCKET_H

# include "Logger.hpp"

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <cerrno>
# include <stdexcept>
# include <string>

# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>

class Socket
{
	private:
		int					_socketFd;
		std::string			_port;
		std::string			_ip;
		struct addrinfo*	_serverInfo;

	public:
		Socket(std::string port = "8080", std::string ip = "127.0.0.1");
		~Socket(void);

		void	createSocket(void);
		void	bindSocket(void);
		void	listenForConnections(void);
		void	*get_in_addr(struct sockaddr *sa);
		int		acceptConnection(int socketFd);
		void	closeSocket(void);

		int			getSocketFd(void) const;
		std::string	getPort(void) const;
		std::string	getIp(void) const;

		class SocketException: public std::exception
		{
			private:
				std::string	_msg;

			public:
				SocketException(const char* msg);
				virtual ~SocketException() throw ();
				virtual const char* what() const throw();
		};
};

#endif
