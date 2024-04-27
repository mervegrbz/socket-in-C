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
  
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
  // Set the maximum number of listening fds to 5
  listen(server_sockfd, 5);
  FD_ZERO(&readfds);
  // Put the fd of the socket to the fd_set
  FD_SET(server_sockfd, &readfds);
  // create a list for the client socket  with client struc
  struct Client clients[10];
  int client_count = 0; // count the number of clients
  printf("Server has started\nWaiting for connections\n");

  while (1)
  {
    int fd;
    int nread;
    testfds = readfds;
    result = select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
    if (result < 1)
    {
      perror("Failed to select!\n");
      exit(1);
    }

    // Loop all the file descriptors
    for (fd = 0; fd < FD_SETSIZE; fd++)
    {
      // Find the fd that associated event occurs
      if (FD_ISSET(fd, &testfds))
      {
        // Determine if it is a server socket
        // if yes, it indicates that the client requests a connection
        if (fd == server_sockfd)
        {
          client_len = sizeof(client_address);
          client_sockfd = accept(server_sockfd,
                                 (struct sockaddr *)&client_address, &client_len);
          // Add the client socket to the collection
          FD_SET(client_sockfd, &readfds);  
          // add the client to the list
          struct Client c;
          c.id = -1;
          c.fd = client_sockfd;
          clients[client_count] = c;
          client_count++;
        }
        // If not, it means there is data request from the client socket
        else
        {
          ioctl(fd, FIONREAD, &nread);

          if (nread == 0) continue;
          // Processing the client data requests
          else
          {
            int index = -1;
            for (int i = 0; i < client_count; i++)
            {
              if (clients[i].fd == fd)
              {
                index = i;
                // printf("Found the client index: %d, client id %d\n", i, index);
                break;
              }
            }
            if( index < 0)
            {
              printf("Client not found\n" );
              exit (1);
            }

            if (clients[index].id == -1)
            {
              char msg[100];

              read(fd, msg, 100);
              clients[index].id = atoi(msg);
              printf("Incoming request from client #%d\n",clients[index].id);
            }
            else
            {
              char msg[100];
              read(fd, msg, 100);
              printf("client (#%d) Request: %s\n", clients[index].id, msg);

              int i = atoi(msg);
              if (i < 0)
              {
                close(fd);
                FD_CLR(fd, &readfds);
                printf(" dropping connection\n", clients[index].id, msg);
              }
              else
              {
                int square = i * i;
                char str[100];
                sprintf(str, "%d", square);

                write(fd, str, 100);
              }
            }
          }
        }
      }
    }
  }

  return 0;
}