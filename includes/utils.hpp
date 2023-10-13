/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:09:00 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 11:41:39 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"

responseData getJson(std::string body);
responseData getContent(std::string root, std::string file);
std::string  extractFileExtension(std::string file);
