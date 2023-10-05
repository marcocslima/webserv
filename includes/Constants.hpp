/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:55:15 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/04 19:04:21 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libs.hpp"

class Constants {
    public:
    Constants();
    ~Constants();

    static std::map<std::string, std::string> getMimeTypes();
    static std::map<std::string, std::string> getStatusCodes();
};
