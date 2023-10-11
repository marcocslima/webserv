/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:38:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/07 05:23:18 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Cgi.hpp"

CGI::CGI() {}

CGI::~CGI() {}

std::string queryToString(std::vector<std::string> query) {
    
    std::string resp;

    if (query.empty()) {
        return (resp);
    }
    
    for (std::vector<std::string>::iterator it = query.begin(); it != query.end(); it++) {
        resp += *it;
        if (it != query.end() - 1) {
            resp += "&";
        }
    }
    return (resp);
}

std::map<std::string, std::string> CGI::assembleCGIHeaders(const HttpRequest& request) {
    
    std::map<std::string, std::string> map_envs;

    map_envs["AUTH_TYPE"] = "";
    map_envs["REDIRECT_STATUS"] = "200";
    map_envs["GATEWAY_INTERFACE"] = "CGI/1.1";
    map_envs["SCRIPT_NAME"] = _cgi_path; // cgi path
    map_envs["SCRIPT_FILENAME"] = _cgi_path; // cgi path
    map_envs["REQUEST_METHOD"] = request.getMethod(); // request method
    map_envs["CONTENT_LENGTH"] = request.getBody().length();//strlen(len); // body converted to char*
    map_envs["CONTENT_TYPE"] = "text/html"; // content type
    map_envs["PATH_INFO"] = _cgi_path; // cgi path
    map_envs["PATH_TRANSLATED"] = _cgi_path; // cgi path
    map_envs["QUERY_STRING"] = queryToString(request.getQuery()); // query
    map_envs["REQUEST_BODY"] = request.getBody(); // request body
    map_envs["REMOTE_ADDR"] = ""; // request port
    map_envs["REMOTE_IDENT"] = ""; // request authorization
    map_envs["REMOTE_USER"] = ""; // request authorization
    map_envs["REQUEST_URI"] = _cgi_path; // request cgi path + resquest query
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
    unsigned int timeout = 10000; // 10 seconds before timeout
    struct timespec startTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    std::string rootTmp = getDir();
    _cgi_path = rootTmp + "/www" + request.getUri();

    std::string bin = getBin(request.getUri());

    if (bin.empty())
        Logger::info << "There are not this program." << std::endl;

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
        write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n\r\n", 38);
    } else {
        close(pipe_fd[1]);
        struct timespec currentTime;
        
        while (true) {
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                break;
            }
            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            if (currentTime.tv_sec - startTime.tv_sec > timeout / 1000) {
                kill(pid, SIGTERM);
                break;
            }
            usleep(500);
        }
        
        char buffer[8192] = {0};
        int n = 0;
        while ((n = read(pipe_fd[0], buffer, 8191)) > 0) {
            body.append(buffer, n);
        }
        close(pipe_fd[0]);
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