/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:09:25 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/04 01:14:59 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) : _root(DEFAULT_ROOT)
{
    this->_responseData.content       = "";
    this->_responseData.statusCode    = "";
    this->_responseData.contentType   = "";
    this->_responseData.contentLength = 0;
}

Location::~Location(void) {}

void Location::setup(Parser &parser, std::string port, std::string uri)
{
    std::vector<int> serverSize = parser.getSizeServers();

    this->_serverIndex = this->_findServerIndex(parser, serverSize[0], port);
    this->_setRoot(parser);
    this->_locationSize               = serverSize[this->_serverIndex + 1];
    this->_locationIndex              = this->_findLocationIndex(parser, uri);
    this->_serverIndexPage            = parser.getServerParam(this->_serverIndex, "index");
    this->_serverErrorPage            = parser.getServerParam(this->_serverIndex, "error_page");
    this->_responseData.content       = "";
    this->_responseData.statusCode    = "";
    this->_responseData.contentType   = "";
    this->_responseData.contentLength = 0;
}

int Location::_findServerIndex(Parser &parser, int serverSize, std::string port)
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

int Location::_findLocationIndex(Parser &parser, std::string uri)
{
    std::vector<std::string>           locationParam;
    std::vector<std::string>::iterator it;
    int                                i = 0;

    for (i = 0; i < this->_locationSize; i++) {
        locationParam = parser.getLocationParam(this->_serverIndex, i, "location");
        it            = std::find(locationParam.begin(), locationParam.end(), uri);
        if (it != locationParam.end()) {
            break;
        }
    }
    return (i);
}

void Location::_setRoot(Parser &parser)
{
    std::vector<std::string> rootParam;

    rootParam = parser.getServerParam(this->_serverIndex, "root");
    if (!rootParam.empty()) {
        this->_root = rootParam[0];
        return;
    }
    this->_root = DEFAULT_ROOT;
}

responseData Location::getLocationContent(void)
{
    if (this->_locationSize == this->_locationIndex) {
        this->_getServerIndexContent();
    }
    return (this->_responseData);
}

void Location::_getServerIndexContent(void)
{
    if (this->_serverIndexPage.empty()) {
        this->_getServerErrorPageContent();
        return;
    }
    this->_getContent(this->_serverIndexPage[0]);
    this->_responseData.statusCode  = "200 OK";
    this->_responseData.contentType = "text/html";
    if (!this->_responseData.contentLength) {
        this->_responseData.statusCode = "500 Internal Server Error";
        this->_getJson("{\"error\": \"Request Failed\"}");
    }
}

void Location::_getServerErrorPageContent(void)
{
    this->_responseData.statusCode = "404 Not Found";
    if (this->_serverErrorPage.size() != 2) {
        this->_getJson("{\"error\": \"Resource not found\"}");
        return;
    }
    if (this->_serverErrorPage[0] != "404") {
        this->_getJson("{\"error\": \"404 error page not configured\"}");
        return;
    }
    this->_getContent(this->_serverErrorPage[1]);
    this->_responseData.contentType = "text/html";
    if (!this->_responseData.contentLength) {
        this->_getJson("{\"error\": \"Unable to open 404 error page\"}");
    }
}

void Location::_getJson(std::string body)
{
    this->_responseData.content       = body;
    this->_responseData.contentLength = (int)body.length();
    this->_responseData.contentType   = "application/json";
}

void Location::_getContent(std::string file)
{
    std::stringstream fullPathStream;
    std::string       fullPath;
    const char       *fullPathCStr;

    fullPathStream << this->_root << "/" << file;
    fullPath     = fullPathStream.str();
    fullPathCStr = fullPath.c_str();
    std::ifstream ifs(fullPathCStr);
    if (ifs.is_open()) {
        std::string content((std::istreambuf_iterator<char>(ifs)),
                            std::istreambuf_iterator<char>());
        this->_responseData.content       = content;
        this->_responseData.contentLength = (int)content.length();
        ifs.close();
    }
}
