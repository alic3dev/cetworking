#include <client_routing.h>

#include <client_routing_parameters.h>
#include <client_routing_client.h>
#include <client_routing_server.h>

#include <clic3.h>

#include <stdio.h>

int main(
  int arguments_length,
  char** arguments
) {
  struct client_routing_parameters client_routing_parameters;
  client_routing_parameters.mode = server;

  for (
    int arguments_index = 1;
    arguments_index < arguments_length;
    ++arguments_index
  ) {
    switch(arguments_index) {
      case 1:
        int index_client_server = clic3_char_arrays_within(
          arguments[
            arguments_index
          ],
          2,
          "client",
          "server"
        );
        
        if (index_client_server == 0) {
          client_routing_parameters.mode = client;
          break;
        } else if (index_client_server == 1) {
          client_routing_parameters.mode = server;
          break;
        }
      default:
        fprintf(
          stderr,
          "unknown_argument: %s\n", 
          arguments[
            arguments_index
          ]
        );

        return 1;
    }
  }

  switch (client_routing_parameters.mode) {
    case client:
      return client_routing_client();
      break;
    case server:
      return client_routing_server();
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

