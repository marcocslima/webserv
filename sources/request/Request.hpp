/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:43:15 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/10 19:30:31 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		void    initMethods();
		void	requestHttp(std::string request);
		bool	parseHttpRequest(const std::string& request, std::map<std::string, std::string>& headers);
		bool	checkFirstLine(std::string requestLine);
		void	findContentLength();

		std::string	requestLine, method, uri, httpVersion;
        std::map<std::string, std::string>	headers;
		std::vector<std::string>			methods;
		size_t								contentLength;
};

#endif