/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:49:52 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/08 00:14:01 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef LOGGER_H
# define LOGGER_H

# include <iostream>
# include <string>
# include <ctime>

class Logger
{
	public:
		enum LogLevel
		{
			INFO,
			WARNING,
			ERROR
		};

		Logger(void);
		~Logger(void);

		void	log(LogLevel level, const std::string& message);
};

#endif
