#include "adjlist.h"
#include <string.h>
// local prototypes
pnode create_node(char nname);
pnode node_cons(pnode first, pnode second);
pedge create_edge(char to, double weight);
pedge edge_cons(pedge first, pedge second);
pedge _add_edge(pedge edge, char to, double weight);
pedge _rem_edge(pedge edges, char to);
void remove_all_edges_to(pnode G, char name);
void remove_all_edges_from(pnode G, char name);
//private helper functions
static void print_graph_now(pnode G);

// create_node: creates node with name nname
pnode create_node(char nname)
{
	pnode new_node = (pnode)calloc(1, sizeof(node));
	new_node->name = nname;
	new_node->d = INFINITY;
	new_node->pi = '-';
	new_node->next_node = NULL;
	new_node->edges = NULL;
	return new_node;
}
// function implementations
bool is_empty(pnode G)
{
	return !G;
}
char get_name(pnode G)
{
	return is_empty(G) ? '-' : G->name;
}
pnode set_name(pnode G, char name)
{
	if (!is_empty(G))
		G->name = name;
	return G;
}
pedge get_edges(pnode G)
{
	return is_empty(G) ? NULL : G->edges;
}
pnode set_edges(pnode G, pedge E)
{
	if (!is_empty(G))
		G->edges = E;
	return G;
}
pnode get_next(pnode G)
{
	return is_empty(G) ? NULL : G->next_node;
}
// see node_cons() for the corresponding "set-next" function
double get_d(pnode G)
{
	return is_empty(G) ? INFINITY : G->d;
}
pnode set_d(pnode G, double d)
{
	if (!is_empty(G))
		G->d = d;
	return G;
}
char get_pi(pnode G)
{
	return is_empty(G) ? '-' : G->pi;
}
pnode set_pi(pnode G, char pi)
{
	if (!is_empty(G))
		G->pi = pi;
	return G;
}
// node_cons: connects two nodes in adjacency list
pnode node_cons(pnode first, pnode second)
{
	if (is_empty(first))
		return NULL;
	first->next_node = second;
	return first;
}
// add_node: adds a new node with name nname to adjacency list G
//           in lexicographical order. If it already exists
//           in graph, nothing is done
pnode add_node(pnode G, char nname) {
	//creates the node if node with the same name does not exist.
	if(find_node(G, nname)) {
		return G;
	}
	
	pnode new_node = create_node(nname); 
	if(G == NULL){
		//print_graph_now(G);
		return new_node;
	}
	if(get_name(new_node) < get_name(G)) {
		new_node->next_node = G;
		//print_graph_now(G);
		return new_node;
	}
	pnode current = G;
	while (current->next_node != NULL && get_name(current->next_node) < get_name(new_node)) {
		current = current->next_node;
		//print_graph_now(G);
	}
	new_node->next_node = get_next(current);
	current->next_node = new_node;
	//print_graph_now(G);
	return G;
}
// rem_node: removes node with name name from adjacency list G
//           if node does not exist, nothing happens

pnode rem_node(pnode G, char name) {
	if (!G) return NULL;  // Empty graph

	pnode current = G;
	pnode prev = NULL;

	// Traverse the node list
	while (current) {
		if (get_name(current) == name) {
			remove_all_edges_to(G, name);
			remove_all_edges_from(G, name);

			if (prev == NULL) { //if first node in list
				G = get_next(current);
			} 
			else {
				//update the prev node to skip curren node
				prev->next_node = get_next(current);
			}
			//free the node and its edges
			free(current);
			return G;
		}
		prev = current;
		current = get_next(current);
	}
	return G;  //for when node not found
}
// get_node: returns pointer to node with name name from adjacency list G
pnode get_node(pnode G, char name)
{
	if(find_node(G, name)){
		while (G != NULL){
			if(get_name(G) == name) {
				return G;
			}
			G = get_next(G);
		}
	}
	//printf("Node not in list.\n");
	return NULL;
}
// get_node: returns true if node with name name exists in adjacency list G
//           false otherwise
bool find_node(pnode G, char name)
{ 
	return	is_empty(G)	        ? false  :
			get_name(G) == name ? true   :
			find_node(G->next_node, name);
}
// create_edge: creates edge
pedge create_edge(char to, double weight)
{
	pedge new_edge = (pedge)calloc(1, sizeof(edge));
	new_edge->to = to;
	new_edge->weight = weight;
	return new_edge;
}
bool edge_empty(pedge E)
{
	return !E;
}
char get_to(pedge E)
{
	return edge_empty(E) ? '-' : E->to;
}
pedge set_to(pedge E, char to)
{
	if (!edge_empty(E))
		E->to = to;
	return E;
}
double get_weight(pedge E)
{
	return edge_empty(E) ? INFINITY : E->weight;
}
pedge set_weight(pedge E, double weight)
{
	if (!edge_empty(E))
		E->weight = weight;
	return E;
}
pedge get_next_edge(pedge E)
{
	return edge_empty(E) ? NULL : E->next_edge;
}
// edge_cons: connects two edges in edge list
pedge edge_cons(pedge first, pedge second)
{
	if (edge_empty(first))
		return NULL;
	first->next_edge = second;
	return first;
}
// upd_edge: updates edge E to new weight
pedge upd_edge(pedge E, double weight)
{
	if(E != NULL){
		set_weight(E,weight);
	}
	return E;
}
// _add_edge: creates and connects new edge to edge-list
pedge _add_edge(pedge E, char to, double weight)
{
	E = create_edge(to, weight);
	return E;
}
// add_edge: adds an edge to G by finding correct start node
//           and then calling _add_edge to create new edge
void add_edge(pnode G, char from, char to, double weight)
{
	//Traverse the nodes until we are on the correct from-node
	while(G && get_name(G) != from){
		G = get_next(G);
		if(G == NULL) return; //preventing infinite loops
	}
	pedge current = get_edges(G);
	pedge prev = NULL;

	while(current && get_to(current) < to){
		prev = current;
		current = get_next_edge(current);
	}
	if(current && get_to(current) == to){
		upd_edge(current, weight);
		return;
	}
	pedge new_edge = create_edge(to, weight);
	//putting new edge at the start
	if(prev == NULL){
		new_edge->next_edge = get_edges(G);
		set_edges(G, new_edge);
	}
	//putting new edge in the mid or end
	else{
		new_edge->next_edge = current;
		prev->next_edge = new_edge;
	}
}
// _find_edge: finds edge in edge-list
bool _find_edge(pedge E, char to)
{
	// TODO
	return false;
}
// find_edge: returns true if edge between from and to exists, false otherwise
bool find_edge(pnode G, char from, char to) {
	if (G == NULL) {
		return false; // Return false if the graph is NULL
	}
	while (G != NULL && get_name(G) != from) {
		G = get_next(G);
	}
	if (G == NULL) {
		return false;
	}
	pedge edgeList = G->edges;
	while (edgeList != NULL) {
		if (edgeList->to == to) {
			return true;
		}
		edgeList = get_next_edge(edgeList); 
	}
	return false; //edge not found
}

// _edge_cardinality: returns the number of edges from one node
int _edge_cardinality(pedge E)
{
	// TODO
	return 0;
}
// edge_cardinality: returns the total number of edges in G
int edge_cardinality(pnode G)
{
	int counter = 0;
	while(G != NULL) {
		pedge E = get_edges(G); //Get the first edge in the nodes edge-list
		while(E != NULL){ //Traverse the edge-list
			counter++;
			E = get_next_edge(E);
		}
		G = get_next(G);
	}
	return counter;
}
// _self_loops: returns the number of edges going back to
//              source node
int _self_loops(pedge E, char src)
{
	int counter = 0;

	while (E != NULL){
		if(get_to(E) == src){
			counter++;
		}
		E = get_next_edge(E);
	}
	return counter;
}
// self_loops: counts the number of self-loops, i.e. edges to and from
//             the same node
int self_loops(pnode G)
{
	int loops = 0;
	char source;

	while(G != NULL){
		source = get_name(G);
		loops += _self_loops(get_edges(G), source);
		G = get_next(G);
	}
	return loops;
}
// _rem_edge: removes edge from edge-list (Currently not used)
pedge _rem_edge(pedge E, char to)
{
	E->next_edge = get_next_edge(get_next_edge(E));
	return E;
}
// rem_edge: removes edge from G
void rem_edge(pnode G, char from, char to) {
	
    while (G && get_name(G) != from) {
        G = get_next(G);
    }
    if (!G) {
        return;
    }
    // Get the first edge from the node
    pedge E = get_edges(G);
    pedge prevE = NULL;

    if (!E) {
        return;
    }

    // Traverse the edge list to find the edge to remove
    while (E) {
        if (get_to(E) == to) {
            if (prevE == NULL) {
                set_edges(G, get_next_edge(E)); // Update the head of the edge list
            }
			else {
                prevE->next_edge = get_next_edge(E); // Skip the current edge
            }
            free(E); // Free the edge memory
            return;
        }
        prevE = E;
        E = get_next_edge(E);
    }
}
// remove_all_edges_to: removes all edges going towards node with name name
void remove_all_edges_to(pnode G, char name)
{
    while (G) {
		pedge current = get_edges(G);
        pedge prev = NULL;
		
		while (current) {
			if (get_to(current) == name) {
				if (prev == NULL) {
					set_edges(G, get_next_edge(current));
				}
				else {
					prev->next_edge = get_next_edge(current);
                }
				free(current);
				break;
            }
			prev = current;
			current = get_next_edge(current);
		}
		G = get_next(G);
	}
}

// remove_all_edges_from: removes all edges from node with name name
void remove_all_edges_from(pnode G, char name)
{
	pnode node = get_node(G, name);
	if (!node) return;


	pedge current = get_edges(node);

	while (current) {
		pedge next = get_next_edge(current);
		free(current);
		current = next;
	}
}
// node_cardinality: returns the number of nodes in G
int node_cardinality(pnode G)
{
	int counter = 0;
	while(G != NULL) {
		counter++;
		G = get_next(G);
		//printf("count is %d \n",counter);
	}
	//printf("Final count is %d \n",counter);
	return counter;
}
// name_to_pos: returns position of node with name c, -1 if not found
int name_to_pos(pnode G, char c)
{
	int index = 0;
	while (G != NULL){
		if (get_name(G) == c){
			return index;  //returning pos of node
		}
		G = get_next(G);
		index++;
	}
	//if node not found
	return -1;
}
// pos_to_name: returns name of node at position pos in G
char pos_to_name(pnode G, int pos)
{
	for(int i = 0; i < pos; i++){
		G = get_next(G);
	}
	return get_name(G);
}
// list_to_pos: creates adjacency matrix from adjacency list
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES])
{
	for (int i = 0; i < MAXNODES; i++){
		for (int j = 0; j < MAXNODES; j++){
			matrix[i][j] = INFINITY;
		}
	}
	int row = 0;
	pnode current = G;

	while(current != NULL){
		pedge edge_list = get_edges(current);
		while (edge_list != NULL){
			char to = get_to(edge_list);
			double weight = get_weight(edge_list);
			int col = name_to_pos(G, to);
			if(col != -1) {
				matrix[row][col] = weight;
			}
			edge_list = get_next_edge(edge_list);
		}
		current = get_next(current);
		row++;
	}
}
//helper for printing the nodes, almost working
static void print_graph_now(pnode G){
	while (G != NULL){
		printf("%c -> ", get_name(G));
		G = G->next_node;
	}
}