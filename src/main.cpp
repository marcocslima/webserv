/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/04 09:26:30 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"
#include <csignal>

Server	server;

void handleSignal(int signal) {
	if (signal == SIGINT) {
		std::cout << std::endl;
		Logger::info << "Stop server..." << std::endl;
		server.closeServer();
		std::exit(0);
	}
}

int	main(int argc, char* argv[])
{
	if (argc < 2)
	{
		Logger::error << "Error: Invalid number of arguments" << std::endl;
		return (1);
	}
	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);

		if (arg == "-a") {
			server.setVerbose(true);
			break;
		}
	}
	signal(SIGINT, handleSignal);
	server.initParser(argv[1]);
	server.initSockets();
	server.initPoll();
	return (server.run());
}
