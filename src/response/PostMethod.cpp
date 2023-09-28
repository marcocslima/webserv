/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:27:05 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/28 08:42:02 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"

PostMethod::PostMethod() {}

PostMethod::~PostMethod() {}

PostMethod::PostMethod(HttpRequest request): _httpRequest(request) {}

void    PostMethod::getBody(std::string request){
    std::size_t bodyStart = request.find("\r\n\r\n") + 4;

    if (bodyStart != std::string::npos){
        body = request.substr(bodyStart);
    }
}

void    PostMethod::handleForm(){

    std::map<std::string, std::string> formData;
    std::istringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        size_t equal = pair.find('=');
        if (equal != std::string::npos) {
            std::string fieldName = pair.substr(0, equal);
            std::string fielddata = pair.substr(equal + 1);
            for (size_t i = 0; i < fielddata.length(); ++i) {
                if (fielddata[i] == '+') {
                    fielddata[i] = ' ';
                }
            }
            formData[fieldName] = fielddata;
        }
    }
    std::map<std::string, std::string>::iterator it;
    for (it = formData.begin(); it != formData.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void saveFile(size_t pos, const std::string& value, const std::string& data) {
    std::size_t filenameEnd = data.find("\"", pos);

    if (filenameEnd != std::string::npos) {
        std::string fileName = data.substr(pos, filenameEnd - pos);

        std::ofstream file(fileName.c_str(), std::ios::binary);
        file.write(value.c_str(), value.length());
        file.close();
    }
}

std::map<std::string, std::string> PostMethod::parseMultipartFormData(std::map<std::string, std::string> formData, size_t pos, size_t endPos) {
    std::string partData = body.substr(pos, endPos - pos);
    std::size_t bodyEnd = partData.find("\r\n\r\n") + 4;

    if (bodyEnd != std::string::npos) {
        std::string data = partData.substr(0, bodyEnd);
        std::string value = partData.substr(bodyEnd);

        std::size_t namePos = data.find("name=\"");
        if (namePos != std::string::npos) {
            namePos += 6;
            std::size_t nameEnd = data.find("\"", namePos);
            if (nameEnd != std::string::npos) {
                std::string name = data.substr(namePos, nameEnd - namePos);

                formData[name] = value;
                std::size_t filenamePos = data.find("filename=\"");
                if (filenamePos != std::string::npos) {
                    filenamePos += 10;
                    saveFile(filenamePos, value, data);
                }
            }
        }
    }
    return(formData);
}

void    PostMethod::handleMultipart(){
    std::map<std::string, std::string> formData;

    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        pos += boundary.length();
        size_t endPos = body.find(boundary, pos);
        if (endPos != std::string::npos) {
            formData = parseMultipartFormData(formData, pos, endPos);
        }
    }
    std::map<std::string, std::string>::iterator it;
    for (it = formData.begin(); it != formData.end(); ++it)
    {
        std::cout << "Field: " << it->first << std::endl;
        std::cout << "data: " << it->second << std::endl;
    }
}

bool    PostMethod::getContentType(){
    std::map<std::string, std::string> headers = _httpRequest.getHeaders();
    std::map<std::string, std::string>::const_iterator it = headers.find("Content-Type");

    if (it != headers.end()){
        std::string tmp = it->second;
        contentType = it->second;
        size_t pos = tmp.find(";");
        if (pos != std::string::npos){
            contentType = tmp.substr(1, pos);
            size_t boundaryPos = tmp.find("boundary=", pos);
            if (boundaryPos != std::string::npos) {
                boundary = tmp.substr(boundaryPos + 9);
                boundary = "--" + boundary;
            }
        }
        return (true);
    } else {
        return (false);
    }
}

std::string PostMethod::handleMethod(std::string uri)
{
    uri = " ";
    std::string responseHeader;

    if (getContentType()) {
        getBody(_httpRequest.getReq());

        if (contentType == "multipart/form-data;")
            handleMultipart();
        if (contentType ==  " application/x-www-form-urlencoded")
            handleForm();
        _response.version = "HTTP/1.1";
        _response.status_code = "200";
        _response.status_message = "OK";
        _response.content_type = "text/html";
        _response.content_length = "0";
        _response.body = "<html><body><h1>200 OK</h1></body></html>";
        responseHeader = assembleResponse();
        Logger::info << "Post Successfully." << std::endl;
    } else {
        _response.version = "HTTP/1.1";
        _response.status_code = "204";
        _response.status_message = "No content";
        _response.content_type = "text/html";
        _response.content_length = "0";
        _response.body = "<html><body><h1>204 No Content</h1></body></html>";
        responseHeader = assembleResponse();
        Logger::info << "No content." << std::endl;
    }

    return (responseHeader);
}
