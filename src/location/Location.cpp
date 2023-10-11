/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:09:25 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/10 18:26:42 by pmitsuko         ###   ########.fr       */
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

    this->_uri                        = uri;
    this->_responseData.content       = "";
    this->_responseData.statusCode    = "";
    this->_responseData.contentType   = "";
    this->_responseData.contentLength = 0;
    this->_serverIndex                = this->_findServerIndex(parser, serverSize[0], port);
    this->_locationSize               = serverSize[this->_serverIndex + 1];
    this->_locationIndex              = this->_findLocationIndex(parser, uri);
    this->_extension                  = this->_extractFileExtension(uri);
    this->_setRoot(parser);
    this->_setIndexPage(parser);
    this->_setErrorPage(parser);
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
    std::string                        found, path;

    path  = this->_extractPathFromURI(uri);
    found = path.empty() ? uri : path;
    for (i = 0; i < this->_locationSize; i++) {
        locationParam = parser.getLocationParam(this->_serverIndex, i, "location");
        it            = std::find(locationParam.begin(), locationParam.end(), found);
        if (it != locationParam.end()) {
            break;
        }
    }
    return (i);
}

std::string Location::_extractPathFromURI(std::string uri)
{
    size_t      pos;
    std::string path;

    if (uri.length() == 1)
        return ("");
    pos = uri.find("/", 1);
    if (pos == std::string::npos) {
        path = uri;
    } else {
        path = uri.substr(0, pos);
    }
    return (path);
}

std::string Location::_extractFileExtension(std::string uri)
{
    size_t dotPos = uri.rfind('.');

    if (dotPos != std::string::npos) {
        std::string extension = uri.substr(dotPos);
        return extension;
    }
    return "";
}

void Location::_setRoot(Parser &parser)
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

void Location::_setIndexPage(Parser &parser)
{
    std::vector<std::string> indexParam;

    if (this->_locationSize != this->_locationIndex) {
        indexParam = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "index");
        if (!indexParam.empty()) {
            this->_indexPage = indexParam[0];
            return;
        }
    }
    indexParam       = parser.getServerParam(this->_serverIndex, "index");
    this->_indexPage = indexParam.empty() ? "index.html" : indexParam[0];
    return;
}

void Location::_setErrorPage(Parser &parser)
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

responseData Location::getLocationContent(void)
{
    if (this->_extension.length()) {
        this->_getFileContent();
    } else {
        this->_getIndexContent();
    }
    return (this->_responseData);
}

void Location::_getFileContent(void)
{
    this->_responseData             = getContent(this->_root, this->_uri);
    this->_responseData.statusCode  = Constants::getStatusCodes("200");
    this->_responseData.contentType = Constants::getMimeTypes(this->_extension);
    if (!this->_responseData.contentLength) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_errorPageConfig, "404", this->_uri, this->_root);
        return;
    }
    return;
}

void Location::_getIndexContent(void)
{
    std::string indexPath, extension;

    if (this->_indexPage.empty()) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_errorPageConfig, "404", this->_uri, this->_root);
        return;
    }
    if (this->_uri[this->_uri.length() - 1] != '/') {
        this->_uri += '/';
    }
    indexPath                       = this->_uri + this->_indexPage;
    this->_responseData             = getContent(this->_root, indexPath);
    this->_responseData.statusCode  = Constants::getStatusCodes("200");
    extension                       = this->_extractFileExtension(indexPath);
    this->_responseData.contentType = Constants::getMimeTypes(extension);
    if (!this->_responseData.contentLength) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_errorPageConfig, "403", this->_uri, this->_root);
    }
    return;
}
