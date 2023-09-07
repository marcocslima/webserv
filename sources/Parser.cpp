/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:49:31 by mcl               #+#    #+#             */
/*   Updated: 2023/09/07 11:09:35 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

std::vector<std::string> split(const std::string str) {
    std::vector<std::string> vtokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        vtokens.push_back(token);
    }
    return vtokens;
}

void Parser::getFile(const char* fileconf) {
    std::string line;
    std::vector<std::string> ckey;
    std::vector<std::string> cvalues;
    std::vector<std::pair<std::string, std::vector<std::string> > > vconfs;

    std::ifstream conf(fileconf);
    if (!conf.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }

    while (std::getline(conf, line)) {
        ckey = split(line);
        if (line.length() == 0 || ckey[0][0] == '}' || ckey[1][0] == '{')
            continue;
        for (size_t i = 1; i < ckey.size(); ++i) {
            cvalues.push_back(ckey[i]);
        }
        if (cvalues[cvalues.size() - 1][0] == '}' || cvalues[cvalues.size() - 1][0] == '{')
            cvalues.pop_back();
        vconfs.push_back(std::make_pair(ckey[0], cvalues));
        cvalues.clear();
    }
    
    for (size_t i = 0; i < vconfs.size(); ++i) {
        std::cout << "Chave: " << vconfs[i].first << std::endl;
        std::cout << "Valores:";
        for (size_t j = 0; j < vconfs[i].second.size(); ++j) {
            std::cout << " " << vconfs[i].second[j];
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    conf.close();
}