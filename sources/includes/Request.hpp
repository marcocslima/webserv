/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:43:15 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/14 16:59:13 by jefernan         ###   ########.fr       */
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

		void	requestHttp(std::string request);
		void    initMethods();

		std::string getMethod( void ) const;
		std::string getUri( void ) const;
		std::string getBody( void ) const;
		std::string getHttp( void ) const;
		std::map<std::string,std::string> getHeaders(void);
	private:
		bool	_parseHttpRequest(const std::string& request, std::map<std::string, std::string>& headers);
		bool	_checkFirstLine(std::string& requestLine);
		void    _findBody(std::string& request);
		void    _findQuery();

		std::string							_requestLine;
		std::string							_method;
		std::string							_uri;
		std::string							_httpVersion;
		std::string							_query;
		std::string							_body;
		std::vector<std::string>			_methods;
        std::map<std::string, std::string>	_headers;
};

#endif