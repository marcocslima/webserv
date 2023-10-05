/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:07 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/04 20:07:40 by pmitsuko         ###   ########.fr       */
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
    static const std::map<std::string, std::string> _mimeTypes;

    int                      _serverIndex;
    int                      _locationIndex;
    int                      _locationSize;
    std::string              _root;
    std::vector<std::string> _serverIndexPage;
    std::vector<std::string> _serverErrorPage;
    responseData             _responseData;
    std::string              _uri;
    std::string              _extension;

    int         _findServerIndex(Parser &parser, int serverSize, std::string port);
    int         _findLocationIndex(Parser &parser, std::string uri);
    std::string _extractFileExtension(std::string uri);
    void        _setRoot(Parser &parser);
    void        _getFileContent(void);
    void        _getServerIndexContent(void);
    void        _getServerErrorPageContent(void);
    void        _getJson(std::string body);
    void        _getContent(std::string file);

    public:
    Location(void);
    ~Location(void);

    void         setup(Parser &parser, std::string port, std::string uri);
    responseData getLocationContent(void);
};
