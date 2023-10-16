/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 08:47:30 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/16 09:59:52 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Libs.hpp"
#include "Logger.hpp"
#include "ResponseHandlers.hpp"

class AutoIndex {
	public:
		AutoIndex(void);
		~AutoIndex(void);

		responseData autoIndex(std::string root, std::string path, std::string port);
	private:
		void			setResponse(std::string statusCode, std::string content);
		std::string		_link(const std::string &pathLink, std::string dirEntry, std::string port);
		responseData	_responseData;

};