#include "backlog.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define MAX_CLIENT_ID 23
#define MAX_CREDENTIALS 65535
#define PORT 8080
#define broker_ip "44.212.64.234"
#define SA struct sockaddr

#include "clientprocessor.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int bytes_rw;
  void func(int sockfd) {
    uint8_t buff_client[MAX];
    uint8_t buff_broker[MAX];

    uint8_t carta[21] = {0x10, 0x13, 0x00, 0x04, 0x4d, 0x51, 0x54,
                         0x54, 0x04, 0xca, 0x00, 0x0a, 0x00, 0x01,
                         0x08, 0x00, 0x01, 0x43, 0x00, 0x01, 0x42};

    int want_connect;
    uint8_t connected = 0x00;

    printf("Hola, para acceder a los servicios del servidor, debes realizar un "
           "CONNECT:\n 1.Continuar\n 2.Salir\n");
    int response = scanf("%d", &want_connect);

    if (response != 1) {
      printf("Te esperamos pronto nuevamente\n");
      return;

    } else {
      int response1;
      printf(
          "Tienes/Deseas credenciales para realizar el CONNECT:\n 1.Sí\n 2.No\n");
      int credentials = scanf("%d", &response1);

      if (credentials != 1) {

        // int response2;
        // printf("Ingresa un ID de cliente válido:\n");
        // uint8_t client_id = scanf("%d", &response2);

        int write_connect = write(sockfd, carta, sizeof(carta));
        int x =read(sockfd, buff_broker, sizeof(buff_broker));
        connected = process_packet(sockfd, &buff_broker[0]);
      } else {

        uint8_t user_connect[21];

        char client_name[MAX_CREDENTIALS];
        printf("Ingresa usuario:\n");
        char name_credential = scanf("%s", client_name);

        char client_password[MAX_CREDENTIALS];
        printf("Ingresa contraseña:\n");
        char password_credential = scanf("%s", client_password);

        uint8_t client_id[MAX_CLIENT_ID];
        printf("Ingresa contraseña:\n");
        uint8_t client_id_credential = scanf("%s", client_id);

        size_t client_name_len = sizeof(name_credential);
        size_t client_password_len = sizeof(password_credential);
        size_t client_id_len = sizeof(client_id_credential);

        // int write_connect = write(sockfd, default_connect,
        // sizeof(default_connect));
      }
    }

    if (connected) {
      int n;
      for (;;) {
        bzero(buff_client, sizeof(buff_client));

        printf("Hola, escribe tu comando: \n");
        int respuesta;

        printf("Selleciona un número:\n 1.SUBSCRIBE\n 2.PUBLISH\n 3.DISCONNECT\n "
               "4.EXIT\n");
        n = 0;
        int g = scanf("%d", &respuesta);

        int f;
        switch (respuesta) {
        case 1:
          break;
        case 2:
          printf("¡Adiós!\n");
          break;
        case 3:
          send_disconnect(sockfd);
          uint8_t disconnected;
          disconnected = process_packet(sockfd, &buff_broker[0]);
          break;
        }

        while ((buff_client[n++] = getchar()) != '\n')
          ;

        char *path = "log.log";

        bzero(buff_broker, sizeof(buff_broker));
        int s = read(sockfd, buff_broker, sizeof(buff_broker));
        printf("From Server : %s", buff_broker);

        // write_log_to_client(*path, client_ip , buff_client, buff_broker);
        if ((strncmp((const char *)buff_client, "EXIT", 4)) == 0) {
          printf("Client Exit...\n");
          break;
        }
      }
    }
  }

  int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
      printf("socket creation failed...\n");
      exit(0);
    } else
      printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(broker_ip);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
      printf("connection with the server failed...\n");
      exit(0);
    } else
      printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
  }
