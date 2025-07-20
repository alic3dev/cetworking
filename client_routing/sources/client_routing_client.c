#include <client_routing_client.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int client_routing_client() {
  unsigned short int port = 419;

  int sock = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (sock < 0) {
    return sock;
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

  int ssock = connect(
    sock,
    (struct sockaddr*) &client,
    sizeof(client)
  );

  if (ssock < 0) {
    return ssock;
  }

  int b = send(
    sock,
    "Hey :)",
    8,
    0
  );

  char message[20];

  recv(
    sock,
    message,
    20,
    0
  );

  printf("%s\n", message);

  close(sock);

  return 0;
}
