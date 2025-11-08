# Project Information - Network File Sharing Application

## What is LSP?
**LSP** stands for **Linux System Programming**. It refers to programming at the system level using low-level POSIX APIs provided by the Linux/Unix operating system.

## Role of LSP in This Project

LSP forms the foundation of this entire project. Every core functionality relies on Linux system calls and programming concepts:

### 1. **Socket Programming (Day 1)**
LSP provides socket APIs for network communication:
- `socket()` - Creates a communication endpoint
- `bind()` - Assigns an address to the socket
- `listen()` - Marks socket as passive, ready to accept connections
- `accept()` - Accepts incoming client connection
- `connect()` - Initiates connection to server
- `send()` / `recv()` - Sends and receives data

**Why LSP?** Direct control over network communication without high-level frameworks.

### 2. **File System Operations (Day 2)**
LSP provides file and directory manipulation APIs:
- `opendir()` - Opens a directory stream
- `readdir()` - Reads directory entries
- `closedir()` - Closes directory stream
- `stat()` - Gets file information (size, type, permissions)

**Why LSP?** Direct access to file system for listing files and retrieving metadata.

### 3. **File I/O Operations (Day 3 & 4)**
LSP provides low-level file operations:
- `open()` - Opens a file and returns file descriptor
- `read()` - Reads data from file descriptor
- `write()` - Writes data to file descriptor
- `close()` - Closes file descriptor

**Why LSP?** Efficient file transfers with direct buffer management and chunk-based streaming.

### 4. **Process Management (Multi-client Support)**
LSP provides process control:
- `fork()` - Creates a new process (child process)
- `getpid()` - Gets process ID
- Parent-child process relationship for handling multiple clients

**Why LSP?** Each client connection is handled by a separate process, allowing concurrent file operations.

### 5. **Security Implementation (Day 5)**
While basic, security uses LSP concepts:
- Memory operations for encryption/decryption
- Secure file permissions using `chmod()`
- Input validation and buffer management

## Day-wise Implementation Breakdown

### **Day 1: Server-Client Communication**
**LSP Concepts Used:**
- Socket creation and configuration
- Address binding
- Connection establishment
- Process forking for multi-client support

**What Happens:**
1. Server creates a socket and binds to port 8080
2. Server listens for incoming connections
3. When client connects, server forks a new process
4. Each client is handled independently

### **Day 2: File Listing and Selection**
**LSP Concepts Used:**
- Directory traversal
- File metadata retrieval
- Inter-process communication (sending/receiving data)

**What Happens:**
1. Client requests file list
2. Server opens shared directory
3. Server reads all files and sends names to client
4. Client displays list and allows selection

### **Day 3: File Download (Server → Client)**
**LSP Concepts Used:**
- File reading using file descriptors
- Buffer management
- Network data transfer in chunks

**What Happens:**
1. Client sends download request with filename
2. Server opens requested file
3. Server reads file in chunks (1024 bytes)
4. Server sends each chunk over socket
5. Client receives chunks and writes to local file

### **Day 4: File Upload (Client → Server)**
**LSP Concepts Used:**
- File reading on client side
- Network transmission
- File writing on server side
- Error handling for file operations

**What Happens:**
1. Client opens local file
2. Client reads file in chunks
3. Client sends chunks to server
4. Server receives chunks and writes to server directory

### **Day 5: Security Features**
**LSP Concepts Used:**
- Authentication protocol design
- Data encryption/decryption
- Secure buffer handling

**What Happens:**
1. Client sends password
2. Server validates password
3. File data is XOR encrypted before sending
4. Receiver decrypts data before writing

## Why This Approach is Entry-Level Appropriate

1. **Simple Logic**: Each function does one thing clearly
2. **Basic APIs**: Uses fundamental system calls, not complex libraries
3. **Easy to Debug**: Clear function boundaries and simple control flow
4. **Explainable**: Every system call has a clear purpose
5. **No External Dependencies**: Uses only standard C/C++ and POSIX APIs
6. **Step-by-Step**: Each day builds on previous concepts

## Key Learning Outcomes

By completing this project, you will understand:
- How network applications communicate
- How file systems are accessed programmatically
- How processes are created and managed
- How to implement basic client-server protocols
- Real-world application of system programming concepts

## Technical Stack

- **Language**: C++ (for better string handling while keeping low-level access)
- **System Calls**: POSIX-compliant (Linux/Unix)
- **Network Protocol**: TCP/IP
- **Build System**: GNU Make

## Interview Talking Points

When presenting this project:
1. Explain the difference between high-level (HTTP, FTP) and low-level (raw sockets) approaches
2. Describe how `fork()` enables concurrent client handling
3. Explain the file transfer protocol you designed
4. Discuss trade-offs between security and simplicity
5. Mention how chunked transfer prevents memory overflow

## Conclusion

LSP (Linux System Programming) is not just a requirement—it's the entire foundation of this project. Every feature relies on direct system calls, giving you complete control and deep understanding of how networked file sharing actually works at the OS level.
