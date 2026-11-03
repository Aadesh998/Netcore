#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#include "tcp/config.hpp"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int opt = 1;

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. Bind socket
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 5. Start listening
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << endl;

    // 6. Accept multiple clients sequentially
    while (true) {
        new_socket = accept(server_fd, nullptr, nullptr);

        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }

        cout << "Client connected!" << endl;

        char buffer[BUFFER_SIZE];

        while (true) {
            ssize_t bytes_read = read(
                new_socket, buffer, BUFFER_SIZE
            );

            if (bytes_read == 0) {
                cout << "Client disconnected" << endl;
                break;
            }

            if (bytes_read < 0) {
                perror("read failed");
                break;
            }

            // Echo received data back
            ssize_t total_sent = 0;

            while (total_sent < bytes_read) {
                ssize_t sent = send(
                    new_socket,
                    buffer + total_sent,
                    bytes_read - total_sent,
                    0
                );

                if (sent <= 0) {
                    perror("send failed");
                    break;
                }

                total_sent += sent;
            }

            if (total_sent < bytes_read) {
                break;
            }
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
