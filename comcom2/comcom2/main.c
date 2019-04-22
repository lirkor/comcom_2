#include <stdio.h>
#include "main.h"
#include "io_module.h"
#define _CRT_SECURE_NO_WARNINGS

int initialize(char* infile_path, char* outfile_path) {
	if ((infile = fopen(infile_path, "r")) == NULL) {
		printf("error opening infile from path %s", infile_path);
		return -1;
	}
	if ((outfile = fopen(outfile_path, "w")) == NULL) {
		printf("error opening outfile from path %s", outfile_path);
		return -1;
	}

	
}

int finalize() {
	if (infile != NULL) {
		fclose(infile);
	}
	if (outfile != NULL) {
		fclose(outfile);
	}
	return 1;
}



int main() {
	char* in_path = "inp1.txt";
	char* out_path = "out.txt";
	initialize(in_path, out_path);
	tests();
	return 1;
}