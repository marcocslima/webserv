/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 03:21:54 by mcl               #+#    #+#             */
/*   Updated: 2023/09/01 04:02:45 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    //char buffer[1024] = {0};

    // Lê o conteúdo do arquivo HTML
    ifstream htmlFile("www/index.html");
    string htmlContent((istreambuf_iterator<char>(htmlFile)), istreambuf_iterator<char>());

    // Criar o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configurar o endereço e a porta para o socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço do localhost
    address.sin_port = htons(3000); // Porta 3000

    // Associar o socket ao endereço e à porta especificados
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha ao associar o socket ao endereço e à porta especificados");
        exit(EXIT_FAILURE);
    }

    // Escutar conexões no socket
    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar conexões no socket");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, ::exit);  // Lidar com o sinal SIGINT (Ctrl+C)

    while (true) {
        // Aguardar uma conexão e aceitá-la quando ela chegar
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Erro ao aceitar a conexão");
            exit(EXIT_FAILURE);
        }

        // Construir o cabeçalho HTTP
        char responseHeader[1024];
        sprintf(responseHeader, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n", (int)htmlContent.length());

        // Enviar o cabeçalho HTTP
        send(new_socket, responseHeader, strlen(responseHeader), 0);

        // Enviar o conteúdo HTML como resposta para o cliente
        send(new_socket, htmlContent.c_str(), htmlContent.length(), 0);
        printf("Página HTML enviada para o cliente\n");

        // Fechar o socket da nova conexão
        close(new_socket);
    }

    return 0;
}
