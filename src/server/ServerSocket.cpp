/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 21:01:05 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/07 23:11:06 by pmitsuko         ###   ########.fr       */
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
		std::cerr << "Erro ao obter informações de endereço: "
			<< gai_strerror(result) << std::endl;
		return (false);
	}
	this->_serverSocket = socket(this->_serverInfo->ai_family,
		this->_serverInfo->ai_socktype, this->_serverInfo->ai_protocol);
	if (this->_serverSocket == -1)
	{
		std::cerr << "Erro ao criar o socket do servidor: "
			<< strerror(errno) << std::endl;
		return (false);
	}
	return (true);
}

bool	ServerSocket::bindSocket(void)
{
	if (bind(this->_serverSocket, this->_serverInfo->ai_addr,
		this->_serverInfo->ai_addrlen) == -1)
	{
		std::cerr << "Erro ao vincular o socket à porta: "
			<< strerror(errno) << std::endl;
		return (false);
	}
	return (true);
}

bool	ServerSocket::listenForConnections(void)
{
	if (listen(this->_serverSocket, BACKLOG) == -1) {
		std::cerr << "Erro ao ouvir conexões: " << strerror(errno) << std::endl;
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
		std::cerr << "Erro ao aceitar conexão do cliente: "
			<< strerror(errno) << std::endl;
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
