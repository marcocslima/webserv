/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:14:20 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/22 11:40:35 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
    std::ifstream defaultHtmlFile("www/index.html");

    this->_defaultHtmlContent = std::string((std::istreambuf_iterator<char>(defaultHtmlFile)),
                                            std::istreambuf_iterator<char>());
}

Server::~Server(void) {}

void Server::initParser(const char *configFile) { this->_parser.init(configFile, this->_verbose); }

void Server::initSockets(void)
{
    int                      serverSize;
    std::vector<int>         serverInfo;
    std::vector<std::string> port;

    serverInfo = this->_parser.getSizeServers();
    if (serverInfo.empty()) {
        Logger::error << "The server was not configured correctly" << std::endl;
        this->closeServer();
        exit(1);
    }
    serverSize = serverInfo[0];
    try {
        for (int i = 0; i != serverSize; ++i) {
            Socket *socket;

            port = this->_parser.getServerParam(i, "listen");
            if (port.empty())
                socket = new Socket();
            else
                socket = new Socket(port[0]);
            socket->createSocket();
            socket->bindSocket();
            socket->listenForConnections();
            _sockets.push_back(socket);
        }
    }
    catch (const std::exception &e) {
        Logger::error << e.what() << std::endl;
        this->closeServer();
        exit(1);
    }
}

void Server::initPoll(void)
{
    for (std::vector<Socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        this->_poll.init((*it)->getSocketFd());
    }
}

bool Server::acceptNewConnection(size_t i)
{
    try {
        Socket *client;
        int     clientSocketFd;

        client         = new Socket();
        clientSocketFd = client->acceptConnection(this->_poll.getListeningSocket(i));
        this->_poll.addPoll(clientSocketFd, POLLIN | POLLOUT);
        delete client;

        return (true);
    }
    catch (const std::exception &e) {
        Logger::error << e.what() << std::endl;
        return (false);
    }
}

void Server::processClientData(int clientSocket, Parser &parser)
{
    char buffer[1024] = {0};
    int  bytesRead    = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0) {
        Logger::info << "Client connection closed"
                     << " on socket " << clientSocket << std::endl;
        this->_poll.addFdToClose(clientSocket);
    }
    else {
        std::string request(buffer, bytesRead);

        _request.requestHttp(request, parser);
        size_t start = request.find(_request.getMethod());
        size_t end   = request.find(_request.getHttp());

        if (start != std::string::npos && end != std::string::npos) {
            if (_request.getUri() == "/") {
                char responseHeader[1024];

                sprintf(responseHeader,
                        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n",
                        (int)_defaultHtmlContent.length());

                send(clientSocket, responseHeader, strlen(responseHeader), 0);

                send(clientSocket, _defaultHtmlContent.c_str(), _defaultHtmlContent.length(), 0);

                Logger::info << "Serving the default page." << std::endl;

                if (this->_verbose) {
                    Logger::verbose << "Request: " << request << std::endl;
                    Logger::verbose << "Response: " << responseHeader << _defaultHtmlContent
                                    << std::endl;
                }
            }
        }
    }
}

int Server::run(void)
{
    while (true) {
        if (this->_poll.execute() == -1) {
            Logger::error << "Error in poll()" << std::endl;
            return (1);
        }
        for (size_t i = 0; i < this->_poll.getSize(); ++i) {
            if (this->_poll.isReadable(i)) {
                if (this->_poll.isListeningSocketMatch(i)) {
                    if (!this->acceptNewConnection(i))
                        continue;
                }
                else {
                    int clientSocket = this->_poll.getPollFd(i);
                    if (clientSocket < 0) {
                        Logger::error << "Index out of bounds of vector _pollFds" << std::endl;
                        continue;
                    }
                    processClientData(clientSocket, this->_parser);
                }
            }
        }
        this->_poll.removeMarkedElements();
    }
    return (0);
}

void Server::closeServer(void)
{
    for (std::vector<Socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        delete *it;
    }
    this->_sockets.clear();
    this->_parser.clearParams();
    this->_poll.closePoll();
}

void Server::setVerbose(bool verbose) { this->_verbose = verbose; }
