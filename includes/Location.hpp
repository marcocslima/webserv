/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:07 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 12:11:32 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Constants.hpp"
#include "ErrorPage.hpp"
#include "Libs.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "Request.hpp"
#include "utils.hpp"

class Location {
    private:
    std::string  _indexPage;
    std::string  _uriExtension;
    HttpRequest  _req;
    responseData _responseData;
    ErrorPage    _errorPage;

    void _setIndexPage(Parser &parser);
    void _getFileContent(void);
    void _getIndexContent(void);

    public:
    Location(void);
    Location(HttpRequest request);
    ~Location(void);

    void         setup(Parser &parser);
    responseData getLocationContent(void);
};
