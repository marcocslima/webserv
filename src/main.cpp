/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:12:39 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 03:59:47 by pmitsuko         ###   ########.fr       */
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

int	main(void)
{
	signal(SIGINT, handleSignal);
	server.initSockets();
	server.initPoll();
	return (server.run());
}
