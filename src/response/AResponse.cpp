/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:25:37 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/12 03:16:09 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AResponse.hpp"

AHttpResponse::AHttpResponse() {}

AHttpResponse::AHttpResponse(Parser *parser) : _parser(parser) {}

AHttpResponse::~AHttpResponse() {}

std::string AHttpResponse::assembleResponse()
{
    std::string response;

    response += _response.version + " ";
    response += _response.status_code + " ";
    response += _response.status_message + "\r\n";
    response += "Content-Type: " + _response.content_type + "\r\n";
    response += "Content-Length: " + _response.content_length + "\r\n";
    response += "\r\n";
    response += _response.body;

    return response;
}

std::string AHttpResponse::getPath(int serverIndex, int locIndex, std::string uri)
{

    std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
    std::string              path    = rootDir[0] + uri;

    if (path[path.length() - 1] != '/')
        path += '/';

    std::cout << path << std::endl;

    return path;
}

bool AHttpResponse::verifyServerNames(const HttpRequest &request, Parser &parser)
{

    std::string       uri = request.getHeaders()["Host"];
    std::stringstream ss(uri);

    std::string host;
    if (std::getline(ss, host, ':'))
        host.erase(std::remove_if(host.begin(), host.end(), ::isspace), host.end());

    std::string port;
    if (std::getline(ss, port))
        port.erase(std::remove_if(port.begin(), port.end(), ::isspace), port.end());

    std::vector<std::string> serverNames = getServerNames(parser, port);

    for (std::vector<std::string>::iterator it = serverNames.begin(); it != serverNames.end();
         ++it) {
        if (*it == host)
            return true;
    }

    return false;
}

std::vector<std::string> getServerNames(Parser &parser, std::string port)
{
    std::vector<std::string> serverNames;
    int                      servers = parser.getServers();

    for (int i = 0; i < servers; i++)
        if (parser.getServerParam(i, "listen")[0] == port)
            serverNames = parser.getServerParam(i, "server_name");

    return serverNames;
}
