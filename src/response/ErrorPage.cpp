/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 05:34:14 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 18:56:20 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"
#include "Logger.hpp"

ErrorPage::ErrorPage(void) {}

ErrorPage::~ErrorPage(void) {}

responseData ErrorPage::getErrorPageContent(std::vector<std::string> errorPage,
                                            int                      statusCode,
                                            std::string              uri,
                                            std::string              root)
{
    std::string  filePath;
    responseData res;

    if (this->_hasErrorPageConfig(errorPage, statusCode)) {
        filePath = this->_getPath(uri);
        if (filePath[filePath.length() - 1] != '/') {
            filePath += '/';
        }
        filePath += errorPage.back();
        res             = getContent(root, filePath);
        res.status      = statusCode; // TODO: Passar essa atribuição para utils, que nem content
        res.statusCode  = Constants::getStatusCodes(to_string(statusCode));
        res.contentType = "text/html";
        if (res.contentLength) {
            return (res);
        }
    }
    std::string standardErrorFile = "/";
    standardErrorFile += to_string(statusCode);
    standardErrorFile += ".html";
    res             = getContent(DEFAULT_ERROR_ROOT, standardErrorFile);
    res.status      = statusCode;
    res.statusCode  = Constants::getStatusCodes(to_string(statusCode));
    res.contentType = "text/html";
    if (res.contentLength) {
        return (res);
    }
    res = getJson("{\"error\": \"Error page not configured\"}", statusCode);
    return (res);
}

bool ErrorPage::_hasErrorPageConfig(std::vector<std::string> errorPage, int statusCode)
{
    for (std::vector<std::string>::iterator it = errorPage.begin(); it != errorPage.end(); ++it) {
        if (*it == to_string(statusCode)) {
            return (true);
        }
    }
    return (false);
}

std::string ErrorPage::_getPath(std::string uri)
{
    size_t firstSlashPos;
    size_t lastSlashPos;
    size_t dotPos;

    if (uri.length() == 1)
        return ("/");
    firstSlashPos = uri.find('/', 1);
    if (firstSlashPos == std::string::npos) {
        return ("/");
    }
    dotPos = uri.rfind('.');
    if (dotPos == std::string::npos) {
        return (uri);
    }
    lastSlashPos = uri.rfind('/');
    return (uri.substr(0, lastSlashPos));
}
