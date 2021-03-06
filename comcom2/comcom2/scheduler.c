#include <stdio.h>
#include <stdlib.h>
#include "io_module.h"
#include "queues_manager.h"
#include "main.h"
#include <string.h>


/* Process the packet info from the parsed line into a struct holding the fields of the packet.
 * The flow id consist of concatinating the ip and port to each other, resulting a uniqe string for
 * for each flow.
 */
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


/* Check if a packet (the first one in the queue) can be sent by the rules of weighted round robin */
int check_RR(flow_node* flow, int flow_sent_count) {
	if (flow_sent_count < flow->weight) {
		return 1;
	}
	else {
		return 0;
	}
}

/* The method of sending a single packet, The actual send is writing to file the time stamp and
 * the packet id. After sending the packet the time is updated and the packet is deleted from the flow.
 */
int send_single_packet(flow_node* flow) {
	fprintf(outfile, "%d: %d\n", total_time, flow->packets_list->packet_id);
	total_time += flow->packets_list->packet_length;
	delete_packet_node_from_flow(flow, flow->packets_list);
	return 1;
}


/* Go to the closest time containd a packet.
 * The function is called only when no packets left to send and all the flows are empty.
 */
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

/* Round robin function - go to the closest flow which has a packet to send. */
void goto_next_flow(flow_node** curr_flow_ptr, int* sent_count_ptr) {
	*curr_flow_ptr = (*curr_flow_ptr)->next;
	*sent_count_ptr = 0;
	if (*curr_flow_ptr == NULL) {
		*curr_flow_ptr = flows.flows_list;
	}
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
				//printf("reached end of input file and sent all packets, exiting\n");
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
			goto_next_flow(&curr_flow, &flow_sent_count);
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
			goto_next_flow(&curr_flow, &flow_sent_count);
		}
	}
}


/* Check if a packet can be send from the flow by the deficit-RR rules. */
int check_DRR(flow_node* flow) {
	if (flow->credit >= flow->packets_list->packet_length) {
		return 1;
	}
	else {
		return 0;
	}
}


int update_flow_credit(flow_node* curr_flow) {
	curr_flow->credit += (curr_flow->weight) * quantum;
}


/* DRR function of going to the next flow which contains a packet to send.
 * The looping continues until the initial flow is reached, which means the list is empty, maybe
 * except one flow.
 * For each flow with no packets in it - the credit is reset.
 */
flow_node* goto_next_flow_drr(flow_node* current_node) {
	flow_node* node_ptr;
	node_ptr = current_node->next;

	while (node_ptr != current_node) {
		if (node_ptr == NULL) {
			node_ptr = flows.flows_list;
		}
		if (node_ptr->packets_list != NULL) {
			update_flow_credit(node_ptr);
			return node_ptr;	// return the first found flow with a packet
		}
		else {
			node_ptr->credit = 0;
			node_ptr = node_ptr->next;
		}
	}
	update_flow_credit(current_node);
	if (current_node->packets_list == NULL) {
		current_node->credit = 0;
	}
	return current_node;
}
	

/* The function that imitates the sending of the packet by writing its time stamp to file.
 * it also changes the total time by DRR rules (increasing by the packet sending time), and decreasing the 
 * credit amount from the flow. After it, the packet is being deleted (dequeue). 
 */
int send_single_packet_drr(flow_node* flow) {
	fprintf(outfile, "%d: %d\n", total_time, flow->packets_list->packet_id);
	total_time += flow->packets_list->packet_length;
	flow->credit -= flow->packets_list->packet_length;
	delete_packet_node_from_flow(flow, flow->packets_list);
	return 1;
}





/* The main function of DRR which manages the deficit round robin scheduler */
int deficit_round_robin() {
	flow_node* curr_flow = NULL; //points to the current working flow
	int num_received;
	int flow_sent_count = 0; //counter so we will not send more than Wf consecutive packets from the current flow
	int send_packet; //flag
	int first_time = 1; //flag
	int total_packets = 0; //indicates the number of all the packets in all of the flows for the current time
	int total_sent = 0;
	int counter = 0;

	while (1) {
		
		//counter += 1;
		// receiving packets according to time
		
		num_received = receive_until_time();
		if (num_received == 0 && total_packets == 0) {
			if (goto_closest_time() == -1) {
				//printf("reached end of input file and sent all packets, exiting\n");
				break;
			}
			num_received = receive_until_time();
		}
		total_packets += num_received;

		//assuming we now have packets and flows in the data structure
		if (first_time) {
			curr_flow = flows.flows_list;
			if (curr_flow->packets_list == NULL) {
				curr_flow = goto_next_flow_drr(curr_flow);
			}
			first_time = 0;
		}
		
		// finding a none-empty flow to send from
		if (curr_flow->packets_list == NULL) {
			curr_flow = goto_next_flow_drr(curr_flow);
			
		}


		// sending a single packet from flow, if allowed
		send_packet = check_DRR(curr_flow);
		if (send_packet) { // we are allowed to send a packet from the current flow
			send_single_packet_drr(curr_flow); // reset the credit of the flow
			total_packets -= 1;
			total_sent += 1;
		}
		else {
			curr_flow = goto_next_flow_drr(curr_flow);
		}

		
	}
}

