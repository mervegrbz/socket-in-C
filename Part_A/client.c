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
int main(int argc, char *argv[]) {
  int client_sockfd;
  int len;
  struct sockaddr_in address; // Server address structure family/ip/port
  int result;
  char id[100];
  
  client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(8888);
  len = sizeof(address);
  result = connect(client_sockfd, (struct sockaddr * ) & address, len);
  if (result == -1 ) {
    perror("Failed to connect to the server");
    exit(1);
  }
  if (argc > 1) {
    strcpy(id, argv[1]);
  } else {
    strcpy(id, "1");
  }
  printf("This is client: %s\n", id);
  // The first read & write
  write(client_sockfd, id, 100); 

  
  while(1){
    // get the char from user input
    printf("Enter request  (negative to terminate): ");
    char str[100];
    scanf("%s",str);

    if(str[0] == '-'){
      write(client_sockfd, str, 100);
      printf("Will Terminate\n");
      close(client_sockfd);
      break;
    }
   
    // write the char to the server
    write(client_sockfd, str, 100);
    // read the result from the server
    read(client_sockfd, str , 100);

    printf("Result: %s\n", str);
  }


  close(client_sockfd);

  return 0;
}