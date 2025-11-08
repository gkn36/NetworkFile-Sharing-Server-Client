Network File Sharing Project
============================

Project Components:
- Server (server.cpp)
- Client (client.cpp)
- Makefile for compilation

LSP Concepts Used:
1. Socket Programming
   - socket(), bind(), listen(), accept(), connect()
   
2. File Operations
   - open(), read(), write(), close()
   - opendir(), readdir(), closedir()
   
3. Process Management
   - fork() for handling multiple clients
   
4. Security
   - Simple authentication
   - Basic XOR encryption

5. Network Protocol
   - Custom commands: LIST, DOWNLOAD, UPLOAD, EXIT
   - Chunk-based file transfer
   
This is a demonstration of entry-level system programming concepts.
