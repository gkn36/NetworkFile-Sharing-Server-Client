/*
 * File Sharing Server
 * Uses Linux System Programming (LSP) concepts
 * Implements: Socket programming, file operations, process management
 */

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024
#define PASSWORD "admin123"  // Simple authentication for demo

// Simple XOR encryption for demonstration
void xor_encrypt_decrypt(char* data, int length, char key) {
    for(int i = 0; i < length; i++) {
        data[i] = data[i] ^ key;
    }
}

// List files in server directory
void send_file_list(int client_socket) {
    DIR* dir;
    struct dirent* entry;
    char file_list[BUFFER_SIZE] = {0};
    
    // Open the server_files directory
    dir = opendir("server_files");
    if (dir == NULL) {
        strcpy(file_list, "ERROR: Cannot open directory");
        send(client_socket, file_list, strlen(file_list), 0);
        return;
    }
    
    // Read all files and build list
    strcat(file_list, "Available Files:\n");
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            strcat(file_list, entry->d_name);
            strcat(file_list, "\n");
        }
    }
    
    closedir(dir);
    
    // Send file list to client
    send(client_socket, file_list, strlen(file_list), 0);
    cout << "Sent file list to client" << endl;
}

// Send file to client (download)
void send_file(int client_socket, const char* filename) {
    char filepath[256];
    sprintf(filepath, "server_files/%s", filename);
    
    // Open file using low-level file descriptor
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        char error_msg[] = "ERROR: File not found";
        send(client_socket, error_msg, strlen(error_msg), 0);
        cout << "File not found: " << filename << endl;
        return;
    }
    
    // Get file size
    struct stat file_stat;
    fstat(fd, &file_stat);
    long file_size = file_stat.st_size;
    
    // Send file size first
    char size_buffer[32];
    sprintf(size_buffer, "%ld", file_size);
    send(client_socket, size_buffer, strlen(size_buffer), 0);
    
    // Wait for acknowledgment
    char ack[10];
    recv(client_socket, ack, sizeof(ack), 0);
    
    // Read and send file in chunks
    char buffer[BUFFER_SIZE];
    int bytes_read;
    long total_sent = 0;
    
    cout << "Sending file: " << filename << " (Size: " << file_size << " bytes)" << endl;
    
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // Simple encryption
        xor_encrypt_decrypt(buffer, bytes_read, 'K');
        
        // Send chunk
        send(client_socket, buffer, bytes_read, 0);
        total_sent += bytes_read;
    }
    
    close(fd);
    cout << "File sent successfully: " << total_sent << " bytes" << endl;
}

// Receive file from client (upload)
void receive_file(int client_socket, const char* filename) {
    char filepath[256];
    sprintf(filepath, "server_files/%s", filename);
    
    // Receive file size
    char size_buffer[32] = {0};
    recv(client_socket, size_buffer, sizeof(size_buffer), 0);
    long file_size = atol(size_buffer);
    
    if (file_size <= 0) {
        cout << "Invalid file size received" << endl;
        return;
    }
    
    // Send acknowledgment
    char ack[] = "OK";
    send(client_socket, ack, strlen(ack), 0);
    
    // Create file using low-level file descriptor
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        cout << "Error creating file: " << filename << endl;
        return;
    }
    
    cout << "Receiving file: " << filename << " (Size: " << file_size << " bytes)" << endl;
    
    // Receive file in chunks
    char buffer[BUFFER_SIZE];
    long total_received = 0;
    int bytes_received;
    
    while (total_received < file_size) {
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) break;
        
        // Decrypt data
        xor_encrypt_decrypt(buffer, bytes_received, 'K');
        
        // Write to file
        write(fd, buffer, bytes_received);
        total_received += bytes_received;
    }
    
    close(fd);
    cout << "File received successfully: " << total_received << " bytes" << endl;
}

// Authenticate client
bool authenticate_client(int client_socket) {
    char password[BUFFER_SIZE] = {0};
    
    // Receive password from client
    recv(client_socket, password, BUFFER_SIZE, 0);
    
    // Simple password check
    if (strcmp(password, PASSWORD) == 0) {
        char success[] = "AUTH_SUCCESS";
        send(client_socket, success, strlen(success), 0);
        cout << "Client authenticated successfully" << endl;
        return true;
    } else {
        char failure[] = "AUTH_FAILED";
        send(client_socket, failure, strlen(failure), 0);
        cout << "Client authentication failed" << endl;
        return false;
    }
}

// Handle individual client connection
void handle_client(int client_socket) {
    cout << "New client connected (PID: " << getpid() << ")" << endl;
    
    // Authentication
    if (!authenticate_client(client_socket)) {
        close(client_socket);
        exit(0);
    }
    
    // Main communication loop
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive command from client
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            cout << "Client disconnected" << endl;
            break;
        }
        
        cout << "Command received: " << buffer << endl;
        
        // Process commands
        if (strcmp(buffer, "LIST") == 0) {
            send_file_list(client_socket);
        }
        else if (strncmp(buffer, "DOWNLOAD:", 9) == 0) {
            char* filename = buffer + 9;
            send_file(client_socket, filename);
        }
        else if (strncmp(buffer, "UPLOAD:", 7) == 0) {
            char* filename = buffer + 7;
            receive_file(client_socket, filename);
        }
        else if (strcmp(buffer, "EXIT") == 0) {
            cout << "Client requested to exit" << endl;
            break;
        }
    }
    
    close(client_socket);
    exit(0);  // Child process exits
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Ignore zombie processes
    signal(SIGCHLD, SIG_IGN);
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }
    cout << "Socket created successfully" << endl;
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Error binding socket" << endl;
        close(server_socket);
        return 1;
    }
    cout << "Socket bound to port " << PORT << endl;
    
    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        cerr << "Error listening on socket" << endl;
        close(server_socket);
        return 1;
    }
    cout << "Server listening on port " << PORT << endl;
    cout << "Waiting for clients..." << endl;
    
    // Main server loop
    while (true) {
        // Accept client connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }
        
        cout << "Connection accepted from " << inet_ntoa(client_addr.sin_addr) << endl;
        
        // Fork to handle multiple clients
        pid_t pid = fork();
        
        if (pid < 0) {
            cerr << "Error forking process" << endl;
            close(client_socket);
        }
        else if (pid == 0) {
            // Child process handles client
            close(server_socket);  // Child doesn't need server socket
            handle_client(client_socket);
        }
        else {
            // Parent process continues accepting connections
            close(client_socket);  // Parent doesn't need client socket
        }
    }
    
    close(server_socket);
    return 0;
}
