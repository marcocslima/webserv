/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:17:35 by mcl               #+#    #+#             */
/*   Updated: 2023/10/12 03:19:39 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/DeleteMethod.hpp"

DeleteMethod::DeleteMethod() {}

DeleteMethod::~DeleteMethod() {}

std::string DeleteMethod::handleMethod(std::string uri)
{

    std::string resorcePath = getDir();
    std::string responseHeader;

    resorcePath = resorcePath + uri;

    std::ifstream file(resorcePath.c_str());

    if (file.is_open()) {
        file.close();
        if (std::remove(resorcePath.c_str()) == 0) {
            _response.version        = "HTTP/1.1";
            _response.status_code    = "204";
            _response.status_message = "OK";
            _response.content_type   = "text/html";
            _response.content_length = "0";
            _response.body           = "<html><body><h1>204 No Content</h1></body></html>";
            responseHeader           = assembleResponse();
            Logger::info << "Resource deleted successfully." << std::endl;
        } else {
            _response.version        = "HTTP/1.1";
            _response.status_code    = "500";
            _response.status_message = "Internal Server Error";
            _response.content_type   = "text/html";
            _response.content_length = "0";
            _response.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
            responseHeader = assembleResponse();
            Logger::info << "Error deleting resource." << std::endl;
        }
    } else {
        _response.version        = "HTTP/1.1";
        _response.status_code    = "404";
        _response.status_message = "Not Found";
        _response.content_type   = "text/html";
        _response.content_length = "0";
        _response.body           = "<html><body><h1>404 Not Found</h1></body></html>";
        responseHeader           = assembleResponse();
        Logger::info << "Resource not found." << std::endl;
    }
    return responseHeader;
}
