/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseDelete.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:14:58 by mcl               #+#    #+#             */
/*   Updated: 2023/09/19 21:01:55 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

std::string getDir () {
    char cwd[1024];
    if (getcwd(cwd, 1024) != NULL) {
        std::string dir(cwd);
        return dir;
    }
    else {
        std::cout << "Error getting current working directory" << std::endl;
        return "";
    }
}

std::string HttpResponse::handleDelete(std::string uri) {

    std::string resorcePath = getDir();
    std::string responseHeader;

    resorcePath.replace(resorcePath.find("src/response"), 12, uri);
    
    std::string indexPath = resorcePath;
    std::ifstream file(indexPath.c_str());

    if (file.is_open()) {
        file.close();
        if (std::remove(indexPath.c_str()) == 0) {
            _response.version = "HTTP/1.1";
            _response.status_code = "204";
            _response.status_message = "OK";
            _response.content_type = "text/html";
            _response.content_length = "0";
            _response.body = "<html><body><h1>204 No Content</h1></body></html>";
            responseHeader = assembleResponse();
            std::cout << responseHeader << std::endl;
            std::cout << "File deleted successfully" << std::endl;
        }
        else {
            _response.version = "HTTP/1.1";
            _response.status_code = "500";
            _response.status_message = "Internal Server Error";
            _response.content_type = "text/html";
            _response.content_length = "0";
            _response.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
            responseHeader = assembleResponse();
            std::cout << responseHeader << std::endl;
            std::cout << "Error deleting file" << std::endl;
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
        std::cout << responseHeader << std::endl;
        std::cout << "File not found" << std::endl;
    }
    return responseHeader;
}
