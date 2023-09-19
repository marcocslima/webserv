/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:19:17 by mcl               #+#    #+#             */
/*   Updated: 2023/09/19 15:11:31 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(Parser *parser): _parser(parser) {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::assembleResponse() {
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

std::string HttpResponse::getPath(int serverIndex, int locIndex, std::string uri) {
    
    std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
    std::string path = rootDir[0] + uri;

    if (path[path.length() - 1] != '/')
        path += '/';

    std::cout << path << std::endl;

    return path;
}