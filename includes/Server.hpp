/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/04 01:00:21 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
#include "Parser.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "libs.hpp"

struct SocketInfo {
    std::string ipAddress;
    std::string port;

    SocketInfo(const std::string &ip, std::string p) : ipAddress(ip), port(p) {}
};

class Server {
    private:
    Parser                _parser;
    std::vector<Socket *> _sockets;
    Poll                  _poll;
    HttpRequest           _request;
    Location              _location;
    bool                  _verbose;

    public:
    Server(void);
    ~Server(void);

    void initParser(const char *configFile);
    void initSockets(void);
    void initPoll(void);
    int  run(void);
    bool acceptNewConnection(size_t i);
    void processClientData(int clientSocket);
    void closeServer(void);

    void setVerbose(bool verbose);
};
