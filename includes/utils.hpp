/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:09:00 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 17:31:00 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Parser.hpp"

responseData getJson(std::string body);
responseData getContent(std::string root, std::string file);
std::string  extractFileExtension(std::string file);
std::string  getDir();
std::string  getBin(const std::string &url);
