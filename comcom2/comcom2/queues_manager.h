#ifndef _QUEUES_MANAGER_H_
#define _QUEUES_MANAGER_H_
#include <stdlib.h>


/* A single node holding the packet information */
typedef struct packet_node {

	long packet_id;
	int packet_length;
	struct packet_node* next;
	struct packet_node* prev;

}packet_node;

/* A single node holding the flow information. The flow's packets are saved in linked list pointed
 * to by 'packet_list' field.
 */
typedef struct flow_node {

	char		 flow_identifier[50];
	packet_node* packets_list;
	packet_node* last_packet_node;
	int			 credit;
	int			 weight;
	struct flow_node*	 next;
	struct flow_node*	 prev;


} flow_node;


/* The sentinel of starting the flows linked list */
typedef struct flows_control_sentinel {

	flow_node*	 flows_list;
	flow_node*	 last_node;

} flows_control_sentinel;


extern flows_control_sentinel flows;


flow_node* search_flow(char* flow_id);
flow_node* add_flow(char* flow_id, char* flow_weight);
packet_node* search_packet_node_at_flow(flow_node* flow, long packet_id);
packet_node* add_packet_to_flow(flow_node* flow, long packet_id, int length);
int delete_packet_node_from_flow(flow_node* flow, packet_node* packet_to_delete);



#endif
