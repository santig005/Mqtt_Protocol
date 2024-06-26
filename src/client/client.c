#include "backlog.h"
#include "basics.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 500
#define MAX_CLIENT_ID 23
#define MAX_CREDENTIALS 65535
#define PORT 1883
#define broker_ip "54.85.112.30"
#define SA struct sockaddr

#include "clientprocessor.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
int bytes_rw;
void network_connection(int sockfd) {
  char buff_client[MAX];
  char buff_broker[MAX];
  uint8_t client_id[24];
  uint8_t connected = 0;
  int n;
  int election;
  int scanf_r;
  printf("Hola, para acceder a los servicios del servidor, debes realizar un "
         "CONNECT:\n 1.Continuar\n 2.Salir\n");
  scanf_r = scanf("%d", &election);

  if (election != 1) {
    printf("Te esperamos pronto nuevamente\n");
    return;

  } else {
    struct connect *customed_connect =
        (struct connect *)malloc(sizeof(struct connect));
    customed_connect->header.basic_header.byte = B_CONNECT;
    printf(
        "Tienes/Deseas credenciales para realizar el CONNECT:\n 1.Sí\n 2.No\n");
    scanf_r = scanf("%d", &election);
    if (election == 1) {
      printf("Ingresa un ID de cliente válido maximo 23 characteres "
             "alfanumericos:\n");
      scanf_r = scanf("%s", client_id);
      customed_connect->payload.client_id = &client_id[0];
      printf("Deseas ingresar un usuario y contraseña:\n 1.Sí\n 2.No\n");
      scanf_r = scanf("%d", &election);
      if (election == 1) {
        customed_connect->variable_header.connect_flags.bits.username = 1;
        customed_connect->variable_header.connect_flags.bits.password = 1;
        int username_length;
        int password_length;
        printf("Ingresa la longitud del usuario maximo 65535:\n");
        scanf_r = scanf("%d", &username_length);
        uint8_t *username = (uint8_t *)malloc(username_length + 1);
        printf("Ingrese el nombre de usario maximo %d caracteres:\n",
               username_length);
        scanf_r = scanf("%s", username);
        customed_connect->payload.username = username;
        printf("Ingresa la longitud de la contraseña maximo 65535:\n");
        scanf_r = scanf("%d", &password_length);
        uint8_t *passwd = (uint8_t *)malloc(password_length + 1);
        printf("Ingrese la contraseña maximo %d caracteres:\n",
               password_length);
        scanf_r = scanf("%s", passwd);
        customed_connect->payload.password = passwd;
      } else {
        customed_connect->variable_header.connect_flags.bits.username = 0;
        customed_connect->variable_header.connect_flags.bits.password = 0;
      }
      customed_connect->variable_header.connect_flags.bits.will_flag = 0;
      customed_connect->variable_header.connect_flags.bits.will_qos = 0;
      // name and lenght mqtt
      customed_connect->variable_header.protocol_name.length = 4;
      uint8_t name_protocol[] = "MQTT";
      customed_connect->variable_header.protocol_name.name = name_protocol;

      customed_connect->variable_header.protocol_level = 4;
      customed_connect->variable_header.keep_alive = 1000;
      // retain, clean session y reserved en 0
      customed_connect->variable_header.connect_flags.bits.clean_session = 0;
      customed_connect->variable_header.connect_flags.bits.reserverd = 0;
      customed_connect->variable_header.connect_flags.bits.will_retain = 0;

      send_connect(sockfd, customed_connect);
      // hacemos lectura y luego procesamos connack
      bytes_rw = read(sockfd, buff_broker, sizeof(buff_broker));
      connected = process_packet(sockfd, &buff_broker[0]);
      uint8_t connack_response = process_packet(sockfd, &buff_broker[0]);
    } else {
      uint8_t default_connect[21] = {0x10, 0x13, 0x00, 0x04, 0x4d, 0x51, 0x54,
                                     0x54, 0x04, 0xca, 0x00, 0x0a, 0x00, 0x01,
                                     0x08, 0x00, 0x01, 0x43, 0x00, 0x01, 0x42};
      bytes_rw = write(sockfd, default_connect, sizeof(default_connect));
    }
  }
  if (connected) {
    int stay_connected = 1;
    for (;;) {
      bzero(buff_client, sizeof(buff_client));

      printf("Hola, escribe tu comando: \n");
      int respuesta;

      printf("Selleciona un número:\n 1.SUBSCRIBE\n 2.PUBLISH\n 3.DISCONNECT\n");
      n = 0;
      int g = scanf("%d", &respuesta);

      int f;
      struct subscribe *customed_subscribe =
          (struct subscribe *)malloc(sizeof(struct subscribe));
      switch (respuesta) {

      case 1:
        customed_subscribe->header.basic_header.byte = B_SUBSCRIBE;
        customed_subscribe->variable_header.packet_id = 1;
        int num_topics;
        printf("Ingresa el número de temas a los que quieres suscribirte, "
               "minimo 1\n");
        scanf_r = scanf("%d", &num_topics);
        struct packet_topic *topics_pointer = (struct packet_topic *)malloc(
            num_topics * sizeof(struct packet_topic));
        customed_subscribe->payload.tuples_len = num_topics;
        customed_subscribe->payload.tuples = topics_pointer;

        while (num_topics > 0) {
          int topic_length;
          printf("Ingresa la longitud del máxima del tema, maximo 65535\n");
          scanf_r = scanf("%d", &topic_length);
          uint8_t *topic = (uint8_t *)malloc(topic_length + 1);
          printf("Ingresa el tema, maximo %d caracteres\n", topic_length);
          scanf_r = scanf("%s", topic);
          customed_subscribe->payload.tuples[num_topics - 1].topic_len =
              topic_length;
          customed_subscribe->payload.tuples[num_topics - 1].topic = topic;
          customed_subscribe->payload.tuples[num_topics - 1].qos = 0;
          num_topics -= 1;
        }
        printf("voy a entrar a send subscribe\n");
        send_subscribe(sockfd, customed_subscribe);
        break;
      case 2:
        printf("¡Adiós!\n");
        break;
      case 3:
        send_disconnect(sockfd);
        stay_connected = 0;
        break;
      }
      if (!stay_connected)
        break;
      bzero(buff_broker, sizeof(buff_broker));
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

  network_connection(sockfd);

  // close the socket
  close(sockfd);
}