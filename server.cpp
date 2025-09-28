#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    char opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        cerr << "Listen failed" << endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << PORT << endl;

    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET new_socket = accept(server_fd, (sockaddr*)&client, &clientSize);
    if (new_socket == INVALID_SOCKET) {
        cerr << "Accept failed" << endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    cout << "Client connected!" << endl;

    char buffer[BUFFER_SIZE] = {0};
    int bytesReceived = recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        cout << "Received: " << buffer << endl;
        send(new_socket, buffer, bytesReceived, 0);
        cout << "Echo sent back to client" << endl;
    }

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}