/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:27:05 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/30 17:21:58 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"

PostMethod::PostMethod()
{}

PostMethod::~PostMethod()
{}

PostMethod::PostMethod(HttpRequest request): _httpRequest(request)
{}

std::string PostMethod::handleMethod(std::string uri)
{
    uri = " ";

    if (_httpRequest._has_body){
        if (_httpRequest._has_multipart)
            handleMultipart();
        if (_httpRequest._has_form)
            handleForm();
        setResponse("200", "OK", "<html><body><h1>200 OK</h1></body></html>");
        Logger::info << "Post request completed successfully." << std::endl;
    }
    else if (!_httpRequest._has_body)
    {
        setResponse("204", "No content","<html><body><h1>204 No Content</h1></body></html>");
        Logger::info << "No content." << std::endl;
    }
    else
    {
        setResponse("500", "Internal Server Error", "<html><body><h1>500  Internal Server Error</h1></body></html>");
        Logger::info << "Error in post request." << std::endl;
    }

    return (_responseHeader);
}

void    PostMethod::handleMultipart()
{
    std::string boundary = _httpRequest.getBoundary();
    std::string body = _httpRequest.getBody();

    _formData.clear();
    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos)
    {
        pos += boundary.length();
        size_t endPos = body.find(boundary, pos);
        if (endPos != std::string::npos)
            parseMultipartFormData(pos, endPos);
    }
    print();
}

void    PostMethod::parseMultipartFormData(size_t pos, size_t endPos)
{
    std::string body = _httpRequest.getBody();
    std::string partData = body.substr(pos, endPos - pos);
    std::size_t bodyEnd = partData.find("\r\n\r\n");

    if (bodyEnd != std::string::npos)
    {
        std::string data = partData.substr(0, bodyEnd);
        std::string value = partData.substr(bodyEnd + 4);

        std::size_t namePos = data.find("name=\"");
        if (namePos != std::string::npos)
        {
            namePos += 6;
            std::size_t nameEnd = data.find("\"", namePos);
            if (nameEnd != std::string::npos)
            {
                std::string name = data.substr(namePos, nameEnd - namePos);
                _formData[name] = value;
            }

            size_t contentPos = data.find("Content-Type: ");
            size_t contendEnd = data.find("\r\n", contentPos + 14);
            if (contentPos != std::string::npos && contendEnd != std::string::npos){
                _contentType = data.substr(contentPos + 14, contendEnd - (contentPos - 14));
            }

            std::size_t filenamePos = data.find("filename=\"");
            if (filenamePos != std::string::npos && _contentType == "text/plain")
            {
                filenamePos += 10;
                saveFile(filenamePos, value, data);
            }
        }
    }
}

void PostMethod::saveFile(size_t pos, const std::string& value, const std::string& data)
{
    std::size_t filenameEnd = data.find("\"", pos);

    if (filenameEnd != std::string::npos)
    {
        std::string fileName = data.substr(pos, filenameEnd - pos);
        std::string filePath = "/uploads/" + fileName;
        for ( int i = 0; i < 10; i++ ) {
            filePath += static_cast<char>( rand() % 26 + 97 );
        }

        std::ofstream file(filePath.c_str(), std::ios::binary);
        if (file.is_open()){

            file.write(value.c_str(), value.length());
            file.close();
            created = true;
        } else {
            created = false;
        }
    }
}

void    PostMethod::handleForm(){
    std::string body = _httpRequest.getBody();
    std::istringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&'))
    {
        size_t equal = pair.find('=');
        if (equal != std::string::npos)
        {
            std::string fieldName = pair.substr(0, equal);
            std::string fielddata = pair.substr(equal + 1);
            for (size_t i = 0; i < fielddata.length(); ++i)
            {
                if (fielddata[i] == '+')
                    fielddata[i] = ' ';
            }
            _formData[fieldName] = fielddata + "\r\n";
        }
    }
    print();
}

void    PostMethod::print()
{
    std::cout << P_BLUE << std::endl;
    std::cout <<  "      ------------ Content Form ------------" << std::endl;
    std::cout << std::left << std::setw(20) << "Key" << std::setw(30) << "| Value" << std::endl;
    std::cout << std::setfill('-') << std::setw(50) << "-" << std::setfill(' ') << std::endl;

    std::map<std::string, std::string>::iterator it;
    for (it = _formData.begin(); it != _formData.end(); ++it)
        std::cout << std::left << std::setw(19) << it->first << " | " << it->second ;
    std::cout << std::setfill('-') << std::setw(50) << "-" << std::setfill(' ') << std::endl;
    std::cout << RESET << std::endl;
}

void    PostMethod::setResponse(std::string status, std::string message, std::string body){
    _response.version = "HTTP/1.1";
    _response.status_code = status;
    _response.status_message = message;
    _response.content_type = "text/html";
    _response.content_length = "0";
    _response.body = body;
    _responseHeader = assembleResponse();
}