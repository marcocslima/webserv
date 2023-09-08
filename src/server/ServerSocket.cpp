/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 21:01:05 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/08 00:36:41 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

ServerSocket::ServerSocket(void):
	_serverSocket(-1),
	_port("3000"),
	_serverInfo(NULL)
{}

ServerSocket::ServerSocket(const char* port):
	_serverSocket(-1),
	_port(port),
	_serverInfo(NULL)
{}

ServerSocket::ServerSocket(ServerSocket const &obj)
{
	*this = obj;
	return ;
}

ServerSocket::~ServerSocket(void)
{}

ServerSocket&	ServerSocket::operator=(ServerSocket const &obj)
{
	if (this != &obj)
	{
		this->_port = obj.getPort();
		this->_serverSocket = obj.getServerSocket();
		this->_serverInfo = obj.getServerInfo();
	}
	return (*this);
}

bool	ServerSocket::createSocket(void)
{
	struct addrinfo	hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int result = getaddrinfo(NULL, this->_port, &hints, &this->_serverInfo);
	if (result != 0)
	{
		std::string errorMessage = "Error getting address information: " +
			std::string(gai_strerror(result));
		logger.log(Logger::ERROR, errorMessage);
		return (false);
	}
	this->_serverSocket = socket(this->_serverInfo->ai_family,
		this->_serverInfo->ai_socktype, this->_serverInfo->ai_protocol);
	if (this->_serverSocket == -1)
	{
		std::string errorMessage = "Error creating server socket: " +
			std::string(strerror(errno));
		logger.log(Logger::ERROR, errorMessage);
		return (false);
	}
	return (true);
}

bool	ServerSocket::bindSocket(void)
{
	if (bind(this->_serverSocket, this->_serverInfo->ai_addr,
		this->_serverInfo->ai_addrlen) == -1)
	{
		std::string errorMessage = "Error binding socket to port: " +
			std::string(strerror(errno));
		logger.log(Logger::ERROR, errorMessage);
		return (false);
	}
	return (true);
}

bool	ServerSocket::listenForConnections(void)
{
	if (listen(this->_serverSocket, BACKLOG) == -1) {
		std::string errorMessage = "Error listening for connections: " +
			std::string(strerror(errno));
		logger.log(Logger::ERROR, errorMessage);
		return (false);
	}
	return (true);
}

int	ServerSocket::acceptConnection(void)
{
	struct sockaddr_storage	their_addr;
	socklen_t				addr_size;

	addr_size = sizeof their_addr;
	int clientSocket = accept(this->_serverSocket, (struct sockaddr *)&their_addr,
		&addr_size);
	if (clientSocket == -1)
	{
		std::string errorMessage = "Error accepting client connection: " +
			std::string(strerror(errno));
		logger.log(Logger::ERROR, errorMessage);
	}
	return (clientSocket);
}

void	ServerSocket::closeSocket(void)
{
	if (this->_serverSocket != -1)
	{
		close(this->_serverSocket);
		this->_serverSocket = -1;
	}
	if (this->_serverInfo != NULL)
	{
		freeaddrinfo(this->_serverInfo);
		this->_serverInfo = NULL;
	}
}

const char*	ServerSocket::getPort(void) const
{
	return (this->_port);
}

int	ServerSocket::getServerSocket(void) const
{
	return (this->_serverSocket);
}

struct addrinfo*	ServerSocket::getServerInfo(void) const
{
	return (this->_serverInfo);
}
