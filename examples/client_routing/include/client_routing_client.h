#ifndef __client_routing_client_h
#define __client_routing_client_h

#include <client_routing_stage.h>

#include <netdb.h>
#include <pthread.h>

struct client_routing {
  int socket_client;
  struct hostent* host_info;
  struct sockaddr_in client;
  int status_socket_client;
  enum client_routing_stage routing_stage;
  unsigned char connected;
  pthread_t pthread;
};

int client_routing_client(
  unsigned char,
  unsigned short int
);

void* client_routing_client_thread(
  void*
);

#endif
