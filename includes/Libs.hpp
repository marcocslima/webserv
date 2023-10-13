/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:12:50 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 15:37:21 by pmitsuko         ###   ########.fr       */
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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP "127.0.0.1"

#define DEFAULT_ROOT "www/html"
#define DEFAULT_ERROR_ROOT "www/error"

#define RESET_COLOR "\033[0m"

#define BAD_REQUEST "400"
#define HTTP_VERSION_NOT_SUPPORTED "505"
#define ENTITY_TOO_LARGE "413"

// #define MAX_BODY_SIZE 100

struct responseData {
    int         status;
    std::string content;
    std::string statusCode;
    std::string contentType;
    int         contentLength;
};
