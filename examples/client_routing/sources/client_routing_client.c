#include <client_routing_client.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int client_routing_client(
  unsigned char host
) {
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
    fprintf(
      stderr,
      "failed_to_connect\n"
    );

    return ssock;
  }

  char message[20];

  if (host == 1) {
    int status = send(
      sock,
      "h",
      2,
      0
    );

    char id[11];
    id[10] = '\0';

    recv(
      sock,
      id,
      10,
      0
    );

    printf(
      "connection_id: %s\n",
      id
    );
  } else {
    int status = send(
      sock,
      "c",
      2,
      0
    );

    char* id_client_host = (void*)0;
    unsigned long int length_id_client_host = 0;

    unsigned char valid_id_client_host = 0;

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

    int b = send(
      sock,
      id_client_host,
      length_id_client_host,
      0
    );

    free(id_client_host);

    recv(
      sock,
      message,
      20,
      0
    );

    printf("%s\n", message);
  }

  close(sock);

  return 0;
}
