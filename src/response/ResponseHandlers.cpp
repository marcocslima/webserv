/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:41:36 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 16:07:55 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandlers.hpp"

ResponseHandlers::ResponseHandlers(void) { CGI _cgi; }

ResponseHandlers::~ResponseHandlers(void) {}

responseData ResponseHandlers::exec(Parser &parser, HttpRequest &request)
{
    this->_res = setResponseData(0, "", "", -1);

    if (!this->_verifyServerName(request, parser)) {
        return (this->_errorPage.getErrorPageContent(
            request.getErrorPageConfig(), BAD_REQUEST, request.getUri(), request.getRoot()));
    }
    if (!this->_methodAllowed(request)) {
        return (this->_errorPage.getErrorPageContent(
            request.getErrorPageConfig(), METHOD_NOT_ALLOWED, request.getUri(), request.getRoot()));
    }
    switch (this->_resolveOption(request.getMethod())) {
        case GET:
            this->_getHandler(request, parser);
            break;
        case POST:
            this->_postHandler(request, parser);
            break;
        case DELETE:
            this->_deleteHandler(request);
            break;
        default:
            break;
    }
    return (this->_res);
}

bool ResponseHandlers::_methodAllowed(HttpRequest &request)
{
    if (request.getLimitExcept().empty()) {
        return (true);
    }
    for (size_t i = 0; i < request.getLimitExcept().size(); i++) {
        if (request.getLimitExcept()[i] == request.getMethod()) {
            return (true);
        }
    }
    return (false);
}

bool ResponseHandlers::_verifyServerName(HttpRequest &request, Parser &parser)
{
    std::vector<std::string> server_names
        = parser.getServerParam(request.getServerIndex(), "server_name");
    if (server_names.empty())
        return (true);
    for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end();
         ++it)
        if (*it == request.getHost())
            return (true);
    return (false);
}

int ResponseHandlers::_resolveOption(std::string method)
{
    std::string option[] = {"GET", "POST", "DELETE"};
    int         i        = 0;

    while (i < 3 && method != option[i])
        i++;
    return (i);
}

responseData ResponseHandlers::_getCgi(HttpRequest &request)
{
    // CGI          cgi;
    responseData res;

    std::string cgi_response = _cgi.executeCGI(request);
    res = setResponseData(OK, "text/html", cgi_response.c_str(), (int)cgi_response.length());
    return (res);
}

void ResponseHandlers::_getHandler(HttpRequest &request, Parser &parser)
{
    Location location(request);

    // TODO: chamar autoindex

    if (Constants::isCgi(extractFileExtension(request.getUri()))) {
        if (_cgi.isCGI(request, parser))
            this->_res = this->_getCgi(request);
    } else {
        location.setup(parser);
        this->_res = location.getLocationContent();
    }
}

void ResponseHandlers::_postHandler(HttpRequest &request, Parser &parser)
{
    PostMethod post_method(request);

    if (Constants::isCgi(extractFileExtension(request.getUri()))) {
        if (_cgi.isCGI(request, parser))
            this->_res = this->_getCgi(request);
    } else
        this->_res = post_method.handleMethod();
}

void ResponseHandlers::_deleteHandler(HttpRequest &request)
{
    DeleteMethod delete_method(request);

    Logger::warning << "Delete Method" << std::endl;

    this->_res = delete_method.handleMethod();
}
