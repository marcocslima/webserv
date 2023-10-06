/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:27:05 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/05 16:26:58 by jefernan         ###   ########.fr       */
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
    static_cast<void>(uri);
    created = false;

    if (_httpRequest._has_body && _httpRequest._tooLarge == false)
    {
        if (_httpRequest._has_multipart)
        {
            handleMultipart();
            if (created && _file == true)
            {
                setResponse("201", "Created", "<html><body><h1>201 Created</h1></body></html>");
                Logger::info << "File created." << std::endl;
                return (_responseHeader);
            }
            else if (!created && _file == true)
            {
                setResponse("500", "Internal Server Error", "<html><body><h1>500  Internal Server Error</h1></body></html>");
                Logger::error << "Unable to create file." << std::endl;
                return (_responseHeader);
            }
        }
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
        Logger::error << "Internal Server Error." << std::endl;
    }
    return (_responseHeader);
}

void    PostMethod::handleMultipart()
{
    std::string boundary = _httpRequest.getBoundary();
    std::string body = _httpRequest.getBody();

    _formData.clear();
    _file = false;
    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos)
    {
        pos += boundary.length();
        size_t endPos = body.find(boundary, pos);
        if (endPos != std::string::npos)
            parseMultipartFormData(pos, endPos);
    }
    if (_file == false)
        print();
}

std::string setFileName(size_t pos, std::string data)
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    int randomNumber = std::rand() % 500;

    std::ostringstream oss;
    oss << "file" << randomNumber;

    std::size_t filenameEnd = data.find("\"", pos);
    std::string aux = data.substr(pos, filenameEnd - pos);
    size_t dot = aux.find(".");
    std::string extension = aux.substr(dot);

    oss << extension;

    std::string fileName = oss.str();
    return (fileName);
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
                std::size_t filenamePos = data.find("filename=\"");

                if (filenamePos != std::string::npos )
                {
                    filenamePos += 10;
                    std::size_t filenameEnd = data.find("\"", filenamePos);
                    if (filenameEnd != std::string::npos)
                    {
                        std::string fileName = setFileName(filenamePos, data);
                        saveFile(fileName, value);
                        _file = true;
                    }
                } else
                    _formData[name] = value;
            }
        }
    }
}

void PostMethod::saveFile(std::string& fileName, const std::string& value)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        std::string filePath = cwd;
        filePath += "/uploads/" + fileName;

        std::ifstream checkFile(filePath.c_str());
        if (checkFile.good())
        {
           created = false;
           Logger::error << "The file already exists." << std::endl;
           return;
        }
        std::ofstream file(filePath.c_str(), std::ios::binary);
        if (file.is_open())
        {
            file.write(value.c_str(), value.length());
            file.close();
            created = true;
        }
        else
            created = false;
    } else
    {
        Logger::error << "Error getting current working directory." << std::endl;
        created = false;
    }
}

std::string replaceChar(const std::string& input)
{
    std::string output;
    size_t pos = 0;

    while (pos < input.length())
    {
        if (input[pos] == '%')
        {
            if (pos + 2 < input.length())
            {
                char hex[3] = { input[pos + 1], input[pos + 2], '\0' };
                int decodedChar = strtol(hex, NULL, 16);
                output.push_back(static_cast<char>(decodedChar));
                pos += 2;
            }
        } else if (input[pos] == '+')
            output.push_back(' ');
        else
            output.push_back(input[pos]);
        pos++;
    }
    return output;
}

void PostMethod::handleForm()
{
    std::string body = _httpRequest.getBody();
    std::istringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&'))
    {
        size_t equal = pair.find('=');
        if (equal != std::string::npos)
        {
            std::string fieldName = replaceChar(pair.substr(0, equal));
            std::string fielddata = replaceChar(pair.substr(equal + 1));
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