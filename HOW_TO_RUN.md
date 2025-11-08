# How to Run This Project

## Prerequisites
- Linux/Unix system (or WSL on Windows)
- g++ compiler installed
- Basic terminal knowledge

## Step-by-Step Guide

### 1. Compile the Project
```bash
make all
```
This creates two executables: `server` and `client`

### 2. Start the Server
Open a terminal and run:
```bash
./server
```
You should see:
```
Socket created successfully
Socket bound to port 8080
Server listening on port 8080
Waiting for clients...
```

### 3. Start the Client
Open another terminal (keep server running) and run:
```bash
./client
```

### 4. Authenticate
When prompted, enter password: `admin123`

### 5. Use the Application

#### Option 1: List Files
- Select option `1`
- View all files available on server

#### Option 2: Download File
- Select option `2`
- Enter filename (e.g., `sample.txt`)
- File will be downloaded to `client_files/` directory

#### Option 3: Upload File
- First, create a test file in `client_files/` directory
- Select option `3`
- Enter filename
- File will be uploaded to server's `server_files/` directory

#### Option 4: Exit
- Select option `4` to disconnect

## Testing Multiple Clients
1. Keep server running
2. Open multiple terminals
3. Run `./client` in each terminal
4. Each client can work independently (fork() magic!)

## Troubleshooting

### "Port already in use"
Wait 30 seconds or use:
```bash
sudo fuser -k 8080/tcp
```

### "Permission denied"
Make executables:
```bash
chmod +x server client
```

### Compilation errors
Make sure you have g++:
```bash
sudo apt-get install g++
```

## Default Credentials
- **Password**: admin123
- **Port**: 8080
- **Server IP**: 127.0.0.1 (localhost)

## Project Structure After Build
```
.
├── server              # Server executable
├── client              # Client executable
├── server.cpp          # Server source
├── client.cpp          # Client source
├── Makefile
├── server_files/       # Server's shared files
│   ├── sample.txt
│   ├── readme.txt
│   └── test.md
├── client_files/       # Client's local files
├── README.md
└── PROJECT_INFO.md
```

## Interview Tips
Be ready to explain:
- How socket() creates communication endpoint
- Why bind() assigns address to socket
- How listen() marks socket for incoming connections
- What accept() returns (new socket for client)
- How fork() creates child process for each client
- Why we use open(), read(), write() instead of fopen(), fread(), fwrite()
- How XOR encryption works (simple bitwise operation)
- The file transfer protocol you designed

Good luck! 🚀
