/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 08:46:45 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/16 12:02:08 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

AutoIndex::AutoIndex() {}

AutoIndex::~AutoIndex() {}

responseData AutoIndex::autoIndex(std::string root, std::string path, std::string port)
{
	static_cast<void>(port);
	std::string dirPath = root + path;

	DIR* dir = opendir(dirPath.c_str());
	if (dir == NULL)
	{
		setResponse("404 Not Found", "<html><body><h1>404 Not Found</h1></body></html>");
		return (_responseData);
	}

	std::string content = "<html><body><h2>Index of: " + dirPath + "</h2><ul>";
	struct dirent* entry;
	for (entry = readdir(dir); entry; entry = readdir(dir))
	{
        std::string entryPath = port + path + (path[path.size() - 1] != '/' ? "/" : "") + std::string(entry->d_name);
        content += "<li><a href=\"http://localhost:" + entryPath + "\">" + std::string(entry->d_name) + "</a></li>\n";
	}

	content += "</ul></body></html>";
	setResponse("200 OK", content);

	closedir(dir);
    return (_responseData);
}

void AutoIndex::setResponse(std::string statusCode, std::string content)
{
    _responseData.content = content;
    _responseData.statusCode = statusCode;
    _responseData.contentType = "text/html";
    _responseData.contentLength = content.size();
}