/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:41:36 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 08:47:32 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandlers.hpp"

ResponseHandlers::ResponseHandlers(void) { CGI _cgi; }

ResponseHandlers::~ResponseHandlers(void) {}

responseData ResponseHandlers::exec(Parser &parser, HttpRequest &request)
{
    this->_initResponseData();
    // TODO: check server_Name
    if (!this->_verifyServerName(request, parser)) {
        this->_res.statusCode = "404";
        return (this->_res);
    }
    // TODO: check method
    switch (this->_resolveOption(request.getMethod())) {
        case GET:
            this->_getHandler(request, parser);
            break;
        case POST:
            this->_postHandler(request, parser);
            break;
        case DELETE:
            this->_deleteHandler(request.getUri());
            break;
        default:
            break;
    }
    return (this->_res);
}

void ResponseHandlers::_initResponseData(void)
{
    this->_res.statusCode    = "";
    this->_res.contentType   = "";
    this->_res.content       = "";
    this->_res.contentLength = -1;
}

bool ResponseHandlers::_verifyServerName(HttpRequest &request, Parser &parser)
{
    std::vector<std::string> server_names
        = parser.getServerParam(request.getServerIndex(), "server_name");
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

void ResponseHandlers::_getHandler(HttpRequest &request, Parser &parser)
{
    Location location(request);

    // TODO: chamar autoindex
    // TODO: chamar cgi

    if (_cgi.isCGI(request, parser)) {
        std::string cgi_response = _cgi.executeCGI(request);
        this->_res.content       = cgi_response;
        this->_res.contentLength = cgi_response.length();
        this->_res.contentType   = "text/html";
        this->_res.statusCode    = "200 OK";
    } else {
        location.setup(parser);
        this->_res = location.getLocationContent();
    }
}

void ResponseHandlers::_postHandler(HttpRequest &request, Parser &parser)
{
    PostMethod post_method(request);

    // TODO: chamar cgi

    if (_cgi.isCGI(request, parser)) {
        std::string cgi_response = _cgi.executeCGI(request);
        this->_res.content       = cgi_response;
        this->_res.contentLength = cgi_response.length();
        this->_res.contentType   = "text/html";
        this->_res.statusCode    = "200 OK";
    } else {
        post_method.handleMethod(request.getUri());
    }

    // TODO: preciso que retorne o responseData
}

void ResponseHandlers::_deleteHandler(std::string uri)
{
    DeleteMethod delete_method;

    delete_method.handleMethod(uri);
    // TODO: preciso que retorne o responseData
}
