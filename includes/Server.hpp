/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/22 11:40:37 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Parser.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "Socket.hpp"

#include <fstream>
#include <vector>

struct SocketInfo
{
    std::string ipAddress;
    std::string port;

    SocketInfo(const std::string &ip, std::string p) : ipAddress(ip), port(p) {}
};

class Server {
    private:
    HttpRequest           _request;
    Parser                _parser;
    std::vector<Socket *> _sockets;
    Poll                  _poll;
    bool                  _verbose;
    std::string           _defaultHtmlContent;

    public:
    Server(void);
    ~Server(void);

    void initParser(const char *configFile);
    void initSockets(void);
    void initPoll(void);
    int  run(void);
    bool acceptNewConnection(size_t i);
    void processClientData(int clientSocket, Parser &parser);
    void closeServer(void);

    void setVerbose(bool verbose);
};
