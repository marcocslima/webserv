/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:42:45 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 11:58:24 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "DeleteMethod.hpp"
#include "Libs.hpp"
#include "Location.hpp"
#include "Parser.hpp"
#include "PostMethod.hpp"
#include "AutoIndex.hpp"
#include "Request.hpp"

typedef enum e_method_option {
    GET,
    POST,
    DELETE,
} t_method_option;

// TODO: consumir do .conf os métodos permitidos

class ResponseHandlers {
    private:
    responseData _res;

    void _initResponseData(void);
    int  _resolveOption(std::string method);
    void _getHandler(HttpRequest &request, Parser &parser);
    void _postHandler(HttpRequest &request);
    void _deleteHandler(std::string uri);

    public:
    ResponseHandlers(void);
    ~ResponseHandlers(void);

    responseData exec(Parser &parser, HttpRequest &request);
};
