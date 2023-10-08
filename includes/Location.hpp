/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:07 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/07 22:07:07 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Constants.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "libs.hpp"

struct responseData {
    std::string content;
    std::string statusCode;
    std::string contentType;
    int         contentLength;
};

class Location {
    private:
    int                      _serverIndex;
    int                      _locationIndex;
    int                      _locationSize;
    std::string              _root;
    std::string              _indexPage;
    std::vector<std::string> _errorPage;
    responseData             _responseData;
    std::string              _uri;
    std::string              _extension;

    int         _findServerIndex(Parser &parser, int serverSize, std::string port);
    int         _findLocationIndex(Parser &parser, std::string uri);
    std::string _extractFileExtension(std::string uri);
    std::string _extractPathFromURI(std::string uri);
    void        _setRoot(Parser &parser);
    void        _setIndexPage(Parser &parser);
    void        _setErrorPage(Parser &parser);
    void        _getFileContent(Constants &constants);
    void        _getIndexContent(Constants &constants);
    void        _getErrorPageContent(Constants &constants);
    void        _getJson(std::string body);
    void        _getContent(std::string file);

    public:
    Location(void);
    ~Location(void);

    void         setup(Parser &parser, std::string port, std::string uri);
    responseData getLocationContent(Constants &constants);
};
