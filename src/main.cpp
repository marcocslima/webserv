/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 01:15:07 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Logger.hpp"
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <vector>
#include <poll.h>

// TODO: Criar classe server
// TODO: Criar classe poll

Socket	mySocket("3000", "127.0.0.1");
Socket	mySocket2("3003", "127.0.0.1");

void handleSignal(int signal) {
	if (signal == SIGINT) {
		std::cout << std::endl;
		Logger::info << "Stop server..." << std::endl;
		mySocket.closeSocket();
		mySocket2.closeSocket();
		// TODO: encerrar os pollFds
		std::exit(0);
	}
}

int	main(void)
{
	std::vector<int>			listeningSockets;
	std::vector<struct pollfd>	pollFds;
	std::vector<int>			socketsToClose;

	signal(SIGINT, handleSignal);

	// * TEST
	std::ifstream defaultHtmlFile("www/index.html");
	std::string defaultHtmlContent((std::istreambuf_iterator<char>(defaultHtmlFile)), std::istreambuf_iterator<char>());

	try
	{
		mySocket.createSocket();
		mySocket.bindSocket();
		mySocket.listenForConnections();
		mySocket2.createSocket();
		mySocket2.bindSocket();
		mySocket2.listenForConnections();
	}
	catch(const std::exception& e)
	{
		Logger::error << e.what() << std::endl;
		mySocket.closeSocket();
		mySocket2.closeSocket();
		exit(1);
	}

	listeningSockets.push_back(mySocket.getSocketFd());
	listeningSockets.push_back(mySocket2.getSocketFd());

	struct pollfd	pollFd1;
	pollFd1.fd = mySocket.getSocketFd();
	pollFd1.events = POLLIN;
	pollFd1.revents = 0;
	pollFds.push_back(pollFd1);

	struct pollfd	pollFd2;
	pollFd2.fd = mySocket2.getSocketFd();
	pollFd2.events = POLLIN;
	pollFd2.revents = 0;
	pollFds.push_back(pollFd2);

	while (true)
	{
		int	activity = poll(&pollFds[0], pollFds.size(), -1);
		if (activity == -1) {
			Logger::error << "Error in poll()" << std::endl;
			return 1;
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (i < listeningSockets.size() && pollFds[i].fd == listeningSockets[i])
				{
					try
					{
						int clientSocketFd = mySocket.acceptConnection(listeningSockets[i]);

						// Adicione o novo socket ao conjunto de descritores de arquivo monitorados pelo poll()
						struct pollfd clientPoolFd;
						clientPoolFd.fd = clientSocketFd;
						clientPoolFd.events = POLLIN | POLL_OUT;
						clientPoolFd.revents = 0;
						pollFds.push_back(clientPoolFd);
					}
					catch(const std::exception& e)
					{
						Logger::error << e.what() << std::endl;
						continue;
					}
				}
				else
				{
					// * TEST - Receber a solicitação HTTP do cliente
					int clientSocket = pollFds[i].fd;
					char buffer[1024] = {0};
					int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
					if (bytesRead <= 0)
					{
						// Erro ou conexão fechada
						Logger::info << "Client connection closed" << " on socket "
							<< clientSocket << std::endl;
						close(clientSocket);
						pollFds.erase(pollFds.begin() + i);
					}
					else
					{
						// Analisar a solicitação HTTP para determinar a rota
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
									(int)defaultHtmlContent.length());

								// Enviar o cabeçalho HTTP
								send(clientSocket, responseHeader, strlen(responseHeader), 0);

								// Enviar o conteúdo HTML da página padrão como resposta
								send(clientSocket, defaultHtmlContent.c_str(), defaultHtmlContent.length(), 0);

								Logger::info << "Serving the default page." << std::endl;
								// TODO: incluir uma flag para imprimir mais detalhes dos logs
								// Logger::info << "Request: " << request << std::endl;
								// Logger::info << "Response: " << responseHeader << defaultHtmlContent << std::endl;
							}
						}
					}
				}
			}
		}

		// Remover os elementos marcados para remoção
		for (std::vector<int>::reverse_iterator it = socketsToClose.rbegin(); it != socketsToClose.rend(); ++it)
		{
			pollFds.erase(pollFds.begin() + *it);
		}
	}

	return (0);
}
