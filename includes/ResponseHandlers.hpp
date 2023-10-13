/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:42:45 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 16:01:33 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "DeleteMethod.hpp"
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

// TODO: consumir do .conf os métodos permitidos

class ResponseHandlers {
    private:
    responseData _res;

    int  _resolveOption(std::string method);
    void _getHandler(HttpRequest &request, Parser &parser);
    void _postHandler(HttpRequest &request);
    void _deleteHandler(HttpRequest &request);

    public:
    ResponseHandlers(void);
    ~ResponseHandlers(void);

    responseData exec(Parser &parser, HttpRequest &request);
};
