/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:08:10 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 00:17:14 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Logger.hpp"

responseData getJson(std::string body)
{
    responseData data;

    data.content       = body;
    data.contentLength = (int)body.length();
    data.contentType   = "application/json";
    return (data);
}

responseData getContent(std::string root, std::string file)
{
    std::stringstream fullPathStream;
    std::string       fullPath;
    const char       *fullPathCStr;
    responseData      data;

    data.content       = "";
    data.statusCode    = "";
    data.contentType   = "";
    data.contentLength = 0;
    fullPathStream << root << file;
    fullPath     = fullPathStream.str();
    fullPathCStr = fullPath.c_str();
    std::ifstream ifs(fullPathCStr);
    if (ifs.is_open()) {
        std::string content((std::istreambuf_iterator<char>(ifs)),
                            std::istreambuf_iterator<char>());
        data.content       = content;
        data.contentLength = (int)content.length();
        ifs.close();
    }
    return (data);
}

std::string extractFileExtension(std::string file)
{
    size_t dotPos = file.rfind('.');

    if (dotPos != std::string::npos) {
        std::string extension = file.substr(dotPos);
        return extension;
    }
    return "";
}

std::string getDir()
{
    char        cwd[1024];
    std::string rPath = "webserv";

    if (getcwd(cwd, 1024) != NULL) {
        std::string dir(cwd);
        size_t      pos = dir.find(rPath);
        if (pos != std::string::npos)
            dir = dir.substr(0, pos + 7);
        return dir;
    } else {
        Logger::error << "Error getting current working directory" << std::endl;
        return "";
    }
}

std::string extentionToBin(std::string extention)
{
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

std::string getBin(const std::string &url)
{

    const char *binName;
    std::string command = "which ";

    std::string::size_type pos_slash = url.find_last_of('/');
    std::string::size_type pos_query = url.find_first_of('?');

    if (pos_slash != std::string::npos && pos_slash < url.length() - 1) {
        std::string            bin_tmp = url.substr(pos_slash + 1, pos_query - pos_slash - 1);
        std::string::size_type pos_dot = bin_tmp.find_last_of('.');
        std::string            bin     = bin_tmp.substr(pos_dot + 1, bin_tmp.length() - 1);
        bin                            = extentionToBin(bin);
        command += bin;
        binName      = command.c_str();
        FILE *stream = popen(binName, "r");
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
