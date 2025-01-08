#include "algorithm.h"
// queue - defines a queue as a pointer to an array of pointers to nodes
typedef pnode *queue;
// local prototypes
void init_single_source(pnode G, char s);
void relax(pnode u, pnode v, int w);
int Q_insert(queue Q, pnode u, int index);
int Q_is_empty(queue Q, int size);
pnode Q_extract_min(queue Q, int size);
bool Q_exists(queue Q, int qsize, char name);
int name_to_pos(pnode G, char c);
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES]);
//--------------------------------------------------------------------------
// init single source, relax and min are to be used by algorithms
//--------------------------------------------------------------------------
void init_single_source(pnode G, char s)
{
	for (pnode node = G; !is_empty(node); node = get_next(node)) {
		if (get_name(node) == s) // start node
			set_d(node, 0);
		else
			set_d(node, INFINITY);
		set_pi(node, '-');
	}
}
void relax(pnode u, pnode v, int w)
{
	if (get_d(v) > (get_d(u) + w)) {
		set_d(v, get_d(u) + w);
		set_pi(v, get_name(u));
	}
}
double min(double a, double b)
{
	return a < b ? a : b;
}
//--------------------------------------------------------------------------
// Q functions - used by other algorithms
//--------------------------------------------------------------------------
int Q_insert(queue Q, pnode u, int index)
{
	Q[index] = u;
	index++;
	return index;
}
int Q_is_empty(queue Q, int size)
{
	if(!Q){return -1;}
	return (size == 0) ? 1 : 0;
}
pnode Q_extract_min(queue Q, int size)
{
	if(size == 0) {return NULL;}

	int index = 0;
	pnode smol_node = Q[0]; //set first node to smallest
	
	for (int i = 0; i < size; i++){
		if(get_d(Q[i]) < get_d(smol_node)){
			index = i;
			smol_node = Q[i];
		}
	}
	for(int i = index; i < size - 1; i++){
		Q[i] = Q[i + 1];
	}
	Q[size - 1] = NULL;
	return smol_node;
}
bool Q_exists(queue Q, int qsize, char name)
{
	if(!Q){return false;}
	for (int i = 0; i < qsize; i++){
		if(Q[i] && get_name(Q[i]) == name){
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------
// Dijkstra's algorithm, single source shortest path tree generator
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [  0,   1,   3]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void dijkstra(pnode G, char s, double *d, char *e)
{
	init_single_source(G, s); // initialize start node
	queue Q = (queue)malloc(MAXNODES * sizeof(pnode)); //malloc our queueu
	int size = 0;

	pnode node = G;
	while(node != NULL){
		size = Q_insert(Q, node, size); //fill queue with all nodes from G
		node = get_next(node);
	}
	
	while (!Q_is_empty(Q, size)){
		pnode u = Q_extract_min(Q, size); //extract the smallest weight edge from the startnode to find out where it goes
		size--;

		for(pedge edge = get_edges(u); edge != NULL; edge = edge->next_edge) {
			
			pnode v = get_node(G, edge->to); //get destination node v from curr edge  
			relax(u, v, edge->weight);      //relax u and v 
		}
	}
	int index = 0;
	node = G;
	while(!is_empty(node)){
		//go through all nodes to update the best routes
		d[index] = get_d(node);
		e[index] = get_pi(node);
		index++;
		node = get_next(node);
	}
	free(Q); //FREEEEDOM
}
//--------------------------------------------------------------------------
// Prim's algorithm - Minimum Spanning Tree generator
// start_node: a
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [inf,   1,   2]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void prim(pnode G, char start_node, double *d, char *e)
{
	if(!G) return;
	
	pnode node = G; 
	int size = 0;
	queue Q = (queue)malloc(MAXNODES * sizeof(pnode)); //malloc our queue
	
	while(node != NULL) {
		//make an empty node to keep track of distances and predecessors
		set_d(node, INFINITY);
		set_pi(node, '-');
		size = Q_insert(Q, node, size);
		node = get_next(node);
	}
	
	//initialize start node 
	pnode starter = get_node(G,start_node);
	if (starter) {
        set_d(starter, 0);
    }
	
    // Main Prim's algorithm loop
    while (!Q_is_empty(Q, size)) {
        pnode u = Q_extract_min(Q, size);
        if (!u) break;
        size--;
        
        // Check all nodes to find connected edges
        pnode current = G;
        while (current != NULL) {
            pedge edge = get_edges(current);
            while (edge != NULL) {
                // Check if this edge connects to our current node u
                if (edge->to == get_name(u) || get_name(current) == get_name(u)) {
                    pnode v;
					if (edge->to == get_name(u)) {
					    v = current;
					} else {
					    v = get_node(G, edge->to);
					}
                    
                    if (v && Q_exists(Q, size, get_name(v))) {
                        double weight = edge->weight;
                        if (weight < get_d(v)) {
	                        set_d(v, weight);
                            set_pi(v, get_name(u));
                        }
                    }
                }
                edge = edge->next_edge;
            }
            current = get_next(current);
        }
    }
	
	int index = 0;
	node = G;
	while (node != NULL) {
		if(get_d(node) == 0){
			set_d(node, INFINITY);
		}
		d[index] = get_d(node);
		e[index] = get_pi(node);
		index++;
		node = get_next(node);
	}
	free(Q);
}
//--------------------------------------------------------------------------
// Floyd's algorithm: returns matrix of distances
// a -> b(1)
// |                   |0  |1  |3  |
// b -> c(2)    -->    |INF|0  |2  |
// |                   |INF|INF|0  |
// c
//--------------------------------------------------------------------------
void floyd(pnode G, double W[MAXNODES][MAXNODES])
{
	int n = 0;

	list_to_matrix(G,W);
	pnode node = G;
	while(!is_empty(node)){
		n++;
		node = get_next(node);
	}

	for (int i = 0; i < n; i++){
		W[i][i] = 0;
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			for (int k = 0; k < n; k++){
				if(W[j][i] < INFINITY && W[i][k] < INFINITY) {
					W[j][k] = min(W[j][k], W[j][i] + W[i][k]);
				} 
			}
		}
	}
}
//--------------------------------------------------------------------------
// Warshall's algorithm: returns matrix of closures, i.e. if paths exists
// a -> b(1)
// |                   |T  |T  |T  |
// b -> c(2)    -->    |F  |T  |T  |
// |                   |F  |F  |T  |
// c
//--------------------------------------------------------------------------
void warshall(pnode G, double W[MAXNODES][MAXNODES])
{
	int n = 0; 
	list_to_matrix(G,W);

	pnode node = G;
	while (!is_empty(node)){
		n++;
		node = get_next(node);
	}
	//handles self loops 
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if(W[i][j] == INFINITY){
				W[i][j] = 0;
			}
			else {
				W[i][j] = 1;
			}
		}
	}
	//handles diagonal
	for(int i = 0; i < n; i++){
		W[i][i] = 1;
	} 
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			for (int k = 0; k < n; k++){
				W[j][k] = W[j][k] || (W[j][i] && W[i][k]);
			}
		}
	}
}