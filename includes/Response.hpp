/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:14:37 by mcl               #+#    #+#             */
/*   Updated: 2023/09/19 20:56:06 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Parser.hpp"
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

class HttpResponse {
    private:
        Parser      *_parser;
        Response    _response;
    public:
        HttpResponse();
        HttpResponse(Parser *parser);
        ~HttpResponse();

    std::string getPath(int serverIndex, int locIndex, std::string uri);
    std::string assembleResponse();
    std::string handleDelete(std::string uri);
};
