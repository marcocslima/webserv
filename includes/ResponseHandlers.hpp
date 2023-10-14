/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:42:45 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 09:09:15 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Cgi.hpp"
#include "Constants.hpp"
#include "DeleteMethod.hpp"
#include "ErrorPage.hpp"
#include "Libs.hpp"
#include "Location.hpp"
#include "Parser.hpp"
#include "PostMethod.hpp"
#include "Request.hpp"

typedef enum e_method_option {
    GET,
    POST,
    DELETE,
} t_method_option;

class ResponseHandlers {
    private:
    responseData _res;
    CGI          _cgi;
    ErrorPage    _errorPage;

    int          _resolveOption(std::string method);
    bool         _verifyServerName(HttpRequest &request, Parser &parser);
    bool         _methodAllowed(HttpRequest &request);
    responseData _getCgi(HttpRequest &request);
    void         _getHandler(HttpRequest &request, Parser &parser);
    void         _postHandler(HttpRequest &request, Parser &parser);
    void         _deleteHandler(HttpRequest &request);

    public:
    ResponseHandlers(void);
    ~ResponseHandlers(void);

    responseData exec(Parser &parser, HttpRequest &request);
};
