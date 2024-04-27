#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
  // Get the port from the command line
  int port = argc > 1 ? atoi(argv[1]) : 5010;

  int client_sockfd;
  int len;
  struct sockaddr_in address; // Server address structure family/ip/port
  int result;
  char ch = 'A';
  char id[100];
  // Create the client socket
  client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(port);
  len = sizeof(address);
  result = connect(client_sockfd, (struct sockaddr *)&address, len);
  if (result == -1)
  {
    perror("Failed to connect to the server");
    exit(1);
  }
  
  // The first read & write

  // get the char from user input
  printf("Enter request: ");
  char str[100];
  scanf("%s", str);
  write(client_sockfd, str, 100);
  read(client_sockfd, str, 100);
  printf("Result: %s\n", str);
  sleep(1);
  close(client_sockfd);

  return 0;
}