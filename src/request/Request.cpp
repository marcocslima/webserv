/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/22 15:14:08 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

HttpRequest::HttpRequest() {
    initMethods();
};

HttpRequest::~HttpRequest() {};

std::string HttpRequest::getMethod( void ) const{
	return (this->_method);
}

std::string HttpRequest::getUri( void ) const{
	return (this->_uri);
}

std::string HttpRequest::getHttp( void ) const{
	return (this->_httpVersion);
}

std::string HttpRequest::getStatusError( void ) const{
	return (this->_statusError);
}

std::map<std::string, std::string> HttpRequest::getHeaders() {
        return (_headers);
}

const char* HttpRequest::RequestException::what() const throw(){
	return ("Error parsing HTTP request.");
}

void    HttpRequest::initMethods(){
    _allowMethods.push_back("GET");
    _allowMethods.push_back("HEAD");
    _allowMethods.push_back("POST");
    _allowMethods.push_back("DELETE");
}

void	HttpRequest::_checkLocations(Parser& parser){
    std::vector<int> sizeServers = parser.getSizeServers();
    bool    foundLocation = false;

    int loc = 1;
    for (int i = 0; i < sizeServers[0]; i++){
        for (int j = 0; j < sizeServers[loc]; j++){
            std::vector<std::string> location = parser.getLocationParam(i, j, "location");
            if (!location.empty() && _uri.find(location[0]) == 0)
                foundLocation = true;
        }
        loc++;
    }
    if (foundLocation == false)
        Logger::error << "Invalid location." << std::endl;
}

void	HttpRequest::_checkPorts(Parser& parser){
    int servers = parser.getServers();
    bool    foundPort = false;

    for (int i = 0; i < servers; i++){
        std::vector<std::string> listen = parser.getServerParam(i, "listen");
        if (!_port.empty() && _port.find(listen[0]) == 0)
                foundPort = true;
    }
    if (foundPort == false){
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }
}

void	HttpRequest::_parseFirstLine(std::string& requestLine) {
    std::istringstream iss(requestLine);

    if (!(iss >> this->_method >> this->_uri >> this->_httpVersion)
        || this->_requestLine != this->_method + " " + this->_uri + " " + this->_httpVersion
        || std::find(_allowMethods.begin(), _allowMethods.end(), _method) == _allowMethods.end()
        || this->_uri[0] != '/') {
            this->_statusError = BAD_REQUEST;
            throw RequestException();
        }
    if (this->_httpVersion != "HTTP/1.1"){
        this->_statusError = HTTP_VERSION_NOT_SUPPORTED;
        throw RequestException();
    }
}

void HttpRequest::_parseHeaders(const std::string& request) {
    std::istringstream iss(request);
    std::string headerLine;

    while (std::getline(iss, headerLine, '\r')) {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());

        if (!headerLine.empty()){
            size_t colonPos = headerLine.find(": ");

            if (colonPos != std::string::npos) {
                std::string key = headerLine.substr(0, colonPos);
                std::string value = headerLine.substr(colonPos + 1);
                _headers[key] = value;
                if (key == "Host"){
                    size_t pos = value.find(":");
                    if (pos != std::string::npos){
                        std::string tmp = value.substr(pos + 1);
                        this->_port = tmp;
                    }
                }
            } else {
                this->_statusError = BAD_REQUEST;
                throw RequestException();
            }
        }
    }
}

void	HttpRequest::requestHttp(std::string request, Parser& parser) {
    size_t firstLineEnd = request.find("\r\n");
    if (firstLineEnd == std::string::npos) {
        this->_statusError = BAD_REQUEST;
        return ;
    }

    this->_requestLine = request.substr(0, firstLineEnd);
    std::string headersPart = request.substr(firstLineEnd + 2);
    try {
        _parseFirstLine(_requestLine);
        _parseHeaders(headersPart);
        _checkLocations(parser);
        _checkPorts(parser);
    } catch (RequestException const &e) {
        Logger::error << e.what() << " " << this->_statusError << std::endl;
    }
}
