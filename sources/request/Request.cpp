/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/06 15:59:59 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

HttpRequest::HttpRequest() {};

HttpRequest::~HttpRequest() {};

bool HttpRequest::parseHttpRequest(const std::string& request, std::string& method, std::string& url, std::vector<std::string>& headers) {
    std::string	header[6] = {"Host", "User-Agent", "Accept", "Accept-Language, Referer, Connection"};
    std::istringstream iss(request);
    std::string line;

    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        lineStream >> method ;
    } else {
        return false;
    }

while (std::getline(iss, line) && !line.empty()) {
    for (int i = 0; i < 6; i++) {
        if (line.compare(0, header[i].length(), header[i]) == 0) {
            headers.push_back(line);
        }
    }
}

    return true;
}

void	HttpRequest::requestHttp(std::string request){

    if (parseHttpRequest(request, method, url, headers)) {
        std::cout << "Method: " << method << std::endl;
        std::cout << "URL: " << url << std::endl;
        std::cout << "Headers:" << std::endl;
        for (std::vector<std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
            const std::string& header = *it;
            std::cout << header << std::endl;
        }
    } else {
        std::cerr << "Error parsing HTTP request." << std::endl;
    }
}
