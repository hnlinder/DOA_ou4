#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"
#include "dlist.h"
#include "array_1d.h"

/*
 * Implementation of a generic graph/1D-array solution for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Otto Nilsson (tfy18onn@cs.umu.se)
 *	   		Johannes Strid (tfy18jsd@cs.umu.se)
 *	  		MÃ¥ns Lundmark (tfy18mlk@cs.umu.se)
 *
 * Version information:
 *   2021-05-18: v1.0, first public version.
 *   2021-03-06: v2.0, second public version.
 */

#define lo1 0
#define lo2 0
#define hi1 10
#define hi2 10

// ===========INTERNAL DATA TYPES============

struct array_1d {
	int low; // Low index limit.
	int high; // High index limit.
	int array_size; // Number of array elements.
	void **values; // Pointer to where the actual values are stored.
	free_function free_func;
};

typedef struct graph
{
        int node_max;
        int n;
        array_1d *nodes;
} graph;

typedef struct node
{
        array_1d *neighbours;
        const char *identifier;
        bool seen_status;
} node;

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
* nodes_are_equal() - Check whether two nodes are equal.
* @n1: Pointer to node 1.
* @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 */
bool nodes_are_equal(const node *n1, const node *n2)
{
      const char *s1 = n1->identifier;
      const char *s2 = n2->identifier;

      return !strcmp(s2, s1);
}

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes) {
		// Allocates memory for graph.
        graph *g = calloc(1, sizeof(graph));
		// Creates an empty array for nodes.
        g->nodes = array_1d_create(0, max_nodes, NULL);
        g->n = 0;
        g->node_max = max_nodes;
        return g;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g) {
        return(!array_1d_has_value(g->nodes, 0));
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g) {
        array_1d *arr = g->nodes;
		// Loop over all rows and cols.
        for(int r = 0; r < g->n; r++) {
				// Inspect nodes
                node *n = array_1d_inspect_value(arr, r);
				// Check neighbours.
                array_1d *neighbours = n->neighbours;
                for(int c = 0; c < g->n; c++) {
                        if(*(int*)array_1d_inspect_value(neighbours, c)) {
                                return 1;
                        }
                }
        }
        return 0;
}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s) {
		// Allocate memory for node.
        node *n = calloc(1,sizeof(node)+sizeof(array_1d*));
		// Set node name in node identifier.
		n->identifier = s;
		// Create empty neighbours graph.
        n->neighbours = array_1d_create(0, g->node_max, NULL);
		// Iterate over graph.
        for(int i = 0; i <= g->node_max; i++) {
				// Allocate memory for neighbours.
                int *nb = malloc(sizeof(nb));
                *nb = 0;
				// Set neighbours.
                array_1d_set_value(n->neighbours, nb, i);
        }
        n->seen_status = 0;
		// Set nodes.
        array_1d_set_value(g->nodes, n, g->n);
        g->n++;
        return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s) {
        array_1d *nodes = g->nodes;
		// Iterate to find and store node.
        for(int i = 0; i < g->n; i++) {
                node *n = array_1d_inspect_value(nodes, i);
                const char *identifier = n->identifier;
				// If true return n.
                if(!strcmp(s, identifier)) {
                        return n;
                }
        }
        return NULL;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n) {
        return n->seen_status;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen) {
		// Set seen status.
        n->seen_status = seen;
		// Return graph.
        return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g) {
        array_1d *nodes = g->nodes;
		// Iterate over nodes to reset seen status.
        for(int r = array_1d_low(g->nodes); r < g->n; r++) {
                node *n = array_1d_inspect_value(nodes, r);
                n->seen_status = 0;
        }
        return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2) {
        array_1d *nodes = g->nodes;
        int srcidx, destidx;
		// Iterate to find and insert edges.
        for(int i = 0; i < g->n; i++) {
                node *n = array_1d_inspect_value(nodes, i);
				// If equal set source.
                if(nodes_are_equal(n1,n)) {
                        srcidx = i;
                }
				// If equal set destination.
                if(nodes_are_equal(n2,n)) {
                        destidx = i;
                }
        }
		// Inspect value for specifik source.
        node *changeNode = array_1d_inspect_value(nodes, srcidx);
		// Set neighbours.
        array_1d *neighbours = changeNode->neighbours;
		// Inspect nodes for specifik destination.
        int *val = array_1d_inspect_value(neighbours, destidx);
        *val = 1;
        return g;
}

/**
 * graph_delete_node() - Remove a node from the graph.
 * @g: Graph to manipulate.
 * @n: Node to remove from the graph.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the node is not in the graph.
 */
graph *graph_delete_node(graph *g, node *n) {
        array_1d *myneighbours = n->neighbours;
		// Iterate over graph to free memory.
        for(int i = 0; i <= g->node_max; i++) {
                int *temp = array_1d_inspect_value(myneighbours, i);
                free(temp);
        }
		// Kill and free memory.
        array_1d_kill(myneighbours);
        free(n);
        //g->n--;
        return g;
}

/**
 * graph_choose_node() - Return an arbitrary node from the graph.
 * @g: Graph to inspect.
 *
 * Returns: A pointer to an arbitrayry node.
 *
 * NOTE: The return value is undefined for an empty graph.
 */
node *graph_choose_node(const graph *g) {
        array_1d *nodes = g->nodes;
		// Inspect value at first position.
        node *n = array_1d_inspect_value(nodes, 0);
        return n;
}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g, const node *n) {
		// Create an empty list.
        dlist *neighboursCopy = dlist_empty(NULL);
        array_1d *neighbourarr = n->neighbours;
        array_1d *nodes = g->nodes;
		// Iterate over graph.
        for(int i = 0; i < g->n; i++) {
				// If true inspect value and insert into list
                if(*(int*)array_1d_inspect_value(neighbourarr,i)) {
                        node *n = array_1d_inspect_value(nodes, i);
                        dlist_insert(neighboursCopy, n, dlist_first(neighboursCopy));
                }
        }
        return neighboursCopy;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g) {
        array_1d *nodes = g->nodes;
		// Iterate over graph.
        for(int i = 0; i < g->n; i++) {
				// Inspect all nodes and delete.
                node *inspectedNode = array_1d_inspect_value(nodes, i);
                g = graph_delete_node(g, inspectedNode);
        }
		// Kill nodes and free graph.
        array_1d_kill(nodes);
        free(g);

}
