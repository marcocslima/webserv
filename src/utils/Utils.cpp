/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 01:59:27 by mcl               #+#    #+#             */
/*   Updated: 2023/10/06 05:20:25 by mcl              ###   ########.fr       */
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

std::string extentionToBin (std::string extention) {
    if (extention == "php")
        return "php";
    else if (extention == "py")
        return "python3";
    else if (extention == "rb")
        return "ruby";
    else if (extention == "pl")
        return "perl";
    else
        return "";
}

std::string getBin(const std::string& url) {

    const char* binName;
    std::string command = "which ";

    std::string::size_type pos_slash = url.find_last_of('/');
    std::string::size_type pos_query = url.find_first_of('?');

    if (pos_slash != std::string::npos && pos_slash < url.length() -1) {
        std::string bin_tmp = url.substr(pos_slash + 1, pos_query - pos_slash - 1);
        std::string::size_type pos_dot = bin_tmp.find_last_of('.');
        std::string bin = bin_tmp.substr(pos_dot + 1, bin_tmp.length() - 1);
        bin = extentionToBin(bin);
        command += bin;
        binName = command.c_str();
        FILE* stream = popen(binName, "r");
        if (!stream) {
            Logger::info << "Error getting binary name" << std::endl;
            return "";
        }
        char buffer[1024];
        while (fgets(buffer, 1024, stream) != NULL) {
            pclose(stream);
            std::string bin(buffer);
            bin.erase(std::remove(bin.begin(), bin.end(), '\n'), bin.end());
            return bin;
        }
        Logger::info << "Error getting binary name" << std::endl;
        return "";
    } else {
        Logger::info << "Error getting binary name" << std::endl;
        return "";
    }
}
