#include <tcp_example.h>

#include <tcp_example_parameters.h>
#include <tcp_example_client.h>
#include <tcp_example_server.h>

#include <clic3.h>

#include <stdio.h>

int main(
  int length_parameters, 
  char** parameters
) {
  struct tcp_example_parameters tcp_example_parameters;
  tcp_example_parameters.mode = server;

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
          tcp_example_parameters.mode = client;
          break;
        } else if (index_client_server == 1) {
          tcp_example_parameters.mode = server;
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

  switch (tcp_example_parameters.mode) {
    case client:
      return tcp_example_client();
      break;
    case server:
      return tcp_example_server();
      break;
    default:
      fprintf(
        stderr,
        "unknown_mode: %i\n",
        tcp_example_parameters.mode
      );
      
      return 1;
  }

  return 0;
}
