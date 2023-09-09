/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:49:31 by mcl               #+#    #+#             */
/*   Updated: 2023/09/09 01:01:18 by mcl              ###   ########.fr       */
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


void printLocations(const std::vector<std::string>& locations) {
    for (size_t i = 0; i < locations.size(); ++i) {
        std::cout << "Location " << i + 1 << ":\n";
        std::cout << locations[i] << "\n---\n";
    }
}


void Parser::getConf(const char* fileconf) {
    std::string line;
    std::vector<std::string> ckey;
    std::vector<std::string> cvalues;
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

    while (!conf.eof()) {
        std::string line;
        std::getline(conf, line);

        if (line.find("server ") != std::string::npos) {
            if (insideServerBlock) {
                serverBlocks.push_back(currentServerBlock);
                currentServerBlock.clear();
            }
            insideServerBlock = true;
            insideLocationBlock = false;
        }
        if(insideServerBlock)
            currentServerBlock += line + "\n";

        if (line.find("location ") != std::string::npos) {
            if (insideLocationBlock) {
                locationBlocks.push_back(line);
                currentLocationBlock.clear();
            }
            insideLocationBlock = true;
            insideServerBlock = false;
        }

        if(insideLocationBlock)
            currentLocationBlock += line + "\n";

        if (line == "}") {
            insideLocationBlock = false;
        }
        
    }

    // Agora, serverBlocks contém os conteúdos de cada bloco "server", e locationBlocks contém os blocos "location"
    for (size_t i = 0; i < serverBlocks.size(); ++i) {
        std::cout << "Bloco " << i + 1 << ":\n";
        std::cout << serverBlocks[i] << "\n";

        std::cout << "Blocos 'location' dentro do Bloco " << i + 1 << ":\n";
        for (size_t j = 0; j < locationBlocks.size(); ++j) {
            std::cout << locationBlocks[j] << "\n";
        }

        std::cout << "---\n";
    }

    conf.close();
}
