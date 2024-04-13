#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "broker.h"
#define MAX 500
#define PORT 1883
#include "basics.h"
#include "clientslist.h"
#include "convertion.h"
#include "brokerprocessor.h"
#include "topic.h"
//Clients * clist;
//pthread_mutex_t clist_mutex = PTHREAD_MUTEX_INITIALIZER;
ssize_t bytes_rw;
struct topic *root;
void* network_connection(void * arg){
int connfd=*((int *)arg);
  uint8_t buff[MAX];
  uint8_t *client_id;
  int n;
  for(;;){
    bytes_rw=read(connfd, buff, sizeof(buff));
    // Here we proccess the buffer
    uint8_t keep_connection=process_packet(connfd,&buff[0],&client_id);
    if(keep_connection==0)break;
    // Here we clean the buffer again
    bzero(buff,MAX);
    
  }
}
int main() {
  clist=Clients_newList();
  uint8_t * name_root=(uint8_t *)malloc(1);
  strcpy((char *)name_root,"");
  root=create_topic(name_root);
  // Server socket id
  int sockfd, ret;

  // Server socket address structures
  struct sockaddr_in serverAddr;

  // Client socket id
  int clientSocket;

  // Client socket address structures
  struct sockaddr_in cliAddr;

  // Stores byte size of server socket address
  socklen_t addr_size;

  // Child process id
  pid_t childpid;

  // Creates a TCP socket id from IPV4 family
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // Error handling if socket id is not valid
  if (sockfd < 0) {
    printf("Error in connection.\n");
    exit(1);
  }

  printf("Server Socket is created.\n");

  // Initializing address structure with NULL
  memset(&serverAddr, '\0', sizeof(serverAddr));

  // Assign port number and IP address
  // to the socket created
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);

  // 127.0.0.1 is a loopback address
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  // Binding the socket id with
  // the socket structure
  ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

  // Error handling
  if (ret < 0) {
    printf("Error in binding.\n");
    exit(1);
  }

  // Listening for connections (upto 10)
  if (listen(sockfd, 10) == 0) {
    printf("Listening...\n\n");
  }

  int cnt = 0;
  while (1) {

    // Accept clients and
    // store their information in cliAddr
    clientSocket = accept(sockfd, (struct sockaddr *)&cliAddr, &addr_size);

    // Error handling
    if (clientSocket < 0) {
      exit(1);
    }

    // Displaying information of
    // connected client

    printf("Connection accepted from %s:%d\n", inet_ntoa(cliAddr.sin_addr),
           ntohs(cliAddr.sin_port));

    // Print number of clients
    // connected till now
    printf("Clients connected: %d\n\n", ++cnt);

    // Creates a child process
   // if ((childpid = fork()) == 0) {
    //  network_connection(clientSocket);
   // }
	pthread_t tid;
pthread_create(&tid,NULL,network_connection,&clientSocket);
  }

  // Close the client socket id
  close(clientSocket);
  return 0;
}
