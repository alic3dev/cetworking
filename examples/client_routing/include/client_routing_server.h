#ifndef __client_routing_server
#define __client_routing_server

#include <client_routing_mode.h>
#include <client_routing_stage.h>

#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>

struct server_routing {
  unsigned short int port;
  int socket_server;

  struct sockaddr_in socket_address_server;
  struct hostent* host_entry;

  char** id_client_host;
  signed long int* index_connected;
  struct sockaddr* socket_address_clients;
  socklen_t* socket_length_clients;
  int* socket_clients;
  pthread_t* pthread_clients;
  enum client_routing_mode* routing_mode_clients;
  enum client_routing_stage* routing_stage;
  unsigned int length_clients;
};

struct server_routing_data_thread_client {
  struct server_routing* server;
  unsigned int index_client;
};

extern char hostname[9];
extern unsigned short int port;

int client_routing_server(
  unsigned short int
);

void* client_routing_server_thread_client(
  void*
);

#endif
