#include <stdio.h>
#include <stdlib.h>
#include "io_module.h"
#include "queues_manager.h"
#include "main.h"
#include <string.h>


int process_packet_info(packet_info* pkt) {
	char flow_id[50];
	flow_node* working_flow;
	packet_node* working_packet;

	memset(flow_id, 0, 50);
	sprintf(flow_id, "%s%s%s%s", pkt->Sadd, pkt->Sport, pkt->Dadd, pkt->Dport);
	working_flow = search_flow(flow_id);
	if (working_flow == NULL) {
		working_flow = add_flow(flow_id, pkt->weight);
	}

	working_packet = search_packet_node_at_flow(working_flow, atol(pkt->pktID));
	if (working_packet == NULL) {
		working_packet = add_packet_to_flow(working_flow, atol(pkt->pktID), atoi(pkt->length));
	}
	return 1;
}



int check_RR(flow_node* flow, int flow_sent_count) {
	if (flow_sent_count < flow->weight) {
		return 1;
	}
	else {
		return 0;
	}
}


int send_single_packet(flow_node* flow) {
	fprintf(outfile, "%d: %d\n", total_time, flow->packets_list->packet_id);
	total_time += flow->packets_list->packet_length; //error here
	delete_packet_node_from_flow(flow, flow->packets_list);
	return 1;
}


int goto_closest_time() {
	if (over_pkt == NULL) {
		over_pkt = parse_line();
	}
	if (over_pkt == NULL) {
		return -1;
	}
	total_time = atoi(over_pkt->Time);
	return 1;
}


//WRR main function
int weighted_round_robin() {
	flow_node* curr_flow = NULL; //points to the current working flow
	int num_received;
	int flow_sent_count = 0; //counter so we will not send more than Wf consecutive packets from the current flow
	int send_packet; //flag
	int first_time = 1; //flag
	int total_packets = 0; //indicates the number of all the packets in all of the flows for the current time
	int total_sent = 0;

	while (1) {
		//receiving packets according to time
		num_received = receive_until_time();
		if (num_received == 0 && total_packets == 0) {
			if (goto_closest_time() == -1) {
				printf("reached end of input file and sent all packets, exiting\n");
				break;
			}
			num_received = receive_until_time();
		}
		total_packets += num_received;

		//assuming we now have packets and flows in the data structure
		if (first_time) {
			curr_flow = flows.flows_list;
			first_time = 0;
		}

		//finding a none-empty flow to send from
		while (curr_flow->packets_list == NULL) {
			curr_flow = curr_flow->next;
			flow_sent_count = 0;
			if (curr_flow == NULL) {
				curr_flow = flows.flows_list;
			}
		}

		//sending a single packet from flow, if allowed
		send_packet = check_RR(curr_flow, flow_sent_count);
		if (send_packet) { //we are allowed to send a packet from the current flow
			send_single_packet(curr_flow);
			flow_sent_count += 1;
			total_packets -= 1;
			total_sent += 1;
		}
		else { //we cant go over the weight limit, so we go to the next flow
			curr_flow = curr_flow->next;
			flow_sent_count = 0;
		}
	}
}



