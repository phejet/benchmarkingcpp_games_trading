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
		printf("Usage:\n\t fizzbuzz_server simulation file\n");
		printf("Provive simulation data filename as a first parameter\n");
		return 0;
    }

    const char* simulationFile = argv[1];
	
	Demo::Application app;
	app.SimulationRun(simulationFile);

	return 0;
}
