/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/10 19:34:47 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

HttpRequest::HttpRequest() {
    this->contentLength = 0;
    initMethods();
};

HttpRequest::~HttpRequest() {};

void    HttpRequest::initMethods(){
    methods.push_back("GET");
    methods.push_back("HEAD");
    methods.push_back("POST");
    methods.push_back("DELETE");
}

void HttpRequest::findContentLength() {
    std::string content;

    std::map<std::string, std::string>::const_iterator it = this->headers.find("Content-Length");
    if (it != headers.end()) {
        content = it->second;
        this->contentLength = atoi(content.c_str());
    }
}

bool	HttpRequest::checkFirstLine(std::string requestLine) {
    std::istringstream iss(requestLine);
    std::string line;

    if (!(iss >> this->method >> this->uri >> this->httpVersion)) {
        std::cout << "400 Bad Request." << std::endl;
        return false;
    }
    if (std::count(requestLine.begin(), requestLine.end(), ' ') != 2 ||
        std::find(methods.begin(), methods.end(), method) == methods.end()) {
        std::cout << "400 Bad Request." << std::endl;
        return false;
    }
    if (this->uri[0] != '/' || this->httpVersion != "HTTP/1.1"){
        std::cout << "400 Bad Request." << std::endl;
        return (false);
    }
    return (true);
}

bool HttpRequest::parseHttpRequest(const std::string& request, std::map<std::string, std::string>& headers) {
    size_t firstLineEnd = request.find("\r\n");
    this->requestLine = request.substr(0, firstLineEnd);

    if (!checkFirstLine(requestLine)){
        return (false);
    }
    std::string headersPart = request.substr(firstLineEnd + 2);
    std::istringstream iss(headersPart);
    std::string headerLine;
    while (std::getline(iss, headerLine, '\r')) {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());
        size_t colonPos = headerLine.find(":");

        if (colonPos != std::string::npos) {
            std::string key = headerLine.substr(0, colonPos);
            std::string value = headerLine.substr(colonPos + 1);
            headers[key] = value;
        }
    }
    return true;
}

void	HttpRequest::requestHttp(std::string request) {
    if (parseHttpRequest(request, headers)) {
        std::cout << this->requestLine << std::endl;
        std::cout << "Headers:" << std::endl;
        std::map<std::string, std::string>::iterator it;
        for (it = headers.begin(); it != headers.end(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }
        std::cout << std::endl;
        findContentLength();
    } else {
        std::cerr << "Error parsing HTTP request." << std::endl << std::endl;
    }
}
