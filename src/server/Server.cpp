/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:14:20 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/03 01:06:45 by pmitsuko         ###   ########.fr       */
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

bool isCssRequest(const std::string &request)
{
    size_t found = request.find("Accept: text/css");
    return found != std::string::npos;
}

void Server::processClientData(int clientSocket, Parser &parser)
{
    char                     buffer[1024] = {0};
    int                      bytesRead    = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::vector<int>         serverSize   = parser.getSizeServers();
    std::vector<std::string> port;
    std::vector<std::string> location;
    std::vector<std::string> confRoot;

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

            Logger::warning << "Port: " << _request.getPort() << std::endl;

            // * Busca o index do servidor que corresponde a porta
            int i = 0;
            for (i = 0; i != serverSize[0]; ++i) {
                port = this->_parser.getServerParam(i, "listen");
                if (port.empty()) {
                    continue;
                }
                if (this->_request.getPort() == port[0]) {
                    break;
                }
            }
            Logger::warning << "Server index: " << i << std::endl;

            confRoot = this->_parser.getServerParam(i, "root");

            // * Definindo root
            std::string root = "/var/www/html";
            if (!confRoot.empty()) {
                root = confRoot[0];
            }
            Logger::warning << "Server root: " << root << std::endl;

            // * Verificar se é um CSS
            // TODO: aplicar essa regra para JS, e arquivos como imagem...
            if (isCssRequest(request)) {
                std::stringstream fullPathStream;
                fullPathStream << root << "/" << _request.getUri();

                std::string fullPath = fullPathStream.str();

                const char *fullPathCStr = fullPath.c_str();

                std::ifstream cssFile(fullPathCStr);

                if (cssFile.is_open()) {
                    std::string cssContent((std::istreambuf_iterator<char>(cssFile)),
                                           std::istreambuf_iterator<char>());

                    char responseHeader[1024];
                    sprintf(
                        responseHeader,
                        "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: %d\r\n\r\n",
                        (int)cssContent.length());

                    send(clientSocket, responseHeader, strlen(responseHeader), 0);

                    send(clientSocket, cssContent.c_str(), cssContent.length(), 0);

                    Logger::info << "Serving the css." << std::endl;

                    cssFile.close();
                }
                else {
                    Logger::error << "Erro ao abrir o arquivo CSS." << std::endl;
                }

                return;
            }

            // * Verifica se existe o location igual ao URI
            int locationSize = serverSize[i + 1];
            int j;
            for (j = 0; j < locationSize; j++) {
                location = this->_parser.getLocationParam(i, j, "location");

                std::vector<std::string>::iterator it
                    = std::find(location.begin(), location.end(), _request.getUri());

                if (it != location.end()) {
                    break;
                }
            }

            Logger::warning << "Location size: " << locationSize << std::endl;
            Logger::warning << "Location index: " << j << std::endl;

            Logger::warning << "URI: " << _request.getUri() << std::endl;

            // * Se locationSize == j, significa que a URI não corresponde a nenhum location!
            if (locationSize == j) {
                std::vector<std::string> serverIndex;
                serverIndex = this->_parser.getServerParam(i, "index");

                // * Se não tiver index global exibe 404
                if (serverIndex.empty()) {
                    std::vector<std::string> serverErrorPage;
                    serverErrorPage = this->_parser.getServerParam(i, "error_page");

                    if (serverErrorPage.size() == 2) {
                        if (serverErrorPage[0] == "404") {
                            std::stringstream fullPathStream;
                            fullPathStream << root << "/" << serverErrorPage[1];

                            std::string fullPath = fullPathStream.str();

                            const char *fullPathCStr = fullPath.c_str();

                            std::ifstream htmlFile(fullPathCStr);

                            if (htmlFile.is_open()) {
                                std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)),
                                                        std::istreambuf_iterator<char>());

                                char responseHeader[1024];
                                sprintf(responseHeader,
                                        "HTTP/1.1 404 Not Found\r\nContent-Type: "
                                        "text/html\r\nContent-Length: %d\r\n\r\n",
                                        (int)htmlContent.length());

                                send(clientSocket, responseHeader, strlen(responseHeader), 0);

                                send(clientSocket, htmlContent.c_str(), htmlContent.length(), 0);

                                Logger::info << "Serving the server index page." << std::endl;

                                htmlFile.close();
                            }
                            else {
                                char responseHeader[1024];
                                // Corpo da resposta no formato JSON
                                std::string body = "{\"error\": \"Recurso não encontrado\"}";

                                sprintf(responseHeader,
                                        "HTTP/1.1 404 Not Found\r\n"
                                        "Content-Type: application/json\r\n"
                                        "Content-Length: %ld\r\n"
                                        "\r\n",
                                        body.size());

                                send(clientSocket, responseHeader, strlen(responseHeader), 0);
                                send(clientSocket, body.c_str(), body.size(), 0);

                                Logger::error << "Erro ao abrir o arquivo HTML." << std::endl;
                            }
                        }
                    }

                    Logger::error << "404" << std::endl;
                    return;
                }

                // * servir o index geral
                std::stringstream fullPathStream;
                fullPathStream << root << "/" << serverIndex[0];

                std::string fullPath = fullPathStream.str();

                const char *fullPathCStr = fullPath.c_str();

                std::ifstream htmlFile(fullPathCStr);

                if (htmlFile.is_open()) {
                    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)),
                                            std::istreambuf_iterator<char>());

                    char responseHeader[1024];
                    sprintf(
                        responseHeader,
                        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n",
                        (int)htmlContent.length());

                    send(clientSocket, responseHeader, strlen(responseHeader), 0);

                    send(clientSocket, htmlContent.c_str(), htmlContent.length(), 0);

                    Logger::info << "Serving the server index page." << std::endl;

                    htmlFile.close();
                }
                else {
                    Logger::error << "Erro ao abrir o arquivo HTML." << std::endl;
                }
            }

            if (this->_verbose) {
                Logger::verbose << "Request: " << request << std::endl;
            }
            return;

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
