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
  struct client_routing client;

  client.socket_client = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (
    client.socket_client < 0
  ) {
    return client.socket_client;
  }

  client.host_info = gethostbyname("localhost");

  if (
    client.host_info == (void*)0
  ) {
    return 1;
  }

  client.client.sin_family = AF_INET;
  client.client.sin_port = htons(port);
  client.client.sin_addr = (
    *(struct in_addr*) client.host_info->h_addr
  );

  client.status_socket_client = connect(
    client.socket_client,
    (struct sockaddr*) &client.client,
    sizeof(client.client)
  );

  if (
    client.status_socket_client < 0
  ) {
    fprintf(
      stderr,
      "failed_to_connect\n"
    );

    return client.status_socket_client;
  }

  client.routing_stage = initialization;
  client.connected = 1;

  do {
    if (host == 1) {
      int status = send(
        client.socket_client, 
        "h",
        2,
        0
      );

      char id[11];
      id[10] = '\0';

      recv(
        client.socket_client,
        id,
        10,
        0
      );

      printf(
        "connection_id: %s\nwaiting_for_client\n",
        id
      );
    } else {
      send(
        client.socket_client,
        "c",
        2,
        0
      );

      char* id_client_host = (void*)0;
      unsigned long int length_id_client_host = 0;

      unsigned char valid_id_client_host = 0;

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
        continue;
      } else {
        valid_id_client_host = 1;
      }

      send(
        client.socket_client,
        id_client_host,
        length_id_client_host,
        0
      );

      free(id_client_host);

      char* valid_id_client_host_char_array = malloc(
        sizeof(char) * 2
      );

      recv(
        client.socket_client,
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
        continue;
      }

      printf(
        "client_connected_to_host\n"
      );
    }

    client.routing_stage = messaging;
  } while (
    client.routing_stage == initialization
  );

  pthread_create(
    &client.pthread,
    (void*)0,
    client_routing_client_thread,
    (void*) &client
  );

  while(client.routing_stage == messaging) {
    char* message = malloc(
      sizeof(char) * 200
    );

    int status_receive = recv(
      client.socket_client,
      message,
      200,
      0
    );

    if (
      status_receive == -1
    ) {
      client.routing_stage = exiting;
      break;
    }

    printf(
      "\nmessage_received: %s\nmessage:",
      message
    );

    if (message[0] == '!') {
      client.routing_stage = exiting;
    }

    fflush(stdout);

    free(message);
  }

  pthread_join(
    client.pthread,
    (void*)0
  );

  close(client.socket_client);

  return 0;
}

void* client_routing_client_thread(
  void* data
) {
  struct client_routing* client = (
    (struct client_routing*) data
  );

  while(client->routing_stage == messaging) {
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

    int status_send = send(
      client->socket_client,
      message,
      200,
      0
    );

    if (
      status_send == -1 ||
      message[0] == '!'
    ) {
      client->routing_stage = exiting;
    }
  }
}
