#ifndef __client_routing_server
#define __client_routing_server

#include <netinet/in.h>
#include <sys/socket.h>

struct server_routing {
  unsigned short int port;
  int socket_server;

  struct sockaddr_in socket_address_server;
  struct hostent* host_entry;

  char** id_client_host;
  struct sockaddr* socket_address_clients;
  socklen_t* socket_length_clients;
  int* socket_clients;
  unsigned int length_clients;
};

extern char hostname[9];
extern unsigned short int port;

int client_routing_server();

#endif
