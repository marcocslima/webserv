/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/28 01:09:52 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include "Parser.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class HttpRequest {
    public:
    HttpRequest();
    ~HttpRequest();

    void requestHttp(std::string request, Parser &parser);
    void initMethods();

    std::string                        getMethod(void) const;
    std::string                        getPort(void) const;
    std::string                        getUri(void) const;
    std::string                        getHttp(void) const;
    std::map<std::string, std::string> getHeaders(void);

    private:
    bool _parseHttpRequest(const std::string &request, std::map<std::string, std::string> &headers);
    bool _checkFirstLine(std::string &requestLine);
    void _checkLocations(Parser &parser);
    void _checkPorts(Parser &parser);

    std::string                        _requestLine;
    std::string                        _method;
    std::string                        _uri;
    std::string                        _httpVersion;
    std::string                        _port;
    std::vector<std::string>           _allowMethods;
    std::map<std::string, std::string> _headers;
};
