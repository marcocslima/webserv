/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:18:36 by mcl               #+#    #+#             */
/*   Updated: 2023/09/12 05:11:40 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

typedef std::map<std::string, std::vector<std::string> > params;

typedef struct {
    params* location;
} conf_locations;

typedef struct {
    params* server;
    conf_locations** locations;
} conf_servers;

class Parser
{
    private:
        
    public:
        Parser();
        ~Parser();

        void getConf(const char* fileconf);       
};
