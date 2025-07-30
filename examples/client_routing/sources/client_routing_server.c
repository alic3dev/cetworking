#include <client_routing_server.h>

#include <client_routing_mode.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char hostname[9] = "localhost";
unsigned short int port = 419;

int client_routing_server() {
  struct server_routing server;
  server.port = port;

  server.socket_server = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (server.socket_server < 0) {
    return server.socket_server;
  }

  server.host_entry = gethostbyname(hostname);

  if (server.host_entry == (void*)0) {
    fprintf(
      stderr,
      "failed_to_get:hostname->{localhost}"
    );

    return 1;
  }

  server.socket_address_server.sin_family = AF_INET;
  server.socket_address_server.sin_port = htons(
    server.port
  );
  server.socket_address_server.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_status = bind(
    server.socket_server,
    (struct sockaddr*) &server.socket_address_server,
    sizeof(server.socket_address_server)
  );

  if (bind_status < 0) {
    fprintf(
      stderr,
      "failed_to_bind_socket\n"
    );

    return bind_status;
  }

  int listen_status = listen(
    server.socket_server,
    1
  );
  
  if (listen_status < 0) {
    fprintf(
      stderr,
      "failed_to_listen_on_socket\n"
    );

    return listen_status;
  }

  server.length_clients = 1;

  server.id_client_host = malloc(
    sizeof(char*) *
    server.length_clients
  );
  server.id_client_host[0] = malloc(
    sizeof(char) * 11
  );

  server.socket_address_clients = malloc(
    sizeof(struct sockaddr) *
    server.length_clients
  );

  server.socket_length_clients = malloc(
    sizeof(socklen_t) *
    server.length_clients
  );

  server.socket_clients = malloc(
    sizeof(int) *
    server.length_clients
  );

  signed char running = 1;

  while (running == 1) {
    unsigned int index_client = (
      server.length_clients - 1
    );

    server.socket_clients[
      index_client
    ] = accept(
      server.socket_server,
      &server.socket_address_clients[
        index_client
      ],
      &server.socket_length_clients[
        index_client
      ]
    );

    if (
      server.socket_clients[
        index_client
      ] < 0
    ) {
      fprintf(
        stderr,
        "failed_to_accept_connection\n"
      );

      continue;
    }

    server.length_clients = (
      server.length_clients + 1
    );

    server.id_client_host = realloc(
      server.id_client_host,
      sizeof(char*) *
      server.length_clients
    );
    server.id_client_host[server.length_clients - 1] = malloc(
      sizeof(char) * 11
    );

    server.socket_address_clients = realloc(
      server.socket_address_clients,
      sizeof(struct sockaddr) *
      server.length_clients
    );

    server.socket_length_clients = realloc(
      server.socket_length_clients,
      sizeof(socklen_t) *
      server.length_clients
    );

    server.socket_clients = realloc(
      server.socket_clients,
      sizeof(int) *
      server.length_clients
    );

    char client_mode[2];

    ssize_t bytes_length = recv(
      server.socket_clients[
        index_client
      ],
      client_mode,
      2,
      0
    );

    enum client_routing_mode routing_mode = (
      client_mode[0] == 'c'
      ? client
      : client_mode[0] == 'h'
      ? client_host
      : -1
    );
    
    server.id_client_host[
      index_client
    ][10] = '\0';

    switch (routing_mode) {
      case client:
        printf("client_connect:mode: client\n");

        bytes_length = recv(
          server.socket_clients[
            index_client
          ],
          server.id_client_host[
            index_client
          ],
          10,
          0
        );

        printf(
          "client[%i].connecting_to->{%s}\n",
          index_client,
          server.id_client_host[
            index_client
          ]
        );

        send(
          server.socket_clients[
            index_client
          ],
          "hi!",
          4,
          0
        );
        break;
      case client_host:
        printf("client_connect:mode: client_host\n");

        for (
          unsigned char index_id_client_host = 0;
          index_id_client_host < 10;
          ++index_id_client_host
        ) {
          server.id_client_host[
            index_client
          ][index_id_client_host] = (rand() % 10) + '0';
        }

        send(
          server.socket_clients[
            index_client
          ],
          server.id_client_host[
            index_client
          ],
          10,
          0
        );
        break;
      default:
        fprintf(
          stderr,
          "unknown_client_routing_mode: %s\n",
          client_mode
        );
        break;
    }

    running = 0;
  }

  for (
    unsigned int index_client = 0;
    index_client < server.length_clients;
    ++index_client
  ) {
    close(
      server.socket_clients[
        index_client
      ]
    );

    free(
      server.id_client_host[
        index_client
      ]
    );
  }

  free(server.id_client_host);
  free(server.socket_address_clients);
  free(server.socket_length_clients);
  free(server.socket_clients);

  close(server.socket_server);

  return 0;
}
