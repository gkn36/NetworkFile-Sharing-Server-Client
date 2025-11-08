# Makefile for File Sharing Application
# Simple compilation for server and client

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Target executables
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

# Default target - build both
all: $(SERVER) $(CLIENT)
	@echo "Build complete! Run './server' and './client' in separate terminals"

# Build server
server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER)
	@echo "Server compiled successfully"

# Build client
client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT)
	@echo "Client compiled successfully"

# Create necessary directories
setup:
	mkdir -p server_files
	mkdir -p client_files
	@echo "Directories created"

# Clean build files
clean:
	rm -f $(SERVER) $(CLIENT)
	@echo "Cleaned build files"

# Clean everything including directories
cleanall: clean
	rm -rf server_files client_files
	@echo "Cleaned all files and directories"

# Help
help:
	@echo "Available targets:"
	@echo "  make all      - Build both server and client"
	@echo "  make server   - Build only server"
	@echo "  make client   - Build only client"
	@echo "  make setup    - Create necessary directories"
	@echo "  make clean    - Remove executables"
	@echo "  make cleanall - Remove executables and directories"
	@echo "  make help     - Show this help message"

.PHONY: all server client setup clean cleanall help
