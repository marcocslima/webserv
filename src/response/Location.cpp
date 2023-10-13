/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:09:25 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 19:52:48 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// TODO: verificar se é definido o status de responseData

Location::Location(void) {}

Location::Location(HttpRequest request) : _req(request) {}

Location::~Location(void) {}

void Location::setup(Parser &parser)
{
    this->_responseData = setResponseData(0, "", "", 0);
    this->_uriExtension = extractFileExtension(this->_req.getUri());
    this->_setIndexPage(parser);
}

void Location::_setIndexPage(Parser &parser)
{
    std::vector<std::string> indexParam;

    if (this->_req.getLocationSize() != this->_req.getLocationIndex()) {
        indexParam = parser.getLocationParam(
            this->_req.getServerIndex(), this->_req.getLocationIndex(), "index");
        if (!indexParam.empty()) {
            this->_indexPage = indexParam[0];
            return;
        }
    }
    indexParam       = parser.getServerParam(this->_req.getServerIndex(), "index");
    this->_indexPage = indexParam.empty() ? "index.html" : indexParam[0];
    return;
}

responseData Location::getLocationContent(void)
{

    if (this->_uriExtension.length()) {
        this->_getFileContent();
    } else {
        this->_getIndexContent();
    }
    return (this->_responseData);
}

void Location::_getFileContent(void)
{
    this->_responseData             = getContent(this->_req.getRoot(), this->_req.getUri());
    this->_responseData.statusCode  = Constants::getStatusCodes(to_string(OK));
    this->_responseData.contentType = Constants::getMimeTypes(this->_uriExtension);
    if (!this->_responseData.contentLength) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_req.getErrorPageConfig(), NOT_FOUND, this->_req.getUri(), this->_req.getRoot());
        return;
    }
    return;
}

void Location::_getIndexContent(void)
{
    std::string indexPath, extension, uri;

    if (this->_indexPage.empty()) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_req.getErrorPageConfig(), NOT_FOUND, this->_req.getUri(), this->_req.getRoot());
        return;
    }
    uri = this->_req.getUri();
    if (uri[uri.length() - 1] != '/') {
        uri += '/';
    }
    indexPath                       = uri + this->_indexPage;
    this->_responseData             = getContent(this->_req.getRoot(), indexPath);
    this->_responseData.statusCode  = Constants::getStatusCodes(to_string(OK));
    this->_responseData.contentType = Constants::getMimeTypes(extractFileExtension(indexPath));
    if (!this->_responseData.contentLength) {
        this->_responseData = this->_errorPage.getErrorPageContent(
            this->_req.getErrorPageConfig(), FORBIDDEN, uri, this->_req.getRoot());
    }
    return;
}
