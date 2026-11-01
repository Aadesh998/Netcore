#include <cstddef>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr{};

    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed to create");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    cout << "connected to tcp server" << endl;

    while(true) {


    string message;

    cout << "Enter message: ";
    getline(cin, message);

    // 5. Send message to server
    ssize_t sent = send(sock, message.c_str(), message.size(), 0);

    if (sent < 0) {
        perror("Send failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_received = read(sock, buffer, BUFFER_SIZE - 1);

    if (bytes_received < 0) {
        perror("Read failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (bytes_received == 0) {
        cout << "Server disconnected" << endl;
    } else {
        buffer[bytes_received] = '\0';
        cout << bytes_received << endl;

        cout << "Server response: " << buffer << endl;
    }
    }
    close(sock);
}
