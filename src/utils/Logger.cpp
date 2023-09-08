/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:49:57 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/08 00:27:54 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::Logger(void)
{}

Logger::~Logger(void)
{}

void	Logger::log(LogLevel level, const std::string& message)
{
	std::string	levelStr;
	std::string	colorCode;
	std::time_t	currentTime;
	std::tm*	localTime;
	char		timeStr[24];
	std::string	resetCode;

	resetCode = "\033[0m";
	switch (level) {
		case INFO:
			levelStr = "INFO";
			colorCode = "\033[92m";
			break;
		case WARNING:
			levelStr = "WARNING";
			colorCode = "\033[93m";
			break;
		case ERROR:
			levelStr = "ERROR";
			colorCode = "\033[95m";
			break;
	}
	currentTime = std::time(NULL);
	localTime = std::localtime(&currentTime);
	std::strftime(timeStr, sizeof(timeStr), "%d/%m/%Y - %H:%M:%S", localTime);
	if (level == ERROR)
	{
		std::cerr << colorCode << "[" << timeStr << "]  "
			<< levelStr << " - " << message << resetCode << std::endl;
	}
	else
	{
		std::cout << colorCode << "[" << timeStr << "]  "
			<< levelStr << " - " << message << resetCode << std::endl;
	}
}
