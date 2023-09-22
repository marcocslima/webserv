/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/21 08:30:54 by jefernan         ###   ########.fr       */
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

std::map<std::string, std::string> HttpRequest::getHeaders() {
        return (_headers);
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
    if (foundPort == false)
        Logger::error << "Invalid Port." << std::endl;

}

bool	HttpRequest::_checkFirstLine(std::string& requestLine) {
    std::istringstream iss(requestLine);
    std::string line;

    if (!(iss >> this->_method >> this->_uri >> this->_httpVersion)) {
        Logger::error << "Failed to parse method, URI, or HTTP version." << std::endl;
        return false;
    }
    if (std::count(requestLine.begin(), requestLine.end(), ' ') != 2 ||
        std::find(_allowMethods.begin(), _allowMethods.end(), _method) == _allowMethods.end()) {
        Logger::error << "Invalid method or number of spaces." << std::endl;
        return false;
    }
    if (this->_uri[0] != '/' || this->_httpVersion != "HTTP/1.1"){
        Logger::error << "Invalid URI or Unsupported HTTP version." << std::endl;
        return (false);
    }
    return (true);
}

bool HttpRequest::_parseHttpRequest(const std::string& request, std::map<std::string, std::string>& headers) {
    size_t firstLineEnd = request.find("\r\n");
    this->_requestLine = request.substr(0, firstLineEnd);

    if (!_checkFirstLine(_requestLine)){
        return (false);
    }
    std::string headersPart = request.substr(firstLineEnd + 2);
    std::istringstream iss(headersPart);
    std::string headerLine;
    while (std::getline(iss, headerLine, '\r')) {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());
        size_t colonPos = headerLine.find(": ");
        if (colonPos != std::string::npos) {
            std::string key = headerLine.substr(0, colonPos);
            std::string value = headerLine.substr(colonPos + 1);
            headers[key] = value;
            if (key == "Host"){
                size_t pos = value.find(":");
                if (pos != std::string::npos){
                    std::string tmp = value.substr(pos + 1);
                    this->_port = tmp;
                }
            }
        }
    }
    return true;
}

void	HttpRequest::requestHttp(std::string request, Parser& parser) {
    if (_parseHttpRequest(request, _headers)) {
        std::cout << this->_requestLine << std::endl;
        std::cout << "Headers:" << std::endl;
        std::map<std::string, std::string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }
        std::cout << std::endl;
        _checkLocations(parser);
        _checkPorts(parser);
    } else {
        Logger::error << "Error parsing HTTP request." << std::endl;
    }
}
