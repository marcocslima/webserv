/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:14:37 by mcl               #+#    #+#             */
/*   Updated: 2023/09/19 01:56:44 by mcl              ###   ########.fr       */
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

class HttpResponse {
    private:
        Parser *_parser;
    public:
        HttpResponse();
        HttpResponse(Parser *parser);
        ~HttpResponse();

    std::string getPath(int serverIndex, int locIndex, std::string uri);
};
