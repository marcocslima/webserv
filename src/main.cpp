/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/07 23:34:39 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include <fstream>
#include <csignal>
#include <cstdlib>

#define PORT "3000"

ServerSocket	server(PORT);

void handleSignal(int signal) {
	if (signal == SIGINT) {
		std::cout << "\nEncerrando o servidor..." << std::endl;
		server.closeSocket();
		std::exit(0);
	}
}

int	main(void)
{
	int		clientSocket;
	char	buffer[1024] = {0};
	int		bytesRead;

	signal(SIGINT, handleSignal);

	// * TEST
	std::ifstream defaultHtmlFile("www/index.html");
	std::string defaultHtmlContent((std::istreambuf_iterator<char>(defaultHtmlFile)), std::istreambuf_iterator<char>());

	if (!server.createSocket() || !server.bindSocket() || !server.listenForConnections())
	{
		return (-1);
	}
	while (true)
	{
		clientSocket = server.acceptConnection();
		if (clientSocket != -1) {

			// * TEST - Receber a solicitação HTTP do cliente
			bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytesRead > 0) {
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
					}
				}
			}

			// TODO: Implementar a lógica de processamento de solicitações HTTP e envio de respostas.
			close(clientSocket);
		}
	}
	return (0);
}
