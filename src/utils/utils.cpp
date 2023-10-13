/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:08:10 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 11:41:32 by pmitsuko         ###   ########.fr       */
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
