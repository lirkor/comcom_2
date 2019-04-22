#include <stdio.h>
#ifndef _IO_MODULE_
#define _IO_MODULE_

//globals

typedef struct packet_info {
char pktID[20];
char Time[100];
char Sadd[20];
char Sport[20];
char Dadd[20];
char Dport[20];
char length[20];
char weight[20];
}packet_info;

//function declarations
void print_pkt_info(packet_info* pkt);
int get_line(char* line);
packet_info* parse_line();
void tests();

#endif