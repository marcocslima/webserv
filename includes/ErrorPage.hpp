/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 05:35:10 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 16:56:14 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "utils.hpp"

class ErrorPage {
    private:
    bool        _hasErrorPageConfig(std::vector<std::string> errorPage, int statusCode);
    std::string _getPath(std::string uri);

    public:
    ErrorPage(void);
    ~ErrorPage(void);

    responseData getErrorPageContent(std::vector<std::string> errorPage,
                                     int                      statusCode,
                                     std::string              uri,
                                     std::string              root);
};
