/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:17:35 by mcl               #+#    #+#             */
/*   Updated: 2023/10/06 19:28:24 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/DeleteMethod.hpp"

DeleteMethod::DeleteMethod() {}

DeleteMethod::~DeleteMethod() {}

std::string getDir () {
    char cwd[1024];
    if (getcwd(cwd, 1024) != NULL) {
        std::string dir(cwd);
        size_t pos = dir.find("webserv");
        if (pos != std::string::npos)
            dir = dir.substr(0, pos + 7);
        return dir;
    }
    else {
        std::cout << "Error getting current working directory" << std::endl;
        return "";
    }
}

std::string DeleteMethod::handleMethod(std::string uri) {

    std::string resorcePath = getDir();
    std::string responseHeader;

    resorcePath = resorcePath + uri;

    std::ifstream file(resorcePath.c_str());

    if (file.is_open()) {
        file.close();
        if (std::remove(resorcePath.c_str()) == 0) {
            _response.version = "HTTP/1.1";
            _response.status_code = "204";
            _response.status_message = "OK";
            _response.content_type = "text/html";
            _response.content_length = "0";
            _response.body = "<html><body><h1>204 No Content</h1></body></html>";
            responseHeader = assembleResponse();
            Logger::info << "Resource deleted successfully." << std::endl;
        }
        else {
            _response.version = "HTTP/1.1";
            _response.status_code = "500";
            _response.status_message = "Internal Server Error";
            _response.content_type = "text/html";
            _response.content_length = "0";
            _response.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
            responseHeader = assembleResponse();
            Logger::info << "Error deleting resource." << std::endl;
        }
    }
    else {
        _response.version = "HTTP/1.1";
        _response.status_code = "404";
        _response.status_message = "Not Found";
        _response.content_type = "text/html";
        _response.content_length = "0";
        _response.body = "<html><body><h1>404 Not Found</h1></body></html>";
        responseHeader = assembleResponse();
        Logger::info << "Resource not found." << std::endl;
    }
    return responseHeader;
}
