/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:49:31 by mcl               #+#    #+#             */
/*   Updated: 2023/09/12 04:39:22 by mcl              ###   ########.fr       */
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

conf_servers* allocateServer(int locs) {
    conf_servers* server = new conf_servers;
    server->server = new params;
    server->locations = new conf_locations*[locs];

    for (int i = 0; i < locs; i++) {
        server->locations[i] = new conf_locations;
        server->locations[i]->location = new params;
    }
    return server;
}

std::vector<std::string> splitTokens(const std::string str) {
    
    std::vector<std::string>    vtokens;
    std::istringstream          iss(str);
    std::string                 token;

    while (iss >> token) {
        if (token == "{" || token == "}")
            continue;
        vtokens.push_back(token);
    }
    
    return vtokens;
}

std::string removeExtraSpaces(const std::string& input) {
    std::string result;
    bool previousCharWasSpace = false;

    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] != ' ') {
            result += input[i];
            previousCharWasSpace = false;
        } else if (!previousCharWasSpace) {
            result += ' ';
            previousCharWasSpace = true;
        }
    }
    if (result[0] == ' ') {
        result.erase(0, 1);
    }
    if (!result.empty() && result[result.length() - 1] == ' ') {
        result.erase(result.length() - 1);
    }

    return result;
}

params* getParams(const std::string str, params* vconfs) {
 
    std::vector<std::string>    tokens;
    std::string                 tmp_str = str;
    std::string                 token;

    size_t pos = str.find('\n');
    while (pos != std::string::npos) {
        token = tmp_str.substr(0, pos);
        tokens.push_back(removeExtraSpaces(token));
        tmp_str = tmp_str.substr(pos + 1);
        pos = tmp_str.find('\n');
    }
    for (size_t i = 0; i < tokens.size(); i++) {
        pos = tokens[i].find(' ');
        if (pos != std::string::npos) {
            std::string key = tokens[i].substr(0, pos);
            std::vector<std::string> value = splitTokens(tokens[i].substr(pos + 1));
            if (key != "server")
                (*vconfs)[key] = value;
        }
    }
        
    return vconfs;
}

bool verifyBlockEnd(const std::string& text) {
    
    std::string::size_type  pos = 0;
    bool                    onlyClosingBrace = true;
    int                     countBrace = 0;
    int                     countSpace = 0;
        
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
    
    std::string                             line;
    std::vector<std::vector<std::string> >  servers;
    std::vector<std::vector<std::string> >  locations;
    params                                  vconfs;
    std::vector<std::string>                serverBlocks;
    std::vector<std::string>                locationBlocks;
    std::string                             currentServerBlock;
    std::string                             currentLocationBlock;
    bool                                    insideServerBlock = false;
    bool                                    insideLocationBlock = false;
    bool                                    blockEnd = false;

    std::ifstream conf(fileconf);
    if (!conf.is_open()) {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }
    conf.clear();
    conf.seekg(0, std::ios::beg);

    while (!conf.eof()) {
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

        if (verifyBlockEnd(line)){
            if (insideLocationBlock){
                locationBlocks.push_back(currentLocationBlock);
                currentLocationBlock.clear();
                insideLocationBlock = false;
            } else if (insideServerBlock && !insideLocationBlock){
                serverBlocks.push_back(currentServerBlock);
                currentServerBlock.clear();
                insideServerBlock = false;
                blockEnd = true;
            }
        }

        if (!insideServerBlock && !insideLocationBlock && blockEnd) {
            servers.push_back(serverBlocks);
            locations.push_back(locationBlocks);
            serverBlocks.clear();
            locationBlocks.clear();
            blockEnd = false;
        }
    }
    conf.close();

    conf_servers* cservers = new conf_servers[servers.size()];

    for (size_t i = 0; i < servers.size(); i++) {
        cservers[i] = *allocateServer(locations[i].size());
        cservers[i].server = getParams(servers[i][0], cservers[i].server);
        for (size_t j = 0; j < locations[i].size(); j++) {
            cservers[i].locations[j]->location = getParams(locations[i][j], cservers[i].locations[j]->location);
        }        
    }

    // for (params::iterator it = cservers->server->begin(); it != cservers->server->end(); ++it) {
    //     std::cout << "Key: " << it->first << ", Values: ";
    //     for (size_t i = 0; i < it->second.size(); ++i) {
    //         std::cout << it->second[i] << " ";
    //     }
    //     std::cout << std::endl;
    // }
}
