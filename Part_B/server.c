#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <string.h>

struct Client
{
  int id;
  int fd;
};

int main()
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int result;
  fd_set readfds, testfds;
  // Create the server side socket
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
  // Set the maximum number of listening fds to 5
  listen(server_sockfd, 10);
  // get the new connection to the server
  FD_ZERO(&readfds);
  // Put the fd of the socket to the fd_set
  FD_SET(server_sockfd, &readfds);
  printf("(parent) Server has started\n(parent) Waiting for connections");
  fflush(stdout);
  while (1)
  {
    int connfd = accept(server_sockfd, (struct sockaddr *)NULL, NULL);
    if (connfd < 0)
    {
      perror("server accept failed");
      exit(1);
    }
    // create a new process
    int pid = fork();
    if (pid == 0)
    {
      char client_id[100];
      read(connfd, client_id, 100);
      int id = atoi(client_id);
      printf("(child #%d) Child created for incoming request\n", id );
      // child process
      char str[100];
      while (1)
      {
        read(connfd, str, 100);
        int i = atoi(str);
        if (str[0] == '-')
        {
          printf("(child #%d)Request=%s Will terminate\n", id, str);
          fflush(stdout);
          close(connfd);
          exit(0);
        }
        int square = i*i;
        char square_str[100];
        printf("(child #%d) Request= %s\n", id, str);
        sprintf(square_str, "%d", square);
        write(connfd, square_str, 100);
      }
      exit(0);
    }
    else
    {
      
    }

  }
}