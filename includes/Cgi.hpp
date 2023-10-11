/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:36:29 by mcl               #+#    #+#             */
/*   Updated: 2023/10/06 03:20:34 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "Parser.hpp"
# include "Request.hpp"
# include "AResponse.hpp"
# include "Utils.hpp"
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>

class CGI : public HttpRequest {
    private:
        char**      _envs;
        std::string _cgi_path;
    public:
        CGI();
        ~CGI();

        std::map<std::string, std::string> assembleCGIHeaders(const HttpRequest& request);
        char** assembleCGIEnv(std::map<std::string, std::string> map_envs);
        std::string executeCGI(const HttpRequest& request);
};
