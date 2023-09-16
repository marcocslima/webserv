/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 05:10:54 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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
	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);

		if (arg == "-a") {
			server.setVerbose(true);
			break;
		}
	}

	signal(SIGINT, handleSignal);
	server.initSockets();
	server.initPoll();
	return (server.run());
}
