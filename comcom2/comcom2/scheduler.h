
#include "io_module.h"
#include "queues_manager.h"


#ifndef _SCHEDULER_
#define _SCHEDULER_
//globals

//function declarations
int process_packet_info(packet_info* pkt);
int check_RR(flow_node* flow, int flow_sent_count);
int send_single_packet(flow_node* flow);
int goto_closest_time();
int weighted_round_robin();

#endif