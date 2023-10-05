/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 01:59:27 by mcl               #+#    #+#             */
/*   Updated: 2023/10/04 21:16:34 by mcl              ###   ########.fr       */
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

std::string getBin(const std::string& url) {
    std::string::size_type pos_slash = url.find_last_of('/');
    std::string::size_type pos_query = url.find_first_of('?');

    if(pos_slash != std::string::npos && pos_slash < url.length() -1) {
        std::string bin_tmp = url.substr(pos_slash + 1, pos_query - pos_slash - 1);
        std::string::size_type pos_dot = bin_tmp.find_last_of('.');
        std::string bin = bin_tmp.substr(pos_dot + 1, bin_tmp.length() - 1);
        std::cout << "bin: " << bin << std::endl;
        return bin;
    } else {
        std::cout << "Error getting binary name" << std::endl;
        return "Error getting binary name";
    }
}


// std::string getPath(int serverIndex, int locIndex, std::string uri) {
    
//     std::vector<std::string> rootDir = _parser->getLocationParam(serverIndex, locIndex, "location");
//     std::string path = rootDir[0] + uri;

//     if (path[path.length() - 1] != '/')
//         path += '/';

//     return path;
// }