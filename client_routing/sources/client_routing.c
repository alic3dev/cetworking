#include <client_routing.h>

#include <client_routing_parameters.h>
#include <client_routing_client.h>
#include <client_routing_server.h>

#include <clic3.h>

#include <stdio.h>

int main(
  int length_parameters,
  char** parameters
) {
  struct client_routing_parameters client_routing_parameters;
  client_routing_parameters.mode = server;

  for (
    int parameters_index = 1;
    parameters_index < length_parameters;
    ++parameters_index
  ) {
    switch(parameters_index) {
      case 1:
        int index_client_server = clic3_char_arrays_within(
          parameters[
            parameters_index
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
