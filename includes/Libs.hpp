/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:12:50 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/14 09:11:12 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define IP "127.0.0.1"

#define DEFAULT_ROOT "www/html"
#define DEFAULT_ROOT_CGI "www/cgi"
#define DEFAULT_ERROR_ROOT "www/error"

#define P_BLUE "\e[0;38;5;24m"
#define RESET_COLOR "\e[0m"

#define ENTITY_TOO_LARGE "413"

#define OK 200
#define NO_CONTENT 204
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define INTERNAL_SERVER_ERROR 500
#define HTTP_VERSION_NOT_SUPPORTED 505

struct responseData {
    int         status;
    std::string content;
    std::string statusCode;
    std::string contentType;
    int         contentLength;
};
