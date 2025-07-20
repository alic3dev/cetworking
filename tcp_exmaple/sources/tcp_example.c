#include "tcp_example.h"

#include <stdio.h>

#include "clic3.h"

#include "tcp_example_parameters.h"
#include "tcp_example_client.h"
#include "tcp_example_server.h"

int main(
  int arguments_length, 
  char** arguments
) {
  struct tcp_example_parameters tcp_example_parameters;
  tcp_example_parameters.mode = server;

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
          tcp_example_parameters.mode = client;
          break;
        } else if (index_client_server == 1) {
          tcp_example_parameters.mode = server;
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

