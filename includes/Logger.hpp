/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:49:52 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/13 21:47:23 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef LOGGER_H
# define LOGGER_H

# include <iostream>
# include <string>
# include <ctime>
# include <sstream>
# include <iomanip>

# define RESET_COLOR "\033[0m"

class Logger {
	public:
		enum LogLevel
		{
			INFO,
			WARNING,
			ERROR
		};

		Logger(void);
		~Logger(void);

		class LogEntry {
			public:
				LogEntry(LogLevel level);
				~LogEntry(void);

				template <typename T>
				LogEntry &operator<<(T const &value) {
					_stream << value;
					return (*this);
				};

				LogEntry &operator<<(std::ostream &(*manipulator)(std::ostream &));

			private:
				LogLevel			_level;
				std::stringstream	_stream;

				std::string	_timestamp(void);
				std::string	_addHeader(void);
		};

		static LogEntry	info;
		static LogEntry	warning;
		static LogEntry	error;

	private:
		static const std::string	_colors[3];
		static const std::string	_levelStrings[3];
};

#endif
