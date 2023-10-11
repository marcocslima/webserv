/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 06:59:01 by mcl               #+#    #+#             */
/*   Updated: 2023/10/06 19:24:44 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Parser.hpp"
#include "../includes/Logger.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <unistd.h>

struct Response
{
    std::string version; // HTTP/1.1
    std::string status_code; // 200
    std::string status_message; // OK
    std::string content_type; // text/html
    std::string content_length;
    std::string body;
};

class AHttpResponse {
    protected:
        Parser      *_parser;
        Response    _response;
    public:
        AHttpResponse();
        AHttpResponse(Parser *parser);
        virtual ~AHttpResponse();

    virtual std::string getPath(int serverIndex, int locIndex, std::string uri);
    virtual std::string assembleResponse();
    virtual std::string handleMethod(std::string uri) = 0;
};