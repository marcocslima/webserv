/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/10 19:18:35 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"

HttpRequest::HttpRequest()
{
    init();
}

HttpRequest::~HttpRequest()
{}

std::string HttpRequest::getMethod(void) const { return (this->_method); }

std::string HttpRequest::getUri(void) const { return (this->_uri); }

std::string HttpRequest::getHttp(void) const { return (this->_httpVersion); }

std::string HttpRequest::getBody( void ) const
{
	return (this->_body);
}

std::string HttpRequest::getBoundary( void ) const
{
	return (this->_boundary);
}

std::vector<std::string> HttpRequest::getQuery( void ) const
{
	return (this->_paramQuery);
}

std::map<std::string, std::string> HttpRequest::getHeaders() const
{
    return (_header);
}

const char* HttpRequest::RequestException::what() const throw()
{
	return ("Error parsing HTTP request.");
}

void    HttpRequest::init()
{
    _allowMethods.push_back("GET");
    _allowMethods.push_back("POST");
    _allowMethods.push_back("DELETE");
    has_body = false;
	has_form = false;
	has_multipart = false;
    tooLarge = false;
	_uri = "";
	_port = "";
	_method = "";
    _boundary = "";
	_httpVersion = "";
	_statusError = "";
    _contentLength = 0;
    _maxBodySize = 0;
    _paramQuery.clear();
    _header.clear();
}

void	HttpRequest::requestHttp(std::string request, Parser& parser)
{
    size_t firstLineEnd = request.find("\r\n");
    if (firstLineEnd == std::string::npos)
    {
        this->_statusError = BAD_REQUEST;
        return ;
    }

    std::string requestLine = request.substr(0, firstLineEnd);
    std::string headersPart = request.substr(firstLineEnd + 2);
    try {
        _parseFirstLine(requestLine);
        _parseHeaders(headersPart);
        _checkLocations(parser);
        _checkPorts(parser);
        _getMaxBody(parser);

        if (has_body)
        {
            has_multipart = false;
            has_form = false;
            std::map<std::string, std::string>::const_iterator it;
            it = _header.find("Content-Type");
            if (it->second.find("multipart/form-data") != std::string::npos)
                has_multipart = true;
            if (it->second.find("application/x-www-form-urlencoded") != std::string::npos)
                has_form = true;
        }

        if (has_multipart)
            _getMultipartData(request);
        else if (has_body)
           _getBody(request);

    } catch (RequestException const &e) {
        Logger::error << e.what() << " " << this->_statusError << std::endl;
        return ;
    }
}

void	HttpRequest::_parseFirstLine(std::string& requestLine)
{
    std::istringstream iss(requestLine);

    if (!(iss >> this->_method >> this->_uri >> this->_httpVersion)
        || requestLine != this->_method + " " + this->_uri + " " + this->_httpVersion
        || std::find(_allowMethods.begin(), _allowMethods.end(), _method) == _allowMethods.end()
        || this->_uri[0] != '/')
    {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }

    if (this->_httpVersion != "HTTP/1.1")
    {
        this->_statusError = HTTP_VERSION_NOT_SUPPORTED;
        throw RequestException();
    }
    size_t pos = this->_uri.find('?');
    if (pos != std::string::npos)
    {
        _parseQuery();
        this->_uri.erase(pos);
    }
}

void  HttpRequest::_parseQuery(){
    size_t posQuery = this->_uri.find("?");
    std::string query;

    if (posQuery != std::string::npos)
    {
        query = this->_uri.substr(posQuery + 1);
        std::string::size_type start = 0;
        while (start < query.length())
        {
            std::string::size_type equal = query.find('=', start);
            if (equal != std::string::npos)
            {
                std::string::size_type ampersand = query.find('&', equal);
                if (ampersand != std::string::npos)
                {
                    _paramQuery.push_back(query.substr(equal + 1, ampersand - equal - 1));
                    start = ampersand + 1;
                } else {
                    _paramQuery.push_back(query.substr(equal + 1));
                    break;
                }
            } else
                break;
        }
    }
}

void HttpRequest::_parseHeaders(const std::string& request)
{
    std::istringstream iss(request);
    std::string headerLine;
    has_body = false;

    while (std::getline(iss, headerLine, '\r'))
    {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());

        if (headerLine.empty())
            break;

        if (!headerLine.empty())
        {
            size_t colonPos = headerLine.find(": ");

            if (colonPos != std::string::npos)
            {
                std::string key = headerLine.substr(0, colonPos);
                std::string value = headerLine.substr(colonPos + 1);
                _header[key] = value;
                _findHeaders(key, value);
            }
        }
    }
}

void HttpRequest::_findHeaders(std::string key,std::string value )
{
    if (key == "Host")
    {
        size_t pos = value.find(":");
        if (pos != std::string::npos)
        {
            std::string tmp = value.substr(pos + 1);
            this->_port = tmp;
        }
    }
    if (key == "Content-Length")
    {
        int length = atoi(value.c_str());
        if (length > 0){
            has_body = true;
            _contentLength = length;
        }
    }
}

void	HttpRequest::_checkLocations(Parser& parser)
{
    std::vector<int> sizeServers = parser.getSizeServers();
    bool    foundLocation = false;

    int loc = 1;
    for (int i = 0; i < sizeServers[0]; i++)
    {
        for (int j = 0; j < sizeServers[loc]; j++)
        {
            std::vector<std::string> location = parser.getLocationParam(i, j, "location");
            if (!location.empty() && _uri.find(location[0]) == 0)
                foundLocation = true;
        }
        loc++;
    }
    if (foundLocation == false)
        Logger::error << "Invalid location." << std::endl;
}

void	HttpRequest::_checkPorts(Parser& parser)
{
    int servers = parser.getServers();
    bool    foundPort = false;

    for (int i = 0; i < servers; i++)
    {
        std::vector<std::string> listen = parser.getServerParam(i, "listen");
        if (!_port.empty() && _port.find(listen[0]) == 0)
            foundPort = true;
    }
    if (foundPort == false)
    {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }
}

void    HttpRequest::_getMaxBody(Parser& parser){
    int servers = parser.getServers();

   for (int i = 0; i < servers; i++) {
        std::vector<std::string> listen = parser.getServerParam(i, "listen");

        if (!listen.empty() && !listen[0].empty() && _port == listen[0])
        {
            std::vector<std::string> maxBody = parser.getServerParam(i, "client_max_body_size");
            if (!maxBody.empty() && !maxBody[0].empty())
            {
                try {
                    _maxBodySize = std::atoi(maxBody[0].c_str());
                } catch (const std::exception& e) {
                    Logger::error << "Error converting client_max_body_size to int: " << e.what() << std::endl;
                }
            }
            break;
        }
    }
}

void    HttpRequest::_getMultipartData(std::string request)
{
    std::string contentType = _header["Content-Type"];

    size_t pos = contentType.find("boundary=");
    if (pos != std::string::npos)
    {
        _boundary = contentType.substr(pos + 9);
        _boundary = "--" + _boundary;
    } else {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }

    size_t startBody = request.find("\r\n\r\n") + 4;

    if (startBody != std::string::npos)
        _body = request.substr(startBody);
    else
    {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }
}

void    HttpRequest::_getBody(std::string request)
{
    std::size_t bodyStart = request.find("\r\n\r\n") + 4;

    tooLarge = false;
    if (bodyStart != std::string::npos)
        _body = request.substr(bodyStart);
    if (_maxBodySize != 0)
    {
        if (_contentLength > _maxBodySize)
        {
            this->_statusError = ENTITY_TOO_LARGE;
            Logger::error << "Entity too large" << std::endl;
            tooLarge = true;
        }
    }
}
