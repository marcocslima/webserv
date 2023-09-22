/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/22 14:27:06 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include <iostream>
# include <sstream>
# include <algorithm>
# include <exception>
# include <string>
# include <vector>
# include <map>
# include "Logger.hpp"
# include "Parser.hpp"

#define BAD_REQUEST                "400"
#define HTTP_VERSION_NOT_SUPPORTED "505"
class HttpRequest {
	public:
		HttpRequest();
		~HttpRequest();

		void	requestHttp(std::string request, Parser& parser);
		void    initMethods();

		std::string getMethod( void ) const;
		std::string getUri( void ) const;
		std::string getHttp( void ) const;
		std::string getStatusError( void ) const;
		std::map<std::string,std::string> getHeaders(void);
		class RequestException : public std::exception {
			public:
				virtual const char* what() const throw();
		};

	private:
		void	_parseHeaders(const std::string& request);
		void	_parseFirstLine(std::string& requestLine);
		void	_checkLocations(Parser& parser);
		void	_checkPorts(Parser& parser);

		std::string							_requestLine;
		std::string							_method;
		std::string							_uri;
		std::string							_httpVersion;
		std::string							_port;
		std::string							_statusError;
		std::vector<std::string>			_allowMethods;
        std::map<std::string, std::string>	_headers;
};
