#include <client_routing_client.h>

#include <client_routing_stage.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int client_routing_client(
  unsigned char host,
  unsigned short int port
) {
  int socket_client = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (socket_client < 0) {
    return socket_client;
  }

  struct hostent *host_info;
  host_info = gethostbyname("localhost");
  if (host_info == (void*)0) {
    return 1;
  }

  struct sockaddr_in client;
  client.sin_family = AF_INET;
  client.sin_port = htons(port);
  client.sin_addr = *(struct in_addr*) host_info->h_addr;

  int status_socket_client = connect(
    socket_client,
    (struct sockaddr*) &client,
    sizeof(client)
  );

  if (status_socket_client < 0) {
    fprintf(
      stderr,
      "failed_to_connect\n"
    );

    return status_socket_client;
  }

  char message[20];

  enum client_routing_stage routing_stage = initialization;
  unsigned char connected = 1;

  if (host == 1) {
    do {
      switch (routing_stage) {
        case initialization:
          int status = send(
            socket_client, 
            "h",
            2,
            0
          );

          char id[11];
          id[10] = '\0';

          recv(
            socket_client,
            id,
            10,
            0
          );

          printf(
            "connection_id: %s\n",
            id
          );

          routing_stage = messaging;
          break;
        case messaging:
          char* message = malloc(
            sizeof(char) * 200
          );

          recv(
            socket_client,
            message,
            200,
            0
          );

          printf(
            "message_recieved: %s",
            message
          );

          free(message);
        case exiting:
        default:
          break;
      }
    } while(connected == 1);
  } else {
    do {
      switch (routing_stage) {
        case initialization:
          send(
            socket_client,
            "c",
            2,
            0
          );

          char* id_client_host = (void*)0;
          unsigned long int length_id_client_host = 0;

          unsigned char valid_id_client_host = 0;

          do {
            do {
              printf("client_host_id: ");

              getline(
                &id_client_host,
                &length_id_client_host,
                stdin
              );

              if (length_id_client_host < 10) {
                fprintf(
                  stderr,
                  "invalid_client_host_id[%li]: %s\n",
                  length_id_client_host,
                  id_client_host
                );

                free(id_client_host);
                length_id_client_host = 0;
              } else {
                valid_id_client_host = 1;
              }
            } while (valid_id_client_host == 0);

            send(
              socket_client,
              id_client_host,
              length_id_client_host,
              0
            );

            free(id_client_host);

            char* valid_id_client_host_char_array = malloc(
              sizeof(char) * 2
            );

            recv(
              socket_client,
              valid_id_client_host_char_array,
              2,
              0
            );

            valid_id_client_host = (
              valid_id_client_host_char_array[0] == 'c'
              ? 1
              : 0
            );

            free(valid_id_client_host_char_array);

            if (
              valid_id_client_host != 1
            ) {
              fprintf(
                stderr,
                "invalid_host_id\n"
              );
            }
          } while (valid_id_client_host != 1);

          printf(
            "client_connected_to_host\n"
          );
          routing_stage = messaging;
          break;
        case messaging:
          unsigned long int length_message = 200;
          char* message = malloc(
            sizeof(char) * length_message
          );

          printf("message: ");
          getline(
            &message,
            &length_message,
            stdin
          );

          send(
            socket_client,
            message,
            200,
            0
          );        
        case exiting:
        default:
          break;
      }
    } while (connected == 1);
  }

  close(socket_client);

  return 0;
}
