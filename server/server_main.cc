#include <stdio.h>
#include <cstdlib> 
#include <stdint.h>
#include <string>
#include <type_traits>
#include <assert.h>
#include <chrono>
#include <ctime>
#include <inttypes.h>
#include <vector>
#include "application.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
		printf("Usage:\n\t server port_number\n");
		printf("Please provide port to listen on\n");
		return 0;
    }

    int port = atoi(argv[1]);
	printf("Running FizzBuzz server on %d port...\n", port);

	Demo::Application app;
	app.SimulationRun();

	return 0;
}
