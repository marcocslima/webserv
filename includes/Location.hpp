/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:07 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/04 00:24:43 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
    std::vector<std::string> _serverIndexPage;
    std::vector<std::string> _serverErrorPage;
    responseData             _responseData;

    int  _findServerIndex(Parser &parser, int serverSize, std::string port);
    int  _findLocationIndex(Parser &parser, std::string uri);
    void _setRoot(Parser &parser);
    void _getServerIndexContent(void);
    void _getServerErrorPageContent(void);
    void _getJson(std::string body);
    void _getContent(std::string file);

    public:
    Location(void);
    ~Location(void);

    void         setup(Parser &parser, std::string port, std::string uri);
    responseData getLocationContent(void);
};
