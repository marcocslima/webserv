/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:29:22 by mcl               #+#    #+#             */
/*   Updated: 2023/09/23 07:29:27 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AResponse.hpp"

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