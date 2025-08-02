#include <client_routing.h>

#include <client_routing_parameters.h>
#include <client_routing_client.h>
#include <client_routing_server.h>

#include <clic3.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(
  int length_parameters,
  char** parameters
) {
  srand(time((void*)0));

  struct client_routing_parameters client_routing_parameters;
  client_routing_parameters.mode = server;

  unsigned short int port = 419;

  for (
    int parameters_index = 1;
    parameters_index < length_parameters;
    ++parameters_index
  ) {
    int index_client_server;

    switch(parameters_index) {
      case 1:
         index_client_server = clic3_char_arrays_within(
          parameters[
            parameters_index
          ],
          3,
          "client",
          "client_host",
          "server"
        );

        client_routing_parameters.mode = index_client_server;
        break;
      default:
        fprintf(
          stderr,
          "unknown_parameter: %s\n", 
          parameters[
            parameters_index
          ]
        );

        return 1;
    }
  }

  switch (client_routing_parameters.mode) {
    case client:
      return client_routing_client(0, port);
      break;
    case client_host:
      return client_routing_client(1, port);
      break;
    case server:
      return client_routing_server(port);
      break;
    default:
      fprintf(
        stderr,
        "unknown_mode: %i\n",
        client_routing_parameters.mode
      );
      
      return 1;
  }

  return 0;
}
