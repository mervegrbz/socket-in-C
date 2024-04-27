#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <sys/time.h>

#include <sys/ioctl.h>

#include <netinet/in.h>
// import strcopy
#include <string.h>
int MAX_CONNECTIONS = 5;

struct Client
{
  int id;
  int fd;
};
int create_socket(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, MAX_CONNECTIONS) < 0) {
        perror("Error listening on socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main()
{
  int server_square = create_socket(5010);
  int server_cube = create_socket(5020);


  // get the new connection to the server
  fd_set readfds;

  FD_ZERO(&readfds);
  // Put the fd of the socket to the fd_set
  printf("(inetd) inetd has started\n");
  printf("(inetd) Waiting for ports 5010 & 5020\n");

  while (1)
  {
  FD_SET(server_square, &readfds);
  FD_SET(server_cube, &readfds);
  int last_fd = server_square > server_cube ? server_square : server_cube;
  int activity = select(last_fd + 1, &readfds, NULL, NULL, NULL);
  if (activity < 0)
  {
    perror("Error in select");
    exit(EXIT_FAILURE);
   
  }
  if (FD_ISSET(server_square, &readfds))
  {
    printf("(inetd) Connection request to square service\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_square, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
      perror("Error accepting connection");
      exit(EXIT_FAILURE);
    }
      char buffer[256];
      int n = read(client_fd, buffer, 255);
      if (n < 0)
      {
        perror("Error reading from socket");
        exit(EXIT_FAILURE);
      }
      buffer[n] = '\0';
      int number = atoi(buffer);
      printf("(square)  Request: %d \n", number);
      fflush(stdout);
      int square = number * number;
      char response[256];
      sprintf(response, "Square of %d is %d\n", number, square);
      n = write(client_fd, response, strlen(response));
      printf("(square)  Reply sent as %d. Terminating...\n", square);
      
      if (n < 0)
      {
        perror("Error writing to socket");
        exit(EXIT_FAILURE);
      }
      close(client_fd);
     
    }
    if (FD_ISSET(server_cube, &readfds))
    {
      printf("(inetd) Connection request to cube service\n");
      
      struct sockaddr_in client_addr;
      socklen_t client_addr_len = sizeof(client_addr);
      int client_fd = accept(server_cube, (struct sockaddr *)&client_addr, &client_addr_len);
      if (client_fd < 0)
      {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
      }
      char buffer[256];
      int n = read(client_fd, buffer, 255);
      if (n < 0)
      {
        perror("Error reading from socket\n");

        exit(EXIT_FAILURE);
      }
      buffer[n] = '\0';
      int number = atoi(buffer);
      printf("(cube)  Request: %d\n", number);
      int cube = number * number * number;
      char response[256];
      sprintf(response, "Cube of %d is %d\n", number, cube);
      n = write(client_fd, response, strlen(response));
      printf("(cube)  Reply sent as %d. Terminating...\n", cube);
      fflush(stdout);
      if (n < 0)
      {
        perror("Error writing to socket");
        exit(EXIT_FAILURE);
      }
      close(client_fd);
      
    }
    
  }


}