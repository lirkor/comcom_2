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
		working_flow = add_flow(flow_id);
	}

	working_packet = search_packet_node_at_flow(working_flow, pkt->pktID);
	if (working_packet == NULL) {
		working_packet = add_packet_to_flow(working_flow, pkt->pktID);
	}

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
	total_time += flow->packets_list->packet_length;
	delete_packet_node_from_flow(flow, flow->packets_list);
}



int weighted_round_robin() {
	flow_node* curr_flow;
	int num_received;
	int flow_sent_count = 0;
	int send_packet;
	int first_time = 1;

	


	while(1) {
		num_received = receive_until_time();
		if (num_received == 0) {
			goto_closest_time();
			receive_until_time();
		}

		if (first_time) {
			curr_flow = flows.flows_list;
			first_time = 0;
		}

		while (curr_flow->packets_list == NULL) {
			curr_flow = curr_flow->next;
			flow_sent_count = 0;
			if (curr_flow == NULL) {
				curr_flow = flows.flows_list;
			}
		}

		send_packet = check_RR(curr_flow, flow_sent_count);
		if (send_packet) {
			send_single_packet(curr_flow);
			flow_sent_count += 1;
		}







}




