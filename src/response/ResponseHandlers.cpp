/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:41:36 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/12 15:37:50 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandlers.hpp"

ResponseHandlers::ResponseHandlers(void) {}

ResponseHandlers::~ResponseHandlers(void) {}

responseData ResponseHandlers::exec(Parser &parser, HttpRequest &request)
{
    this->_initResponseData();
    // TODO: check server_Name
    // TODO: check method
    switch (this->_resolveOption(request.getMethod())) {
        case GET:
            this->_getHandler(parser, request.getPort(), request.getUri());
            break;
        case POST:
            this->_postHandler(request);
        case DELETE:
            this->_deleteHandler(request.getUri());
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

void ResponseHandlers::_getHandler(Parser &parser, std::string port, std::string uri)
{
    Location location;

    // TODO: chamar autoindex
    // TODO: chamar cgi
    location.setup(parser, port, uri);
    this->_res = location.getLocationContent();
}

void ResponseHandlers::_postHandler(HttpRequest &request)
{
    PostMethod post_method(request);

    // TODO: chamar cgi
    post_method.handleMethod(request.getUri());
    // TODO: preciso que retorne o responseData
}

void ResponseHandlers::_deleteHandler(std::string uri)
{
    DeleteMethod delete_method;

    delete_method.handleMethod(uri);
    // TODO: preciso que retorne o responseData
}
