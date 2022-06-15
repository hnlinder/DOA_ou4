#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"
#include "dlist.h"


/*
* Implementation of a generic graph/dlist solution for the "Datastructures and
* algorithms" courses at the Department of Computing Science, Umea
* University.
*
* Authors: Isak Mikaelsson (tfy20imn@cs.umu.se)
*          Henrik Linder (tfy18hlr@cs.umu.se)
*
*
* Version information:
*   2022-05-30: v1.0, first public version.
*/
/*Defines a graph structure */
typedef struct graph
{
        int *node_max;
        dlist *nodes;
} graph;
/*Defines a structure for the nodes */
typedef struct node
{
        const char *identifier;
        dlist *neighbours;
        bool seen_status;
} node;
/**
* nodes_are_equal() - Check whether two nodes are equal.
* @n1: Pointer to node 1.
* @n2: Pointer to node 2.
*
* Returns: true if the nodes are considered equal, otherwise false.
*
*/
bool nodes_are_equal(const node *n1, const node *n2)
{
        const char *s1 = n1->identifier;
        const char *s2 = n2->identifier;

        /*strcmp returns 0 if strings are equal hence !strcmp*/
        return !strcmp(s2, s1);
}
/**
* graph_empty() - Create an empty graph.
* @max_nodes: The maximum number of nodes the graph can hold.
*
* Returns: A pointer to the new graph.
*/
graph *graph_empty(int max_nodes)
{     //Allocates memory for graph
        graph *g = calloc(1, sizeof(graph));
        //Creates empty list for nodes
        g->nodes = dlist_empty(NULL);
        return g;
}
/**
* graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
* @g: Graph to check.
*
* Returns: True if graph is empty, otherwise false.
*/
bool graph_is_empty(const graph *g)
{
        return dlist_is_empty(g->nodes);
}
/**
* graph_has_edges() - Check if a graph has any edges.
* @g: Graph to check.
*
* Returns: True if graph has any edges, otherwise false.
*/
bool graph_has_edges(const graph *g)
{     //Takes out first position in list
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {
                node *inspected_node = dlist_inspect(g->nodes, pos);
                if (!dlist_is_empty(inspected_node->neighbours))
                {
                        return 1;
                }
                //Updates position to next pos
                pos = dlist_next(g->nodes, pos);
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
graph *graph_insert_node(graph *g, const char *s)
{     //Allocates memory for node and setting values
        node *Node = calloc(1, sizeof(node) + sizeof(dlist *));
        Node->identifier = s;
        Node->neighbours = dlist_empty(NULL);
        Node->seen_status = false;
        //inserting set values into the graph
        dlist_insert(g->nodes, Node, dlist_first(g->nodes));
        return g;
}
/**
* graph_find_node() - Find a node stored in the graph.
* @g: Graph to manipulate.
* @s: Node identifier, e.g. a char *.
*
* Returns: A pointer to the found node, or NULL.
*/
node *graph_find_node(const graph *g, const char *s)
{
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {
                node *inspected_node = dlist_inspect(g->nodes, pos);
                //If the inspected nodes identifier is the same as given string
                if (!strcmp(inspected_node->identifier, s))
                {
                        //Returns the node
                        return inspected_node;
                }
                pos = dlist_next(g->nodes, pos);
        }
        //If node is not found, returns NULL
        return NULL;
}
/**
* graph_node_is_seen() - Return the seen status for a node.
* @g: Graph storing the node.
* @n: Node in the graph to return seen status for.
*
* Returns: The seen status for the node.
*/
bool graph_node_is_seen(const graph *g, const node *n)
{
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
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{     //Sets seen_status to seen
        n->seen_status = seen;
        return g;
}
/**
* graph_reset_seen() - Reset the seen status on all nodes in the graph.
* @g: Graph to modify.
*
* Returns: The modified graph.
*/
graph *graph_reset_seen(graph *g)
{
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {
                node *inspected_node = dlist_inspect(g->nodes, pos);
                inspected_node->seen_status = false;
                pos = dlist_next(g->nodes, pos);
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
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
        // Get the neighbours of n1
        dlist *n1_neighbours = n1->neighbours;
        void *new_neighbour = (void *)n2->identifier;
        // Insert (identifier of)n2 as a neighbour
        dlist_insert(n1_neighbours, new_neighbour, dlist_first(n1_neighbours));
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
graph *graph_delete_node(graph *g, node *n)
{
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {
                node *inspected_node = dlist_inspect(g->nodes, pos);
                //If the inspected node and given node are equal
                if (nodes_are_equal(n, inspected_node))
                {
                        //Removes node from graph and frees.
                        dlist_remove(g->nodes, pos);
                        dlist_kill(inspected_node->neighbours);

                        free(inspected_node);
                        return g;
                }
        }
        return g;
}
/**
* graph_delete_edge() - Remove an edge from the graph.
* @g: Graph to manipulate.
* @n1: Source node (pointer) for the edge.
* @n2: Destination node (pointer) for the edge.
*
* Returns: The modified graph.
*
* NOTE: Undefined if the edge is not in the graph.
*/
graph *graph_delete_edge(graph *g, node *n1, node *n2)
{
        const void *source = n1->identifier;
        const void *destination = n2->identifier;
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {
                node *inspected_node = dlist_inspect(g->nodes, pos);
                //Compare strings of source and inspected nodes identifier
                if (!strcmp(source, inspected_node->identifier))
                {
                        dlist_pos pos2 = dlist_first(inspected_node->neighbours);
                        while (!dlist_is_end(inspected_node->neighbours, pos2))
                        {
                                void *inspected_identifier = dlist_inspect(inspected_node->neighbours, pos2);
                                if (!strcmp(destination, inspected_identifier))
                                {     //Removes inspected_nodes neighbours
                                        dlist_remove(inspected_node->neighbours, pos2);
                                        return g;
                                }
                                pos2 = dlist_next(inspected_node->neighbours, pos2);
                        }
                        //Prints out error message if given destination does not exist.
                        printf("ERROR: Destination does not exist in the graph!\n");
                        return NULL;
                }
                pos = dlist_next(g->nodes, pos);
        }
        //Prints out error message if given source does not exist.
        printf("ERROR: Source does not exist in the graph!\n");
        return NULL;
}
/**
* graph_choose_node() - Return an arbitrary node from the graph.
* @g: Graph to inspect.
*
* Returns: A pointer to an arbitrary node.
*
* NOTE: The return value is undefined for an empty graph.
*/
node *graph_choose_node(const graph *g)
{
        return dlist_inspect(g->nodes, dlist_first(g->nodes));
}
/**
* graph_neighbours() - Return a list of neighbour nodes.
* @g: Graph to inspect.
* @n: Node to get neighbours for.
*
* Returns: A pointer to a list of nodes. Note: The list must be
* dlist_kill()-ed after use.
*/
dlist *graph_neighbours(const graph *g, const node *n)
{     //Creates an empty list
        dlist *neighbours_copy = dlist_empty(NULL);
        //Defines the list neighbours
        dlist *neighbours = n->neighbours;
        dlist_pos pos = dlist_first(neighbours);
        while (!dlist_is_end(neighbours_copy, pos))
        {
                node *n = graph_find_node(g, dlist_inspect(neighbours, pos));
                //Inserting node n into neighbours_copy
                dlist_insert(neighbours_copy, n, dlist_first(neighbours_copy));
                pos = dlist_next(neighbours, pos);
        }
        //returns the shallow copy
        return neighbours_copy;
}
/**
* graph_kill() - Destroy a given graph.
* @g: Graph to destroy.
*
* Return all dynamic memory used by the graph.
*
* Returns: Nothing.
*/
void graph_kill(graph *g)
{
        dlist *the_nodes = g->nodes;
        while (!dlist_is_empty(the_nodes))
        {
                node *inspected_node = graph_choose_node(g);
                g = graph_delete_node(g, inspected_node);
        }
        //Kills list and free memory used by graph
        dlist_kill(the_nodes);
        free(g);
}
/**
* graph_print() - Iterate over the graph elements and print their values.
* @g: Graph to inspect.
*
* Prints the contents of the graph.
*
* Returns: Nothing.
*/
void graph_print(const graph *g)
{
        dlist_pos pos = dlist_first(g->nodes);
        while (!dlist_is_end(g->nodes, pos))
        {     //Defines inspected node
                node *inspected_node = dlist_inspect(g->nodes, pos);
                //Defines a list of neighbours
                dlist *neighbours = graph_neighbours(g, inspected_node);//inspected_node->neighbours;
                const char *identifier = inspected_node->identifier;
                bool seenstatus = inspected_node->seen_status;
                dlist_pos pos2 = dlist_first(neighbours);
                //Prints out identifier
                printf("\nIdentifier: %s\n", identifier);
                //Prints out neighbours to corresponding identifier
                printf("Neighbours: ");
                while (!dlist_is_end(neighbours, pos2))
                {
                        node *tmpnode = dlist_inspect(neighbours, pos2);
                        const char *tmpstr = tmpnode->identifier;
                        printf("%s            \n            ", tmpstr);//(char *)dlist_inspect(neighbours, pos2));
                        pos2 = dlist_next(g->nodes, pos2);
                }
                //prints seen status
                printf("\nSeenstatus: %d\n\n", seenstatus);
                pos = dlist_next(g->nodes, pos);
        }
}
