#include <client_routing_server.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int client_routing_server() {
  unsigned short port = 419;

  int sock = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (sock < 0) {
    return sock;
  }

  struct hostent* host_info;
  host_info = gethostbyname("localhost");
  if (host_info == (void*)0) {
    return 1;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_status = bind(
    sock,
    (struct sockaddr *) &server,
    sizeof(server)
  );

  if (bind_status < 0) {
    return bind_status;
  }

  int listen_status = listen(
    sock,
    1
  );
  
  if (listen_status < 0) {
    return listen_status;
  }

  struct sockaddr client;
  socklen_t client_length;

  int csock = accept(
    sock,
    &client,
    &client_length
  );

  if (csock < 0) {
    return csock;
  }

  char message[20];

  ssize_t bytes_length = recv(
    csock,
    message,
    20,
    0
  );

  printf("%s\n", message);

  sleep(1);

  send(
    csock,
    "hi!",
    4,
    0
  );

  close(sock);

  return 0;
}

