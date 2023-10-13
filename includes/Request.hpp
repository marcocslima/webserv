/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/13 12:19:38 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "utils.hpp"

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
    int                                getServerIndex(void) const;
    int                                getLocationIndex(void) const;
    int                                getLocationSize(void) const;
    std::string                        getRoot(void) const;
    std::vector<std::string>           getErrorPageConfig(void) const;

    bool _has_body;
    bool _has_form;
    bool _tooLarge;
    bool _has_multipart;
    class RequestException : public std::exception {
        public:
        virtual const char *what() const throw();
    };

    private:
    void        _parseHeaders(const std::string &request);
    void        _parseFirstLine(std::string &requestLine);
    void        _findHeaders(std::string key, std::string value);
    void        _parseQuery(void);
    void        _getMultipartData(std::string request);
    void        _getMaxBody(Parser &parser);
    void        _getServerParam(Parser &parser);
    void        _getBody(std::string request);
    int         _findServerIndex(Parser &parser, int serverSize, std::string port);
    int         _findLocationIndex(Parser &parser);
    std::string _extractPathFromURI(void);
    void        _setRoot(Parser &parser);
    void        _setErrorPage(Parser &parser);

    int                                _contentLength;
    int                                _maxBodySize;
    std::string                        _uri;
    std::string                        _body;
    std::string                        _port;
    std::string                        _method;
    std::string                        _boundary;
    std::string                        _httpVersion;
    std::string                        _statusError;
    std::vector<std::string>           _paramQuery;
    std::map<std::string, std::string> _header;
    int                                _serverIndex;
    int                                _locationIndex;
    int                                _locationSize;
    std::string                        _root;
    std::vector<std::string>           _errorPageConfig;
};
