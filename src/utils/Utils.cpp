/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 01:59:27 by mcl               #+#    #+#             */
/*   Updated: 2023/10/04 17:18:15 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

std::string getDir () {
    char cwd[1024];
    if (getcwd(cwd, 1024) != NULL) {
        std::string dir(cwd);
        size_t pos = dir.find("webserv");
        if (pos != std::string::npos)
            dir = dir.substr(0, pos + 7);
        return dir;
    }
    else {
        std::cout << "Error getting current working directory" << std::endl;
        return "";
    }
}

// std::string getPath(int serverIndex, int locIndex, std::string uri) {
    
//     std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
//     std::string path = rootDir[0] + uri;

//     if (path[path.length() - 1] != '/')
//         path += '/';

//     return path;
// }