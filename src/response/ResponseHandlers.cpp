/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:41:36 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 17:15:39 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandlers.hpp"

ResponseHandlers::ResponseHandlers(void) { CGI _cgi; }

ResponseHandlers::~ResponseHandlers(void) {}

responseData ResponseHandlers::exec(Parser &parser, HttpRequest &request)
{
    this->_initResponseData();
    // TODO: check server_Name
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
    std::string resp = _cgi.executeCGI(request, parser);

    location.setup(parser);
    this->_res               = location.getLocationContent();
    this->_res.content       = resp;
    this->_res.contentLength = resp.length();
    this->_res.contentType   = "text/html";
    this->_res.statusCode    = "200 OK";
}

void ResponseHandlers::_postHandler(HttpRequest &request, Parser &parser)
{
    PostMethod post_method(request);

    // TODO: chamar cgi

    std::string resp = _cgi.executeCGI(request, parser);

    this->_res.content       = resp;
    this->_res.contentLength = resp.length();
    this->_res.contentType   = "text/html";
    this->_res.statusCode    = "200 OK";

    post_method.handleMethod(request.getUri());
    // TODO: preciso que retorne o responseData
}

void ResponseHandlers::_deleteHandler(std::string uri)
{
    DeleteMethod delete_method;

    delete_method.handleMethod(uri);
    // TODO: preciso que retorne o responseData
}
