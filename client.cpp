/*
 * File Sharing Client
 * Uses Linux System Programming (LSP) concepts
 * Implements: Socket programming, file operations, user interface
 */

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

#define SERVER_IP "127.0.0.1"  // localhost
#define PORT 8080
#define BUFFER_SIZE 1024
#define PASSWORD "admin123"

// Simple XOR encryption/decryption (same as server)
void xor_encrypt_decrypt(char* data, int length, char key) {
    for(int i = 0; i < length; i++) {
        data[i] = data[i] ^ key;
    }
}

// Authenticate with server
bool authenticate(int sock) {
    cout << "\n=== Authentication ===" << endl;
    cout << "Password: ";
    
    char password[BUFFER_SIZE];
    cin.getline(password, BUFFER_SIZE);
    
    // Send password to server
    send(sock, password, strlen(password), 0);
    
    // Receive authentication result
    char response[BUFFER_SIZE] = {0};
    recv(sock, response, BUFFER_SIZE, 0);
    
    if (strcmp(response, "AUTH_SUCCESS") == 0) {
        cout << "Authentication successful!" << endl;
        return true;
    } else {
        cout << "Authentication failed!" << endl;
        return false;
    }
}

// Request and display file list
void list_files(int sock) {
    char command[] = "LIST";
    send(sock, command, strlen(command), 0);
    
    char file_list[BUFFER_SIZE] = {0};
    recv(sock, file_list, BUFFER_SIZE, 0);
    
    cout << "\n" << file_list << endl;
}

// Download file from server
void download_file(int sock) {
    char filename[256];
    cout << "\nEnter filename to download: ";
    cin.getline(filename, 256);
    
    // Send download command
    char command[BUFFER_SIZE];
    sprintf(command, "DOWNLOAD:%s", filename);
    send(sock, command, strlen(command), 0);
    
    // Receive file size
    char size_buffer[32] = {0};
    recv(sock, size_buffer, sizeof(size_buffer), 0);
    
    // Check for error
    if (strncmp(size_buffer, "ERROR", 5) == 0) {
        cout << "Error: " << size_buffer << endl;
        return;
    }
    
    long file_size = atol(size_buffer);
    if (file_size <= 0) {
        cout << "Invalid file size" << endl;
        return;
    }
    
    // Send acknowledgment
    char ack[] = "OK";
    send(sock, ack, strlen(ack), 0);
    
    // Create file in client_files directory
    char filepath[300];
    sprintf(filepath, "client_files/%s", filename);
    
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        cout << "Error creating file locally" << endl;
        return;
    }
    
    cout << "Downloading file: " << filename << " (" << file_size << " bytes)" << endl;
    
    // Receive file in chunks
    char buffer[BUFFER_SIZE];
    long total_received = 0;
    int bytes_received;
    
    while (total_received < file_size) {
        bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) break;
        
        // Decrypt data
        xor_encrypt_decrypt(buffer, bytes_received, 'K');
        
        // Write to file
        write(fd, buffer, bytes_received);
        total_received += bytes_received;
        
        // Show progress
        int progress = (total_received * 100) / file_size;
        cout << "\rProgress: " << progress << "%" << flush;
    }
    
    close(fd);
    cout << "\nFile downloaded successfully!" << endl;
}

// Upload file to server
void upload_file(int sock) {
    char filename[256];
    cout << "\nEnter filename to upload (from client_files/): ";
    cin.getline(filename, 256);
    
    // Open local file
    char filepath[300];
    sprintf(filepath, "client_files/%s", filename);
    
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        cout << "Error: File not found locally" << endl;
        return;
    }
    
    // Get file size
    struct stat file_stat;
    fstat(fd, &file_stat);
    long file_size = file_stat.st_size;
    
    // Send upload command
    char command[BUFFER_SIZE];
    sprintf(command, "UPLOAD:%s", filename);
    send(sock, command, strlen(command), 0);
    
    // Small delay to ensure command is processed
    usleep(100000);
    
    // Send file size
    char size_buffer[32];
    sprintf(size_buffer, "%ld", file_size);
    send(sock, size_buffer, strlen(size_buffer), 0);
    
    // Wait for acknowledgment
    char ack[10] = {0};
    recv(sock, ack, sizeof(ack), 0);
    
    cout << "Uploading file: " << filename << " (" << file_size << " bytes)" << endl;
    
    // Send file in chunks
    char buffer[BUFFER_SIZE];
    int bytes_read;
    long total_sent = 0;
    
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // Encrypt data
        xor_encrypt_decrypt(buffer, bytes_read, 'K');
        
        // Send chunk
        send(sock, buffer, bytes_read, 0);
        total_sent += bytes_read;
        
        // Show progress
        int progress = (total_sent * 100) / file_size;
        cout << "\rProgress: " << progress << "%" << flush;
    }
    
    close(fd);
    cout << "\nFile uploaded successfully!" << endl;
}

// Display menu and get user choice
int show_menu() {
    cout << "\n=== File Sharing Client ===" << endl;
    cout << "1. List Files" << endl;
    cout << "2. Download File" << endl;
    cout << "3. Upload File" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();  // Clear newline from buffer
    
    return choice;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }
    cout << "Socket created successfully" << endl;
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Convert IP address from string to binary
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        cerr << "Invalid address" << endl;
        close(sock);
        return 1;
    }
    
    // Connect to server
    cout << "Connecting to server..." << endl;
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connection failed" << endl;
        close(sock);
        return 1;
    }
    cout << "Connected to server successfully!" << endl;
    
    // Authenticate
    if (!authenticate(sock)) {
        close(sock);
        return 1;
    }
    
    // Main interaction loop
    bool running = true;
    while (running) {
        int choice = show_menu();
        
        switch (choice) {
            case 1:
                list_files(sock);
                break;
            case 2:
                download_file(sock);
                break;
            case 3:
                upload_file(sock);
                break;
            case 4:
                cout << "Exiting..." << endl;
                send(sock, "EXIT", 4, 0);
                running = false;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
    
    close(sock);
    cout << "Connection closed" << endl;
    return 0;
}
