#ifndef _QUEUES_MANAGER_H_
#define _QUEUES_MANAGER_H_
#include <stdlib.h>


typedef struct packet_node {

	long packet_id;
	struct packet_node* next;
	struct packet_node* prev;

}packet_node;


typedef struct flow_node {

	char*		 flow_identifier;
	packet_node* packets_list;
	packet_node* last_packet_node;
	int			 credit;
	struct flow_node*	 next;
	struct flow_node*	 prev;


} flow_node;



typedef struct flows_control_sentinel {

	flow_node*	 flows_list;
	flow_node*	 last_node;

} flows_control_sentinel;


extern flows_control_sentinel flows;


flow_node* search_flow(char* flow_id);
int add_flow(char* flow_id);
packet_node* search_packet_node_at_flow(flow_node* flow, long packet_id);
int add_packet_to_flow(flow_node* flow, long packet_id);
int delete_packet_node_from_flow(flow_node* flow, packet_node* packet_to_delete);



#endif
