#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "queues_manager.h"s
#include "main.h"
#include "io_module.h"


void print_pkt_info(packet_info* pkt) {
	printf("packet ID - %s	", pkt->pktID);
	printf("packet Time - %s	", pkt->Time);
	printf("packet Sadd - %s	", pkt->Sadd);
	printf("packet Sport - %s	", pkt->Sport);
	printf("packet Dadd - %s	", pkt->Dadd);
	printf("packet Dport - %s	", pkt->Dport);
	printf("packet length - %s	", pkt->length);
	if (strlen(pkt->weight) != 0) {
		printf("packet weight - %s", pkt->weight);
	}
	printf("\n");
}

int get_line(char* line) {
	char* res = NULL;
	res = fgets(line, 128,infile);
	if (res == NULL || res == EOF) {
		printf("done reading\n");
		return 0;
	}
	printf("line received = %s", line); //debug
	return 1;
}

packet_info* new_packet() {
	packet_info* pkt = (packet_info*)malloc(sizeof(packet_info));
	memset(pkt->weight, 0, sizeof(pkt->weight));
	return pkt;
}


packet_info* parse_line() {
	char line[128];
	char* weight;
	if (get_line(line) == 0) {
		printf("done parsing\n");
		return NULL;
	}
	packet_info* pkt = new_packet();
	strcpy(pkt->pktID,strtok(line, " "));
	strcpy(pkt->Time,strtok(NULL, " "));
	strcpy(pkt->Sadd,strtok(NULL, " "));
	strcpy(pkt->Sport,strtok(NULL, " "));
	strcpy(pkt->Dadd,strtok(NULL, " "));
	strcpy(pkt->Dport,strtok(NULL, " "));
	strcpy(pkt->length,strtok(NULL, " "));
	if ((weight = strtok(NULL, " ")) != NULL) {
		strcpy(pkt->weight,weight);
	}
	print_pkt_info(pkt);
	return pkt;
}

void tests() {
	packet_info* pkt;
	while ((pkt = parse_line()) != NULL) {
		print_pkt_info(pkt);
	}
	printf("done tests\n");
}