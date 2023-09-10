/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:49:31 by mcl               #+#    #+#             */
/*   Updated: 2023/09/10 11:06:54 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

typedef std::map<std::string, std::vector<std::string> > params;

typedef struct {
    params* location;
} conf_locations;

typedef struct {
    params* server;
    conf_locations** locations;
} conf_servers;

conf_servers* server(int locs){
    conf_servers* server = new conf_servers;
    server->locations = new conf_locations*[locs];
    return server;
}

std::vector<std::string> split(const std::string str) {
    std::vector<std::string> vtokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        vtokens.push_back(token);
    }
    return vtokens;
}

int countServers(std::ifstream& conf){
    int count = 0;
    while(!conf.eof()){
        std::string line;
        std::getline(conf, line);
        if (line.find("server") != std::string::npos)
            count++;
    }
    return count;
}

std::vector<std::string> extractLocations(const std::string& text) {
    std::vector<std::string> locations;
    std::string keyword = "location ";
    std::string::size_type pos = 0;

    while ((pos = text.find(keyword, pos)) != std::string::npos) {
        // Encontrou a palavra-chave "location"
        pos += keyword.length();
        // Encontre o próximo '}' a partir da posição atual
        std::string::size_type start = pos;
        
        // Verifique se a linha contém apenas "}"
        bool onlyClosingBrace = true;
        while (pos < text.length() && text[pos] != '\n') {
            if (text[pos] != '}' && text[pos] != ' ' && text[pos] != '\t') {
                onlyClosingBrace = false;
                break;
            }
            ++pos;
        }

        if (onlyClosingBrace) {
            // Encontrou uma linha contendo apenas "}", adicione-a ao vetor de locations
            locations.push_back(text.substr(start, pos - start));
        } else {
            while (text[pos] != '}' && pos < text.length()) {
                ++pos;
            }
            if (text[pos] == '}') {
                // Encontrou um bloco válido, adicione-o ao vetor de locations
                locations.push_back(text.substr(start, pos - start + 1));
            }
        }
    }
    return locations;
}

bool verifyBlockEnd(const std::string& text) {
    std::string::size_type pos = 0;
    bool onlyClosingBrace = true;
    int countBrace = 0;
    int countSpace = 0;
        
    while (pos < text.length() && text[pos] != '\n') {
        if (text[pos] != '}' && text[pos] != ' ') {
            onlyClosingBrace = false;
            break;
        }
        if (text[pos] == '}')
            countBrace++;
        if (text[pos] == ' ')
            countSpace++;
        ++pos;
    }
    if (countBrace != 1)
        onlyClosingBrace = false;
    if (countSpace == 0 && countBrace == 1)
        onlyClosingBrace = true;
    return onlyClosingBrace;
}

bool verifyLineEmpty(const std::string& text) {

    bool emptyLine = true;

    for (size_t i = 0; i < text.length(); i++) {
        if (!isspace(text[i])) {
            emptyLine = false;
            return emptyLine;
        }
    }
    return emptyLine;
}

void Parser::getConf(const char* fileconf) {
    std::string line;
    std::vector<std::vector<std::string> > servers;
    std::vector<std::vector<std::string> > locations;
    params vconfs;

    std::ifstream conf(fileconf);
    if (!conf.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }

    std::cout << countServers(conf) << std::endl;
    conf.clear();
    conf.seekg(0, std::ios::beg);

    std::vector<std::string> serverBlocks;
    std::vector<std::string> locationBlocks;
    std::string currentServerBlock;
    std::string currentLocationBlock;
    bool insideServerBlock = false;
    bool insideLocationBlock = false;
    bool blockEnd = false;

    while (!conf.eof()) {
        std::string line;
        std::getline(conf, line);

        if (verifyLineEmpty(line))
            continue;

        if (line.find("server ") != std::string::npos)
            insideServerBlock = true;

        if (line.find("location ") != std::string::npos)
            insideLocationBlock = true;
        
        if(insideServerBlock && !insideLocationBlock)
            currentServerBlock += line + "\n";

        if(insideLocationBlock)
            currentLocationBlock += line + "\n";

        if (insideLocationBlock && verifyBlockEnd(line)) {
            locationBlocks.push_back(currentLocationBlock);
            currentLocationBlock.clear();
            insideLocationBlock = false;
            blockEnd = true;
        }

        if (insideServerBlock && !insideLocationBlock && verifyBlockEnd(line) && blockEnd) {
            serverBlocks.push_back(currentServerBlock);
            currentServerBlock.clear();
            insideServerBlock = false;
            blockEnd = false;
        }

        if (!insideServerBlock && !insideLocationBlock && verifyBlockEnd(line) && blockEnd) {
            servers.push_back(serverBlocks);
            locations.push_back(locationBlocks);
            serverBlocks.clear();
            locationBlocks.clear();
            blockEnd = false;
        }
    }

    conf.close();
}
