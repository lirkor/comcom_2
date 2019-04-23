//#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "queues_manager.h"s
#include "main.h"
#include "io_module.h"

//globals
packet_info* over_pkt = NULL; //when we over read, we save the overread packet for next time change


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
	if (res == NULL) {
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

int receive_until_time() {
	packet_info* pkt;
	int num_received = 0;
	int pkt_time;
	if (over_pkt != NULL) {
		pkt_time = atoi(over_pkt->Time);
		if (pkt_time > total_time) {
			return num_received;
		}
		num_received += 1;
		//process_packet_info(over_pkt);
		over_pkt = NULL;
	}

	while ((pkt = parse_line()) != NULL) {
		print_pkt_info(pkt); //debug
		pkt_time = atoi(pkt->Time);
		if (pkt_time > total_time) { //if we overread
			over_pkt = pkt; //saving overread packet for next time change
			break;
		}
		num_received += 1;
		//process_packet_info(pkt);
	}
	return num_received;
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
	return pkt;
}

void tests() {
	//testing receive_until_time
	if (1) {
		int num_received;
		while (total_time < 120) {
			num_received = receive_until_time();
			printf("total_time is currently - %d, received %d lines\n", total_time, num_received);
			total_time += 1;
		}
	}
	printf("done tests\n");
}