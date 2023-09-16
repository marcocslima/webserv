/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:14:20 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 04:39:24 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void):
	_socketsInfo(initializeSocketInfo())
{
	std::ifstream defaultHtmlFile("www/index.html");

	this->_defaultHtmlContent = std::string((std::istreambuf_iterator<char>(defaultHtmlFile)),
		std::istreambuf_iterator<char>());
}

Server::~Server(void)
{}

std::vector<SocketInfo>	Server::initializeSocketInfo(void)
{
	std::vector<SocketInfo> socketsInfo;
	socketsInfo.push_back(SocketInfo("127.0.0.1", "3000"));
	socketsInfo.push_back(SocketInfo("127.0.0.1", "3010"));
	socketsInfo.push_back(SocketInfo("127.0.0.1", "3020"));
	return socketsInfo;
}

void	Server::initSockets(void)
{
	try
	{
		for (std::vector<SocketInfo>::const_iterator it = _socketsInfo.begin(); it != _socketsInfo.end(); ++it)
		{
			Socket* socket = new Socket(it->port, it->ipAddress);
			socket->createSocket();
			socket->bindSocket();
			socket->listenForConnections();
			_sockets.push_back(socket);
		}
	}
	catch(const std::exception& e)
	{
		Logger::error << e.what() << std::endl;
		this->closeServer();
		exit(1);
	}
}

void	Server::initPoll(void)
{
	for (std::vector<Socket*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
	{
		this->_poll.init((*it)->getSocketFd());
	}
}

bool	Server::acceptNewConnection(size_t i)
{
	try {
		Socket	*client;
		int		clientSocketFd;

		client = new Socket();
		clientSocketFd = client->acceptConnection(this->_poll.getListeningSocket(i));
		this->_poll.addPoll(clientSocketFd, POLLIN | POLLOUT);
		delete client;

		return (true);
	} catch (const std::exception& e) {
		Logger::error << e.what() << std::endl;
		return (false);
	}
}

void	Server::processClientData(int clientSocket)
{
	char buffer[1024] = {0};
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesRead <= 0)
	{
		Logger::info << "Client connection closed" << " on socket "
			<< clientSocket << std::endl;
		this->_poll.addFdToClose(clientSocket);
	}
	else
	{
		std::string	request(buffer, bytesRead);
		size_t		start = request.find("GET ");
		size_t		end = request.find(" HTTP/1.1");

		if (start != std::string::npos && end != std::string::npos)
		{
			std::string route = request.substr(start + 4, end - start - 4);

			if (route == "/") // Servir a página padrão (www/index.html)
			{
				// Construir o cabeçalho HTTP
				char	responseHeader[1024];

				sprintf(responseHeader,
					"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n",
					(int)_defaultHtmlContent.length());

				// Enviar o cabeçalho HTTP
				send(clientSocket, responseHeader, strlen(responseHeader), 0);

				// Enviar o conteúdo HTML da página padrão como resposta
				send(clientSocket, _defaultHtmlContent.c_str(), _defaultHtmlContent.length(), 0);

				Logger::info << "Serving the default page." << std::endl;
				// TODO: incluir uma flag para imprimir mais detalhes dos logs
				// Logger::info << "Request: " << request << std::endl;
				// Logger::info << "Response: " << responseHeader << _defaultHtmlContent << std::endl;
			}
		}
	}
}

int	Server::run(void)
{
	while (true)
	{
		if (this->_poll.execute() == -1)
		{
			Logger::error << "Error in poll()" << std::endl;
			return (1);
		}
		for (size_t i = 0; i < this->_poll.getSize(); ++i)
		{
			if (this->_poll.isReadable(i))
			{
				if (this->_poll.isListeningSocketMatch(i))
				{
					if (!this->acceptNewConnection(i))
						continue;
				}
				else
				{
					int clientSocket = this->_poll.getPollFd(i);
					if (clientSocket < 0)
					{
						Logger::error << "Index out of bounds of vector _pollFds" << std::endl;
						continue;
					}
					Logger::warning << "i = " << i << " | socket = " << clientSocket << std::endl;
					processClientData(clientSocket);
				}
			}
		}
		this->_poll.removeMarkedElements();
	}
	return (0);
}

void	Server::closeServer(void)
{
	for (std::vector<Socket*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
	{
		delete *it;
	}
	_sockets.clear();
	this->_poll.closePoll();
}
