
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Targets
TARGETS = server client

# Build all binaries
all: $(TARGETS)

# Build server
server: server.c
	$(CC) $(CFLAGS) -o server server.c

# Build client
client: client.c
	$(CC) $(CFLAGS) -o client client.c

# Clean binary files
clean:
	rm -f $(TARGETS)