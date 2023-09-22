/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/21 08:18:24 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parser.hpp"
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
	if (argc != 2) {
		Logger::error << "Error: Invalid number of arguments" << std::endl;
		exit(1);
	}

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);

		if (arg == "-a") {
			server.setVerbose(true);
			break;
		}
	}

	Parser parser(argv[1]);

	signal(SIGINT, handleSignal);
	server.initSockets();
	server.initPoll();
	return (server.run(parser));
}
