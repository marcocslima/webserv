/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:38:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/05 05:24:47 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Cgi.hpp"

CGI::CGI() {}

CGI::~CGI() {}

std::map<std::string, std::string> CGI::assembleCGIHeaders(const HttpRequest& request) {
    
    std::map<std::string, std::string> map_envs;
    
    map_envs["AUTH_TYPE"] = "";
    map_envs["REDIRECT_STATUS"] = "200";
    map_envs["GATEWAY_INTERFACE"] = "CGI/1.1";
    map_envs["SCRIPT_NAME"] = _cgi_path; // cgi path
    map_envs["SCRIPT_FILENAME"] = _cgi_path; // cgi path
    map_envs["REQUEST_METHOD"] = request.getMethod(); // request method
    map_envs["CONTENT_LENGTH"] = "";//strlen(len); // body converted to char*
    map_envs["CONTENT_TYPE"] = "text/html"; // content type
    map_envs["PATH_INFO"] = _cgi_path; // cgi path
    map_envs["PATH_TRANSLATED"] = _cgi_path; // cgi path
    map_envs["QUERY_STRING"] = "name=value"; // query
    map_envs["REMOTE_ADDR"] = "80"; // request port
    map_envs["REMOTE_IDENT"] = ""; // request authorization
    map_envs["REMOTE_USER"] = ""; // request authorization
    map_envs["REQUEST_URI"] = _cgi_path + "/name=value"; // request cgi path + resquest query
    map_envs["SERVER_NAME"] = "localhost"; // request host
    map_envs["SERVER_PROTOCOL"] = "HTTP/1.1";
    map_envs["SERVER_SOFTWARE"] = "Webserv/1.0";
    
    return (map_envs);
}

char** CGI::assembleCGIEnv(std::map<std::string, std::string> map_envs) {
    
    int size = map_envs.size();
    char** envs = new char*[size + 1];
    int i = 0;
    
    for (std::map<std::string, std::string>::iterator it = map_envs.begin(); it != map_envs.end(); it++, i++) {
        std::string env = it->first + "=" + it->second;
        envs[i] = new char[env.length() + 1];
        strcpy(envs[i], env.c_str());
    }
    
    envs[size] = 0;
    return (envs);
}

std::string CGI::executeCGI(const HttpRequest& request) {

    pid_t       pid;
    int         pipe_fd[2];
    std::string body;

    std::string rootTmp = getDir();
    _cgi_path = rootTmp + "/www" + request.getUri();

    std::string bin = getBin(request.getUri());

    if (bin.empty()) {
        throw std::runtime_error("bin is empty");
    } else if (bin == "php") {
        bin = "/usr/bin/" + bin;
    } else if (bin == "py") {
        bin = "/usr/bin/python3";
    }

    _envs = assembleCGIEnv(assembleCGIHeaders(request));
    
    char* cbin = const_cast<char*>(bin.c_str());
    char* pathBin = const_cast<char*>(_cgi_path.c_str());

    if (pipe(pipe_fd) == -1) {
        throw std::runtime_error("pipe() failed");
    }

    pid = fork();

    if (pid == -1) {
        throw std::runtime_error("fork() failed");
    } else if ( !pid ) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);

        char* argv[] = {cbin, pathBin, NULL};
        execve(cbin, argv, _envs);
        std::cerr << " execve() failed " << std::endl;
        write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n\r\n", 38);
    } else {
        close(pipe_fd[1]);
        char buffer[8192] = {0};
        int n = 0;
        while ((n = read(pipe_fd[0], buffer, 8191)) > 0) {
            body.append(buffer, n);
        }
        close(pipe_fd[0]);
        waitpid(pid, NULL, 0);
    }

    if ( !pid ) {
        int i = 0;
        while (_envs[i]) {
            delete [] _envs[i];
            i++;
        }
        delete [] _envs;
        exit (0);
    }

    return (body);
}