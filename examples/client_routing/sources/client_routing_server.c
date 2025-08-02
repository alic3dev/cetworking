#include <client_routing_server.h>

#include <client_routing_mode.h>
#include <client_routing_stage.h>

#include <clic3.h>

#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char hostname[9] = "localhost";

int client_routing_server(
  unsigned short int port
) {
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

  server.index_connected = malloc(
    sizeof(unsigned int) *
    server.length_clients
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

  server.routing_mode_clients = malloc(
    sizeof(enum client_routing_mode) *
    server.length_clients
  );

  server.pthread_clients = malloc(
    sizeof(pthread_t) *
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

    server.index_connected = realloc(
      server.index_connected,
      sizeof(unsigned int) *
      server.length_clients
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

    server.routing_mode_clients = realloc(
      server.routing_mode_clients,
      sizeof(enum client_routing_mode) *
      server.length_clients
    );

    server.pthread_clients = realloc(
      server.pthread_clients,
      sizeof(pthread_t) *
      server.length_clients
    );

    struct server_routing_data_thread_client data_thread_client;
    data_thread_client.server = &server;
    data_thread_client.index_client = index_client;

    pthread_create(
      &server.pthread_clients[
        index_client
      ],
      (void*)0,
      client_routing_server_thread_client,
      (void*) &data_thread_client
    );

    running = 1;
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
  free(server.index_connected);
  free(server.routing_mode_clients);

  for (
    unsigned int index_client = 0;
    index_client < server.length_clients;
    ++index_client
  ) {
    pthread_join(
      server.pthread_clients[index_client],
      (void*)0
    );
  }

  free(server.pthread_clients);

  close(server.socket_server);

  return 0;
}

void* client_routing_server_thread_client(
  void* _data
) {
  struct server_routing_data_thread_client* data = (
    (struct server_routing_data_thread_client*) _data
  );

  struct server_routing* server = data->server;
  unsigned int index_client = data->index_client;

  char client_mode[2];

  ssize_t bytes_length = recv(
    server->socket_clients[
      index_client
    ],
    client_mode,
    2,
    0
  );

  server->routing_mode_clients[
    index_client
  ] = (
    client_mode[0] == 'c'
    ? client
    : client_mode[0] == 'h'
    ? client_host
    : -1
  );
  
  server->id_client_host[
    index_client
  ][10] = '\0';

  unsigned char connected = 1;
  enum client_routing_stage routing_stage = initialization;

  while (connected) {
    switch (
      server->routing_mode_clients[
        index_client
      ]
    ) {
      case client:
        switch (routing_stage) {
          case initialization: {
            printf("client_connect:mode: client\n");

            bytes_length = recv(
              server->socket_clients[
                index_client
              ],
              server->id_client_host[
                index_client
              ],
              10,
              0
            );

            server->id_client_host[
              index_client
            ] = realloc(
              server->id_client_host[
                index_client
              ],
              sizeof(char) * 11
            );

            server->id_client_host[
              index_client
            ][10] = '\0';

            unsigned char exists_client_host = 0;

            for (
              unsigned int index_client_lookup = 0;
              index_client_lookup < server->length_clients;
              ++index_client_lookup
            ) {
              printf(
                "%s : %s\n",
                server->id_client_host[
                  index_client_lookup
                ],
                server->id_client_host[
                  index_client
                ]
              );

              if (
                index_client != index_client_lookup &&
                server->routing_mode_clients[
                  index_client_lookup
                ] == client_host &&
                clic3_char_arrays_equal(
                  server->id_client_host[
                    index_client
                  ],
                  server->id_client_host[
                    index_client_lookup
                  ]
                ) == 1
              ) {
                server->index_connected[
                  index_client
                ] = index_client_lookup;

                server->index_connected[
                  index_client_lookup
                ] = index_client;

                exists_client_host = 1;
                break;
              }
            }

            if (exists_client_host) {
              printf(
                "client[%i].connecting_to->{%s}\n",
                index_client,
                server->id_client_host[
                  index_client
                ]
              );

              send(
                server->socket_clients[
                  index_client
                ],
                "c",
                2,
                0
              );

              routing_stage = messaging;
            } else {
              send(
                server->socket_clients[
                  index_client
                ],
                "i",
                2,
                0
              );

              fprintf(
                stderr,
                "failed_to_connect_client_to_invalid_host\n"
              );
            }
            break;
          }
          case messaging: {
            char* message = malloc(
              sizeof(char) * 200
            );

            recv(
              server->socket_clients[
                index_client
              ],
              message,
              200,
              0
            );

            if (message[0] == '!') {
              routing_stage = exiting;
            }

            send(
              server->socket_clients[
                server->index_connected[index_client]
              ],
              message,
              200,
              0
            );

            free(message);
            break;
          }
          case exiting:
          default:
            connected = 0;
            break;
        }
        break;
      case client_host:
        switch(routing_stage) {
          case initialization: {
            printf("client_connect:mode: client_host\n");

            for (
              unsigned char index_id_client_host = 0;
              index_id_client_host < 10;
              ++index_id_client_host
            ) {
              server->id_client_host[
                index_client
              ][index_id_client_host] = (rand() % 10) + '0';
            }

            server->id_client_host[
              index_client
            ][10] = '\0';

            send(
              server->socket_clients[
                index_client
              ],
              server->id_client_host[
                index_client
              ],
              10,
              0
            );

            routing_stage = messaging;
            break;
          }
          case messaging: {
            char* message = malloc(
              sizeof(char) * 200
            );

            recv(
              server->socket_clients[
                index_client
              ],
              message,
              200,
              0
            );

            send(
              server->socket_clients[
                server->index_connected[index_client]
              ],
              message,
              200,
              0
            );

            free(message);
          }
          case exiting:
          default:
            connected = 0;
            break;
        }
        break;
      default:
        fprintf(
          stderr,
          "unknown_client_routing_mode: %s\n",
          client_mode
        );
        connected = 0;
        break;
    }
  }

  return (void*)0;
}
