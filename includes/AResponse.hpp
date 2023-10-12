/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 06:59:01 by mcl               #+#    #+#             */
/*   Updated: 2023/10/12 05:24:03 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Parser.hpp"
#include "Request.hpp"

struct Response {
    std::string version;
    std::string status_code;
    std::string status_message;
    std::string content_type;
    std::string content_length;
    std::string body;
};

class AHttpResponse {
    protected:
    Parser  *_parser;
    Response _response;

    public:
    AHttpResponse();
    AHttpResponse(Parser *parser);
    virtual ~AHttpResponse();

    virtual std::string getPath(int serverIndex, int locIndex, std::string uri);
    virtual std::string assembleResponse();
    virtual std::string handleMethod(std::string uri) = 0;
    virtual bool        verifyServerNames(const HttpRequest &request, Parser &parser);
};

std::vector<std::string> getServerNames(Parser &parser, std::string port);
