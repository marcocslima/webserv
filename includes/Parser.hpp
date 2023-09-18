/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:18:36 by mcl               #+#    #+#             */
/*   Updated: 2023/09/18 15:12:16 by jefernan         ###   ########.fr       */
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
    params* server;
    params** locations;
} conf_servers;

class Parser
{
    private:
        conf_servers*       _cservers;
        int                 _servers;
        std::vector<int>    _locs;
    public:
        Parser();
        Parser(const char* fileconf);
        ~Parser();

        void setConfs(const char* fileconf);
        void allocateServers(conf_servers* server, int locs);
        void deallocateServers(conf_servers* server, int locs);
        void populateConfs(std::vector<std::vector<std::string> > servers, std::vector<std::vector<std::string> > locations);
        std::vector<std::string> getServerParam (int server, std::string param);
        std::vector<std::string> getLocationParam (int server, int location, std::string param);
        std::vector<int> getSizeServers ();
};

std::vector<std::string> splitTokens(const std::string str);
std::string removeExtraSpaces(const std::string& input);
params* setParams(const std::string str, params* vconfs);
bool verifyBlockEnd(const std::string& text);
bool verifyLineEmpty(const std::string& text);
