/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 01:53:02 by mcl               #+#    #+#             */
/*   Updated: 2023/10/06 04:57:05 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Parser.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <unistd.h>

std::string getDir ();
std::string getBin(const std::string& url);
