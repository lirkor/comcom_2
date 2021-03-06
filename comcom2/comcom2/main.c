#include <stdio.h>
#include "main.h"
#include "io_module.h"
#include "queues_manager.h"
#include "scheduler.h"
#include <string.h>

//initialization function, opens the input and output files for reading and writing
//uses the command line arguments to give initial values for the weight an quantum variables
int initialize(char* infile_path, char* outfile_path, char* _default_weight, char* _quantum) {
	if ((infile = fopen(infile_path, "r")) == NULL) {
		printf("error opening infile from path %s", infile_path);
		return -1;
	}
	if ((outfile = fopen(outfile_path, "w")) == NULL) {
		printf("error opening outfile from path %s", outfile_path);
		return -1;
	}
	total_time = 0;
	memset(default_weight, 0, 20);
	strcpy(default_weight, _default_weight);
	quantum = atoi(_quantum);

	return 1;
}

//finalization function
//closes the files that were opened for reading and writing
int finalize() {
	if (infile != NULL) {
		fclose(infile);
	}
	if (outfile != NULL) {
		fclose(outfile);
	}
	return 1;
}



int main(int argc, char* argv[]) {
	//usage check
	if (argc != 6) {
		printf("Number of arguments is invalid\nExiting...\n");
		exit(1);
	}

	initialize(argv[2], argv[3], argv[4], argv[5]);
	
	//choosing scheduling type according to command line argument
	if (strcmp(argv[1], "RR") == 0) {
		weighted_round_robin();
	}
	else if (strcmp(argv[1], "DRR") == 0) {
		deficit_round_robin();
	}
	else {
		printf("Unknown scheduler\nExiting..\n");
		exit(1);
	}

	finalize();

	return 1;
}