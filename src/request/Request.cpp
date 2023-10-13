/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/13 12:19:39 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"

HttpRequest::HttpRequest() { init(); }

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getMethod(void) const { return (this->_method); }

std::string HttpRequest::getPort(void) const { return (this->_port); }

std::string HttpRequest::getUri(void) const { return (this->_uri); }

std::string HttpRequest::getHttp(void) const { return (this->_httpVersion); }

std::string HttpRequest::getBody(void) const { return (this->_body); }

std::string HttpRequest::getBoundary(void) const { return (this->_boundary); }

std::vector<std::string> HttpRequest::getQuery(void) const { return (this->_paramQuery); }

std::map<std::string, std::string> HttpRequest::getHeaders() const { return (_header); }

int HttpRequest::getServerIndex(void) const { return (this->_serverIndex); }

int HttpRequest::getLocationIndex(void) const { return (this->_locationIndex); }

int HttpRequest::getLocationSize(void) const { return (this->_locationSize); }

std::string HttpRequest::getRoot(void) const { return (this->_root); }

std::vector<std::string> HttpRequest::getErrorPageConfig(void) const
{
    return (this->_errorPageConfig);
}

const char *HttpRequest::RequestException::what() const throw()
{
    return ("Error parsing HTTP request.");
}

void HttpRequest::init()
{
    _has_body      = false;
    _has_form      = false;
    _has_multipart = false;
    _tooLarge      = false;
    _uri           = "";
    _port          = "";
    _method        = "";
    _boundary      = "";
    _httpVersion   = "";
    _statusError   = "";
    _contentLength = 0;
    _maxBodySize   = 0;
    _paramQuery.clear();
    _header.clear();
}

void HttpRequest::requestHttp(std::string request, Parser &parser)
{
    std::vector<int> serverSize   = parser.getSizeServers();
    size_t           firstLineEnd = request.find("\r\n");

    if (firstLineEnd == std::string::npos) {
        this->_statusError = BAD_REQUEST;
        return;
    }

    std::string requestLine = request.substr(0, firstLineEnd);
    std::string headersPart = request.substr(firstLineEnd + 2);
    try {
        _parseFirstLine(requestLine);
        _parseHeaders(headersPart);
        _getMaxBody(parser);
        _getServerParam(parser);

        if (_has_body) {
            _has_multipart = false;
            _has_form      = false;
            std::map<std::string, std::string>::const_iterator it;
            it = _header.find("Content-Type");
            if (it->second.find("multipart/form-data") != std::string::npos)
                _has_multipart = true;
            if (it->second.find("application/x-www-form-urlencoded") != std::string::npos)
                _has_form = true;
        }

        if (_has_multipart)
            _getMultipartData(request);
        else if (_has_body)
            _getBody(request);

    } catch (RequestException const &e) {
        Logger::error << e.what() << " " << this->_statusError << std::endl;
        return;
    }
}

void HttpRequest::_parseFirstLine(std::string &requestLine)
{
    std::istringstream iss(requestLine);

    if (!(iss >> this->_method >> this->_uri >> this->_httpVersion)
        || requestLine != this->_method + " " + this->_uri + " " + this->_httpVersion
        || this->_uri[0] != '/') {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }

    if (this->_httpVersion != "HTTP/1.1") {
        this->_statusError = HTTP_VERSION_NOT_SUPPORTED;
        throw RequestException();
    }
    size_t pos = this->_uri.find('?');
    if (pos != std::string::npos) {
        _parseQuery();
        this->_uri.erase(pos);
    }
}

void HttpRequest::_parseQuery()
{
    size_t      posQuery = this->_uri.find("?");
    std::string query;

    if (posQuery != std::string::npos) {
        query                        = this->_uri.substr(posQuery + 1);
        std::string::size_type start = 0;
        while (start < query.length()) {
            std::string::size_type equal = query.find('=', start);
            if (equal != std::string::npos) {
                std::string::size_type ampersand = query.find('&', equal);
                if (ampersand != std::string::npos) {
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

void HttpRequest::_parseHeaders(const std::string &request)
{
    std::istringstream iss(request);
    std::string        headerLine;
    _has_body = false;

    while (std::getline(iss, headerLine, '\r')) {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());

        if (headerLine.empty())
            break;

        if (!headerLine.empty()) {
            size_t colonPos = headerLine.find(": ");

            if (colonPos != std::string::npos) {
                std::string key   = headerLine.substr(0, colonPos);
                std::string value = headerLine.substr(colonPos + 1);
                _header[key]      = value;
                _findHeaders(key, value);
            }
        }
    }
}

void HttpRequest::_findHeaders(std::string key, std::string value)
{
    if (key == "Host") {
        size_t pos = value.find(":");
        if (pos != std::string::npos) {
            std::string tmp = value.substr(pos + 1);
            this->_port     = tmp;
        }
    }
    if (key == "Content-Length") {
        int length = atoi(value.c_str());
        if (length > 0) {
            _has_body      = true;
            _contentLength = length;
        }
    }
}

void HttpRequest::_getMaxBody(Parser &parser)
{
    int servers = parser.getServers();

    for (int i = 0; i < servers; i++) {
        std::vector<std::string> listen = parser.getServerParam(i, "listen");

        if (!listen.empty() && !listen[0].empty() && _port == listen[0]) {
            std::vector<std::string> maxBody = parser.getServerParam(i, "client_max_body_size");
            if (!maxBody.empty() && !maxBody[0].empty())
                _maxBodySize = std::atoi(maxBody[0].c_str());
            break;
        }
    }
}

void HttpRequest::_getMultipartData(std::string request)
{
    std::string contentType = _header["Content-Type"];

    size_t pos = contentType.find("boundary=");
    if (pos != std::string::npos) {
        _boundary = contentType.substr(pos + 9);
        _boundary = "--" + _boundary;
    } else {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }

    size_t startBody = request.find("\r\n\r\n") + 4;

    if (startBody != std::string::npos)
        _body = request.substr(startBody);
    else {
        this->_statusError = BAD_REQUEST;
        throw RequestException();
    }
}

void HttpRequest::_getBody(std::string request)
{
    std::size_t bodyStart = request.find("\r\n\r\n") + 4;

    _tooLarge = false;
    if (bodyStart != std::string::npos)
        _body = request.substr(bodyStart);
    if (_maxBodySize > 0) {
        if (_contentLength > _maxBodySize) {
            this->_statusError = ENTITY_TOO_LARGE;
            Logger::error << "Entity too large" << std::endl;
            _tooLarge = true;
        }
    } else if (_maxBodySize < 0) {
        Logger::error << "Invalid client_max_body_size." << std::endl;
    }
}

void HttpRequest::_getServerParam(Parser &parser)
{
    std::vector<int> serverSize = parser.getSizeServers();

    this->_serverIndex   = this->_findServerIndex(parser, serverSize[0], this->_port);
    this->_locationSize  = serverSize[this->_serverIndex + 1];
    this->_locationIndex = this->_findLocationIndex(parser);
    this->_setRoot(parser);
    this->_setErrorPage(parser);
}

int HttpRequest::_findServerIndex(Parser &parser, int serverSize, std::string port)
{
    std::vector<std::string> listenParam;
    int                      i = 0;

    for (i = 0; i != serverSize; ++i) {
        listenParam = parser.getServerParam(i, "listen");
        if (listenParam.empty()) {
            continue;
        }
        if (port == listenParam[0]) {
            break;
        }
    }
    return (i);
}

int HttpRequest::_findLocationIndex(Parser &parser)
{
    std::vector<std::string>           locationParam;
    std::vector<std::string>::iterator it;
    int                                i = 0;
    std::string                        found, path;

    path  = this->_extractPathFromURI();
    found = path.empty() ? this->_uri : path;
    for (i = 0; i < this->_locationSize; i++) {
        locationParam = parser.getLocationParam(this->_serverIndex, i, "location");
        it            = std::find(locationParam.begin(), locationParam.end(), found);
        if (it != locationParam.end()) {
            break;
        }
    }
    return (i);
}

std::string HttpRequest::_extractPathFromURI(void)
{
    size_t      pos;
    std::string path;

    if (this->_uri.length() == 1)
        return ("");
    pos = this->_uri.find("/", 1);
    if (pos == std::string::npos) {
        path = this->_uri;
    } else {
        path = this->_uri.substr(0, pos);
    }
    return (path);
}

void HttpRequest::_setRoot(Parser &parser)
{
    std::vector<std::string> rootParam;

    this->_root = DEFAULT_ROOT;
    rootParam   = parser.getServerParam(this->_serverIndex, "root");
    if (!rootParam.empty()) {
        this->_root = rootParam[0];
    }
    if (this->_locationSize != this->_locationIndex) {
        rootParam = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "root");
        if (!rootParam.empty()) {
            this->_root = rootParam[0];
        }
    }
    return;
}

void HttpRequest::_setErrorPage(Parser &parser)
{
    if (this->_locationSize != this->_locationIndex) {
        this->_errorPageConfig
            = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "error_page");
        if (!this->_errorPageConfig.empty()) {
            return;
        }
    }
    this->_errorPageConfig = parser.getServerParam(this->_serverIndex, "error_page");
    return;
}
