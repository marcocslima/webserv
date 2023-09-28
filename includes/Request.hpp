/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/28 13:36:58 by jefernan         ###   ########.fr       */
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
		void	init();

		std::string							getMethod( void ) const;
		std::string							getUri( void ) const;
		std::string							getHttp( void ) const;
		// std::string							getReq( void ) const;
		std::vector<std::string>			getQuery( void ) const;
		std::string 						getBody( void ) const;
		std::string							getBoundary( void ) const;
		std::map<std::string,std::string>	getHeaders(void) const;
		class RequestException : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		bool								_has_body;
		bool								_has_chunked;
		bool								_has_multipart;
		bool								_has_form;
	private:
		void	_parseHeaders(const std::string& request);
		void	_parseFirstLine(std::string& requestLine);
		void	_findHeaders(std::string key,std::string value );
		void	_parseQuery(void);
		void	_getMultipartData(std::string request);
		void	_getBody(std::string request);
		void	_checkLocations(Parser& parser);
		void	_checkPorts(Parser& parser);

		std::string							_body;
		std::string 						_boundary;
		std::string							_method;
		std::string							_uri;
		std::string							_httpVersion;
		std::string							_port;
		std::string							_statusError;
		std::vector<std::string> 			_paramQuery;
		std::vector<std::string>			_allowMethods;
        std::map<std::string, std::string>	_header;
};
