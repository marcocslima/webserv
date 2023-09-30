/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:29:22 by mcl               #+#    #+#             */
/*   Updated: 2023/09/24 16:58:01 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AResponse.hpp"

AHttpResponse::AHttpResponse() {}

AHttpResponse::AHttpResponse(Parser *parser): _parser(parser) {}

AHttpResponse::~AHttpResponse() {}

std::string AHttpResponse::assembleResponse() {
    char response[1024];

    sprintf(response,
			"%s %s %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n%s",
			_response.version.c_str(),
            _response.status_code.c_str(),
            _response.status_message.c_str(),
            _response.content_type.c_str(),
            (int)_response.body.length(),
            _response.body.c_str());

    return response;
}

std::string AHttpResponse::getPath(int serverIndex, int locIndex, std::string uri) {
    
    std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
    std::string path = rootDir[0] + uri;

    if (path[path.length() - 1] != '/')
        path += '/';

    return path;
}