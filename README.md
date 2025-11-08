# Network File Sharing - Server & Client Application

## Overview
A simple networked file sharing application built using Linux System Programming (LSP) concepts. This project demonstrates client-server architecture with TCP sockets, enabling file transfers over a network.

## Features
- ✅ Server-Client communication using TCP sockets
- ✅ File listing and selection
- ✅ File download (server to client)
- ✅ File upload (client to server)
- ✅ Basic authentication
- ✅ Simple encryption for secure transfers
- ✅ Multi-client support using fork()

## Project Structure
```
├── server.cpp          # Server implementation
├── client.cpp          # Client implementation
├── Makefile           # Build configuration
├── server_files/      # Server's shared files directory
├── client_files/      # Client's download directory
├── README.md          # This file
└── PROJECT_INFO.md    # Detailed project documentation
```

## Requirements
- Linux/Unix operating system
- g++ compiler
- Basic networking knowledge

## Compilation
```bash
make all
```

Or compile individually:
```bash
make server
make client
```

## Usage

### Start the Server
```bash
./server
```
Server will start on port 8080 and wait for client connections.

### Start the Client
```bash
./client
```
Client will connect to localhost:8080 and display a menu.

### Menu Options
1. **List Files** - View available files on server
2. **Download File** - Download a file from server
3. **Upload File** - Upload a file to server
4. **Exit** - Close connection

## Technical Details
- **Language**: C++
- **Socket Type**: TCP (SOCK_STREAM)
- **Port**: 8080 (configurable)
- **Authentication**: Simple password-based
- **Encryption**: Basic XOR cipher (for demonstration)

## Learning Objectives (LSP Concepts)
- Socket programming: `socket()`, `bind()`, `listen()`, `accept()`, `connect()`
- File operations: `open()`, `read()`, `write()`, `close()`
- Directory operations: `opendir()`, `readdir()`, `closedir()`
- Process management: `fork()` for multi-client handling
- Network communication and protocol design

## Security Note
⚠️ This is an educational project. The security features (basic password and XOR encryption) are for demonstration purposes only and should NOT be used in production environments.

## Author
Capstone Project - Entry Level Networking Application

## License
Educational Use Only
