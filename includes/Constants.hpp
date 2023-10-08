/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:55:15 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/05 22:58:39 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Constants {
    private:
    static const std::map<std::string, std::string> _mimeTypes;
    static const std::map<std::string, std::string> _statusCodes;

    static std::map<std::string, std::string> setMimeTypes();
    static std::map<std::string, std::string> setStatusCodes();

    public:
    Constants();
    ~Constants();

    std::string getMimeTypes(std::string extension);
    std::string getStatusCodes(std::string status);
};
