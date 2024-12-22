#include <stdio.h>
#include <winsock2.h>
#include "server.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 57\r\n"
        "\r\n"
        "<html><body><h1>E Whotto Welcome to the WinSock Web Server!</h1></body></html>";

    // Initialize Winsock
    printf("Whotto Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

    // Prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code : %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Bind done.\n");

    // Start listening
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error Code : %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Pako Listening on port %d...\n", PORT);

    // Accept connections
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) != INVALID_SOCKET) {
        printf("Client connected.\n");
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Request received:\n%s\n", buffer);

        // Send HTTP response
        send(client_socket, response, strlen(response), 0);
        closesocket(client_socket);
        printf("Response sent, connection closed.\n");
    }

    // Cleanup
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

