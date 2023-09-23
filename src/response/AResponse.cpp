/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:25:37 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/23 18:29:37 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AResponse.hpp"

AHttpResponse::AHttpResponse() {}

AHttpResponse::AHttpResponse(Parser *parser): _parser(parser) {}

AHttpResponse::~AHttpResponse() {}

std::string AHttpResponse::assembleResponse() {
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

std::string AHttpResponse::getPath(int serverIndex, int locIndex, std::string uri) {

    std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
    std::string path = rootDir[0] + uri;

    if (path[path.length() - 1] != '/')
        path += '/';

    std::cout << path << std::endl;

    return path;
}

