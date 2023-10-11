/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/11 16:05:06 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Logger.hpp"
#include "Parser.hpp"

class HttpRequest {
    public:
    HttpRequest();
    ~HttpRequest();

    void requestHttp(std::string request, Parser &parser);
    void init();

    std::string                        getMethod(void) const;
    std::string                        getPort(void) const;
    std::string                        getUri(void) const;
    std::string                        getHttp(void) const;
    std::string                        getBody(void) const;
    std::string                        getBoundary(void) const;
    std::vector<std::string>           getQuery(void) const;
    std::map<std::string, std::string> getHeaders(void) const;

    bool _has_body;
    bool _has_form;
    bool _tooLarge;
    bool _has_multipart;
    class RequestException : public std::exception {
        public:
        virtual const char *what() const throw();
    };

    private:
    void _parseHeaders(const std::string &request);
    void _parseFirstLine(std::string &requestLine);
    void _findHeaders(std::string key, std::string value);
    void _parseQuery(void);
    void _getMultipartData(std::string request);
    void _getMaxBody(Parser &parser);
    void _checkLocations(Parser &parser);
    void _checkPorts(Parser &parser);
    void _getBody(std::string request);

    size_t                             _contentLength;
    std::string                        _uri;
    std::string                        _body;
    std::string                        _port;
    std::string                        _method;
    std::string                        _boundary;
    std::string                        _httpVersion;
    std::string                        _statusError;
    std::vector<std::string>           _paramQuery;
    std::vector<std::string>           _allowMethods;
    std::map<std::string, std::string> _header;
};
