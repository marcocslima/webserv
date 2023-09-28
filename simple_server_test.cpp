/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_server_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 03:21:54 by mcl               #+#    #+#             */
/*   Updated: 2023/09/24 23:29:22 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./sources/includes/Request.hpp"
#include <arpa/inet.h>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    HttpRequest        request;
    int                server_fd, new_socket;
    struct sockaddr_in address;
    int                addrlen      = sizeof(address);
    char               buffer[1024] = {0};

    // Lê o conteúdo do arquivo HTML para a página padrão
    ifstream defaultHtmlFile("www/index.html");
    string   defaultHtmlContent((istreambuf_iterator<char>(defaultHtmlFile)),
                              istreambuf_iterator<char>());

    // Criar o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configurar o endereço e a porta para o socket
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço do localhost
    address.sin_port        = htons(3000);            // Porta 3000

    // Associar o socket ao endereço e à porta especificados
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha ao associar o socket ao endereço e à porta especificados");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escutar conexões no socket
    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar conexões no socket");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, ::exit); // Lidar com o sinal SIGINT (Ctrl+C)

    while (true) {
        // Aguardar uma conexão e aceitá-la quando ela chegar
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen))
            < 0) {
            perror("Erro ao aceitar a conexão");
            exit(EXIT_FAILURE);
        }

        // Receber a solicitação HTTP do cliente
        int bytesRead = recv(new_socket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            // Analisar a solicitação HTTP para determinar a rota
            string req(buffer, bytesRead);

            request.requestHttp(req);

            size_t start = req.find(request.getMethod());
            size_t end   = req.find(request.getHttp());

            if (start != string::npos && end != string::npos) {

                if (request.getUri() == "/") {
                    // Servir a página padrão (www/index.html)

                    // Construir o cabeçalho HTTP
                    char responseHeader[1024];
                    sprintf(
                        responseHeader,
                        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n",
                        (int)defaultHtmlContent.length());

                    // Enviar o cabeçalho HTTP
                    send(new_socket, responseHeader, strlen(responseHeader), 0);

                    // Enviar o conteúdo HTML da página padrão como resposta
                    send(new_socket, defaultHtmlContent.c_str(), defaultHtmlContent.length(), 0);
                }
                else if (request.getUri() == "/upload") {
                    // Rota "/upload" para exibir a página de upload

                    ifstream uploadHtmlFile("www/upload.html");
                    string   uploadHtmlContent((istreambuf_iterator<char>(uploadHtmlFile)),
                                             istreambuf_iterator<char>());

                    char responseHeader[1024];
                    sprintf(
                        responseHeader,
                        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n",
                        (int)uploadHtmlContent.length());

                    // Enviar o cabeçalho HTTP
                    send(new_socket, responseHeader, strlen(responseHeader), 0);

                    // Enviar o conteúdo HTML da página de upload como resposta
                    send(new_socket, uploadHtmlContent.c_str(), uploadHtmlContent.length(), 0);
                    if (request.getMethod() == "POST") {

                        std::map<std::string, std::string> headers = request.getHeaders();
                        if (headers.find("Content-Type") != headers.end()
                            && headers["Content-Type"].find("multipart/form-data")
                                != std::string::npos) {

                            std::string fileContentStart = "\r\n\r\n";
                            size_t fileContentStartPos   = request.getBody().find(fileContentStart);

                            if (fileContentStartPos != std::string::npos) {
                                fileContentStartPos += fileContentStart.length();
                                // O restante do corpo da solicitação contém o conteúdo do arquivo
                                std::string fileData
                                    = request.getBody().substr(fileContentStartPos);

                                // Encontre o final do conteúdo do arquivo com base no limite
                                // (boundary)
                                std::string boundary = headers["Content-Type"];
                                boundary = boundary.substr(boundary.find("boundary=") + 9);
                                std::string boundaryEnd = "--" + boundary + "--";
                                size_t      fileEndPos  = fileData.find(boundaryEnd);

                                if (fileEndPos != std::string::npos) {

                                    std::string fileName = "uploaded_file.txt";

                                    // Caminho onde o arquivo será salvo no servidor
                                    std::string filePath = "sources/" + fileName;

                                    // Crie um fluxo de saída para gravar o conteúdo no arquivo
                                    std::ofstream outputFile(filePath.c_str(),
                                                             std::ios::binary | std::ios::app);
                                    if (outputFile) {
                                        outputFile.write(fileData.c_str(), fileEndPos);
                                        outputFile.close();

                                        std::string responseMessage
                                            = "Arquivo salvo com sucesso em: " + filePath;

                                        // Construir o cabeçalho HTTP de resposta
                                        char responseHeader[1024];
                                        sprintf(responseHeader,
                                                "HTTP/1.1 200 OK\r\nContent-Type: "
                                                "text/plain\r\nContent-Length: %d\r\n\r\n",
                                                (int)responseMessage.length());

                                        // Enviar o cabeçalho HTTP
                                        send(new_socket, responseHeader, strlen(responseHeader), 0);

                                        // Enviar a mensagem de resposta
                                        send(new_socket,
                                             responseMessage.c_str(),
                                             responseMessage.length(),
                                             0);
                                    }
                                    else {
                                        std::string responseMessage = "Erro ao salvar o arquivo.";

                                        // Construir o cabeçalho HTTP de resposta
                                        char responseHeader[1024];
                                        sprintf(
                                            responseHeader,
                                            "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
                                            "text/plain\r\nContent-Length: %d\r\n\r\n",
                                            (int)responseMessage.length());

                                        // Enviar o cabeçalho HTTP
                                        send(new_socket, responseHeader, strlen(responseHeader), 0);

                                        // Enviar a mensagem de resposta de erro
                                        send(new_socket,
                                             responseMessage.c_str(),
                                             responseMessage.length(),
                                             0);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Fechar o socket da nova conexão
            close(new_socket);
        }
    }
    return 0;
}
