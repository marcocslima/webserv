/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:08:10 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 20:00:58 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Logger.hpp"

responseData getJson(std::string body, int status)
{
    responseData data;

    data = setResponseData(status, "application/json", body, (int)body.length());
    return (data);
}

responseData getContent(std::string root, std::string file, int status)
{
    std::stringstream fullPathStream;
    std::string       fullPath;
    const char       *fullPathCStr;
    responseData      data;
    std::string       extension;

    data      = setResponseData(0, "", "", 0);
    extension = extractFileExtension(file);
    fullPathStream << root << file;
    fullPath     = fullPathStream.str();
    fullPathCStr = fullPath.c_str();
    std::ifstream ifs(fullPathCStr);
    if (ifs.is_open()) {
        std::string content((std::istreambuf_iterator<char>(ifs)),
                            std::istreambuf_iterator<char>());
        data = setResponseData(
            status, Constants::getMimeTypes(extension), content, (int)content.length());
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

responseData
setResponseData(int status, std::string contentType, std::string content, int contentLength)
{
    responseData res;

    res.status        = status;
    res.statusCode    = Constants::getStatusCodes(to_string(status));
    res.contentType   = contentType;
    res.content       = content;
    res.contentLength = contentLength;
    return (res);
}

std::string vector_join(std::vector<std::string> vec, std::string delimiter)
{
    std::string                        result = "";
    std::vector<std::string>::iterator it;

    for (it = vec.begin(); it != vec.end(); ++it) {
        result += *it;
        result += delimiter;
    }
    return (result);
}

template <typename ValueType>
std::string to_string(ValueType v)
{
    std::ostringstream oss;
    oss << v;
    return oss.str();
}
