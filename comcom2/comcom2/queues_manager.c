#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queues_manager.h"
//#define _CRT_SECURE_NO_WARNINGS



flows_control_sentinel flows;





/* 
 * gets a flow_id and search for the specific flow.
 * returns the pointer to flow_node if found the node,
 * else return NULL 
 */ 

flow_node* search_flow(char* flow_id) {
	flow_node* examind_node;

	examind_node = flows.flows_list;

	while (examind_node != NULL) {

		if (strcmp(examind_node->flow_identifier, flow_id) == 0) {
			return examind_node;
		}

		examind_node = examind_node->next;
	}

	return NULL;
}




int add_flow(char* flow_id) {
	flow_node* new_flow_node;
	

	new_flow_node = (flow_node*)malloc(sizeof(flow_node));
	new_flow_node->credit = 0;
	strcpy(new_flow_node->flow_identifier, flow_id);
	new_flow_node->packets_list = NULL;
	new_flow_node->next = NULL;


	/* First node to add */
	if (flows.flows_list == NULL) {
		new_flow_node->prev = NULL;
		flows.flows_list = new_flow_node;
		flows.last_node = new_flow_node;
	}
	else {
		/* else - flows list is not empty, update the last one to point to the new flow,
		   and the new flow node to point backwards to the last one */

		new_flow_node->prev = flows.last_node;
		flows.last_node->next = new_flow_node;
		
	}

	return 0;
}




/* Search for a packet in a specific flow.
 * @return: The found packet on packed has found, NULL otherwise 
 */

packet_node* search_packet_node_at_flow(flow_node* flow, long packet_id) {
	packet_node* examined_packet;

	examined_packet = flow->packets_list;

	while (examined_packet != NULL) {
		if (examined_packet->packet_id == packet_id) {
			return examined_packet;
		}

		examined_packet = examined_packet->next;
	}

	return NULL;

}


int add_packet_to_flow(flow_node* flow, long packet_id) {
	packet_node* new_packet_node;

	new_packet_node = (packet_node*)malloc(sizeof(packet_node));

	new_packet_node->next = NULL;
	new_packet_node->packet_id = packet_id;

	if (flow->packets_list == NULL) {
		flow->packets_list = new_packet_node;
		new_packet_node->prev = NULL;
		flow->last_packet_node = new_packet_node;
	}
	else {
		new_packet_node->prev = flow->last_packet_node;
		flow->last_packet_node = new_packet_node;
	}

	return 0;

}


int delete_packet_node_from_flow(flow_node* flow, packet_node* packet_to_delete) {

	/* The flow contains single packet */
	if (packet_to_delete->next == NULL && packet_to_delete->prev == NULL) {
		if (flow->packets_list != packet_to_delete) {
			printf("DEBUG: SOMETHING WRONG WITH THE PACKET LIST(1)\n");
			return 1;
		}
		flow->packets_list = NULL;
		flow->last_packet_node = NULL;
		free(packet_to_delete);
	}
	else if (packet_to_delete->next == NULL && packet_to_delete->prev != NULL) {
		if (flow->last_packet_node != packet_to_delete) {
			printf("DEBUG: SOMETHING WRONG WITH THE PACKET LIST(2)\n");
			return 1;
		}

		packet_to_delete->prev->next = NULL;
		flow->last_packet_node = packet_to_delete->prev;
		free(packet_to_delete);
	}
	else if (packet_to_delete->prev == NULL && packet_to_delete->next != NULL) {
		if (flow->packets_list != packet_to_delete) {
			printf("DEBUG: SOMETHING WRONG WITH THE PACKET LIST(3)\n");
		}
		flow->packets_list = packet_to_delete->next;
		packet_to_delete->next->prev = NULL;
		free(packet_to_delete);
	}
	else if (packet_to_delete->next != NULL && packet_to_delete->prev != NULL) {
		packet_to_delete->next->prev = packet_to_delete->prev;
		packet_to_delete->prev->next = packet_to_delete->next;
		free(packet_to_delete);
	}
	else {
		printf("DEBUG: SOMETHING WRONG WITH THE PACKET LIST(4)\n");
	}

	return 0;

}


		