#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "dlist.h"
#include "graph.h"
#include "queue.h"


/*
* Implementation of algorithm that reads nodes and edges from a text file, creates
* a graph from these and determines whether there's a path from a given
* starting node to a given destination node.
*
* Authors: Isak Mikaelsson (tfy20imn@cs.umu.se)
*          Henrik Linder (tfy18hlr@cs.umu.se)
*
* Version information:
*   2022-05-30: v1.0, first public version.
*/

#define BUFSIZE 150
#define bufsize 100
typedef struct graph
{
        int *node_max;
        dlist *nodes;
} graph;

typedef struct node
{
        const void *identifier;
        dlist *neighbours;
        bool seen_status;
} node;
/**
* first_non_white_space() - Check where first non-whitespace is.
* @s: Pointer to character.
*
* Returns: i if s[i] is not NULL
* Returns -1 if s[i] is NULL
*
*/
int first_non_white_space(const char *s)
{
        int i = 0;
        while (s[i] && isspace(s[i]))
        {
                i++;
        }
        if (s[i])
        {
                return i;
        }
        else
        {
                return -1;
        }
}
/**
* last_non_white_space() - Check where last non-whitespace is.
* @s: Pointer to character.
*
* Returns: i if i>= 0,
* Returns -1 else
*
*/
int last_non_white_space(const char *s)
{       //Defines i as length of string -1
        int i = strlen(s) - 1;
        while (i >= 0 && isspace(s[i]))
        {
                i--;
        }
        if (i >= 0)
        {
                return i;
        }
        else
        {
                return -1;
        }
}
/**
* number_of_strings() - Determine number of strings in char.
* @s: Pointer to character.
*
* Returns: an integer, count.
*/
int number_of_strings(const char *s)
{
        int beg = first_non_white_space(s);
        int end = last_non_white_space(s);
        int i = beg;
        int count = 1;
        int check = 0;
        while (i <= end)
        {
                if (isspace(s[i]) && !check)
                {
                        count++;
                        check++;
                        continue;
                }
                check = 0;
                i++;
        }
        return count;
}
/**
* remove_comment() - Removes text strings containing #.
* @s: Pointer to character.
*
* Returns: Modified string without comments.
*/
char *remove_comment(char *s)
{
        int stringlen = strlen(s);
        char *substring = malloc(stringlen + 1);
        int i = 0;
        while (s[i] != '#' && i < stringlen)
        {
                substring[i] = s[i];
                i++;
        }
        substring[i] = '\0';
        return substring;
}
/**
* line_has_one_string() - Tests if given char is just one string.
* @s: Pointer to character.
*
* Returns: true if number_of_strings() returns 1
* false otherwise.
*/
bool line_has_one_string(const char *s)
{
        return number_of_strings(s) == 1;
}
/**
* line_is_blank - Tests if a given line of string is blank.
* @s: Pointer to character.
*
* Returns: true if line is blank, otherwise false
*/
bool line_is_blank(const char *s)
{
        return first_non_white_space(s) < 0;
}
/**
* line_is_comment - Tests if a given line of string is a comment.
* @s: Pointer to character.
*
* Returns: true if line is a comment and if i >= 0
*          false otherwise.
*/
bool line_is_comment(const char *s)
{
        int i = first_non_white_space(s);
        return (i >= 0 && s[i] == '#');
}
/**
* white_space() - Tests if a given line of string is "space"
* @s: Pointer to character.
*
* Returns: integer i if s[i] = true, i.e not NULL
*          false otherwise.
*/
int white_space(const char *s)
{
        int i = 0;
        while (s[i] && !isspace(s[i]))
        {
                i++;
        }
        if (s[i])
        {
                return i;
        }
        else
        {
                return -1;
        }
}
/**
* count_white_spaces() - Counts number of blanks in given line of string.
* @s: Pointer to character.
*
* Returns: integer white_space_cnt which is number of white spaces in line
*/
int count_white_spaces(const char *s)
{
        int i = 0;
        int white_space_cnt = 0;
        //While s[i] != NULL
        while (s[i])
        {       // if s[i] is a "space"
        if (isspace(s[i]))
        {
                white_space_cnt++;
        }
        i++;
}
// -1 to account for the last whitespace
return white_space_cnt;
}
/**
* trim() - trims a given line of string.
* @s: Pointer to character.
*
* Returns: the trimmed line without comments and white spaces
*
*/
char *trim(char *line)
{
        //Remove lines of comment
        char *stemp = remove_comment(line);
        //Takes out the index of first non-whitespace
        int first_non_blank = first_non_white_space(stemp);
        //Takes out the index of last non-whitespace
        int last_non_blank = last_non_white_space(stemp);
        //Determines length of line without blanks
        int length = last_non_blank - first_non_blank + 1;
        //Allocates memory
        char *s = malloc(strlen(stemp) + 1);
        strncpy(s, &stemp[first_non_blank], length);
        //Adds a "ending" to string
        s[length] = '\0';
        //Frees memotry of stemp
        free(stemp);
        return s;
}
/**
* build_graph() - Builds the graph
* @str: Pointer to character.
* @max_elements integer
*
*Returns: The graph with corresponding nodes & strings
*/
graph *build_graph(char **str1, char **str2, int max_elements, int edges)
{
        //Creates empty graph
        graph *g = graph_empty(max_elements);
        for (int i = 0; i < edges; i++)
        {
                //Finding nodes corresponding to str1 & str2 respectively
                if (graph_find_node(g, str1[i]) == NULL)
                {
                        g = graph_insert_node(g, str1[i]);
                }
                if (graph_find_node(g, str2[i]) == NULL)
                {
                        g = graph_insert_node(g, str2[i]);
                }
                // Get the nodes just inserted

                node *n1 = graph_find_node(g, str1[i]);
                node *n2 = graph_find_node(g, str2[i]);
                // Insert an edge from n1 to n2
                if (n1 != NULL && n2 != NULL)
                {
                        g = graph_insert_edge(g, n1, n2);
                }
        }

        return g;
}
/**
* find_path() - function that utilizes breadthfirst
* @g: Pointer to graph.
* @src: Pointer to source-node.
* @dest: Pointer to destination-node
*
*Returns: 1 if nodes are equal
*         0 if not.
*/
bool find_path(graph *g, node *src, node *dest)
{
        //Creates an empty queue
        queue *q = queue_empty(NULL);
        //Changes seen status for source node
        g = graph_node_set_seen(g, src, 1);
        //queues the source node on the queue
        q = queue_enqueue(q, src);
        while (!queue_is_empty(q))
        {       //Inspects the node on the queues front
                node *inspectedNode = queue_front(q);
                if (nodes_are_equal(inspectedNode, dest))
                {       //Resets seen status of all nodes
                        g = graph_reset_seen(g);
                        //kills the queue and frees allocated memory
                        queue_kill(q);
                        return 1;
                }
                //Dequeues node first in queue.
                q = queue_dequeue(q);
                //Creates a list of neighbours to inspectedNode
                dlist *neighbours = graph_neighbours(g, inspectedNode);
                //Takes out first position of list of neighbours.
                dlist_pos pos = dlist_first(neighbours);
                while (!dlist_is_end(neighbours, pos))
                {
                        node *neighbour = dlist_inspect(neighbours, pos);
                        //If given node is NOT seen
                        if (!graph_node_is_seen(g, neighbour))
                        {       //Change seen status to 1(i.e seen)
                                g = graph_node_set_seen(g, neighbour, 1);
                                //queue the node.
                                q = queue_enqueue(q, neighbour);
                        }
                        pos = dlist_next(neighbours, pos);
                }
                //Kills and free allocated memory of list
                dlist_kill(neighbours);
        }
        //Kills and free allocated memory of queue
        queue_kill(q);
        //Resets seen status of nodes
        g = graph_reset_seen(g);
        return 0;
}
/**
* count_nodes() - Counts the number of unique nodes in 2 given strings.
* @str1: string of start nodes.
* @str2: string of destination nodes.
* @edges: number of edges.
*
* Returns: Number of unique nodes.
*/
int count_nodes(char **str1, char **str2, int edges)
{
        // Allocate memory for unique nodes
        char **uniquenodes = malloc(BUFSIZE);
        // Initiate number of unique nodes to 0
        int cnt = 0;
        // This is a status that it used to determine whether a given node already exists
        // in uniquenodes.
        int status1, status2;
        for(int i = 0; i < edges; i++) {
                // Initiate status to true
                status1 = 1;
                status2 = 1;
                // Read node i from tempstr1 and tempstr2
                char *tempstr1 = (char*)str1[i];
                char *tempstr2 = (char*)str2[i];
                for(int j = 0; j < cnt; j++) {
                        // If node tempstr1 already is in uniquenodes
                        if(!strcmp(uniquenodes[j],tempstr1) && status1) {
                                status1 = 0;
                        }
                        // If node tempstr2 already is in uniquenodes
                        if(!strcmp(uniquenodes[j],tempstr2)  && status2) {
                                status2 = 0;
                        }
                }
                if(status1) {
                        // Insert tempstr1 to uniquenodes if it doesn't already exist
                        // and increase cnt by 1.
                        uniquenodes[cnt] = malloc(BUFSIZE);
                        strcpy(uniquenodes[cnt], tempstr1);
                        cnt++;
                }
                if(status2) {
                        // Insert tempstr2 to uniquenodes if it doesn't already exist
                        // and increase cnt by 1.
                        uniquenodes[cnt] = malloc(BUFSIZE);
                        strcpy(uniquenodes[cnt], tempstr2);
                        cnt++;
                }
        }
        // Deallocate memory for uniquenodes.
        for(int i = 0; i < cnt; i++) {
                free(uniquenodes[i]);
        }
        free(uniquenodes);
        // Return cnt.
        return cnt;
}
/**
* set_str_to_empty() - Takes a string and sets first position to '\0'.
* @str: String that is to be emptied.
*
* Returns: Nothing.
*/
void set_nodes_to_empty(char *empty_node1, char *empty_node2)
{
        empty_node1[0] = '\0';
        empty_node2[0] = '\0';
}
/**
* read_map() - Reads and parses information from the map file
* containing a description of a graph.
* @argv: Command line arguments.
* @iter: Iteration counter.
* @str1: Array of origin node names.
* @str2: Array of destination node names.
*
* Returns: Number of edges in the graph.
*/
int read_map(const char **argv, int *iter, char **str1, char **str2)
{
        char line[BUFSIZE];
        char **information = malloc(BUFSIZE);
        FILE *in;
        int edges, length1, length2;
        int check = 0;
        //Reads in the map-file to in
        if (argv[1] != NULL)
        {
                in = fopen(argv[1], "r");
        }
        else {
                fprintf(stderr,"ERROR: No input!\n");
                exit(EXIT_FAILURE);
        }
        if(in == NULL) {
                fprintf(stderr,"ERROR: Empty file!\n");
                exit(EXIT_FAILURE);
        }

        while (fgets(line, BUFSIZE, in) != NULL)
        {       //If line from map-file is blank or comment
                if (line_is_blank(line) || line_is_comment(line))
                {
                        continue;
                }
                // Check if first non-commented line is an integer
                if (check == 0)
                {
                        check = 1;
                        if (!line_has_one_string(line))
                        {       //Prints error to stderr
                                fprintf(stderr, "ERROR: The first line contains more than one string!\n");
                                exit(EXIT_FAILURE);
                        }
                        else if (!isdigit(*line))
                        {       //Prints error to stderr
                                fprintf(stderr, "ERROR: The first line is not a number!\n");
                                exit(EXIT_FAILURE);
                        }
                }

                if (line_has_one_string(line))
                {       //reads number in string
                        edges = atoi(line);
                        if(!isdigit(*(char*)line)) {
                                fprintf(stderr, "ERROR: Bad format!\n");
                                exit(EXIT_FAILURE);
                        }
                        continue;
                }

                information[*iter] = trim(line);
                // Check that number of whitespaces are correct
                if (count_white_spaces(information[*iter]) != 1)
                {
                        fprintf(stderr, "ERROR: Not the correct number of whitespaces!\n");
                        exit(EXIT_FAILURE);
                }

                //Allocates two string elements
                str1[*iter] = malloc(BUFSIZE);
                str2[*iter] = malloc(BUFSIZE);
                length1 = white_space(information[*iter]);
                length2 = strlen(information[*iter]) - length1 - 1;
                //Adds a terminator to string
                str1[*iter][length1] = '\0';
                str2[*iter][length2] = '\0';
                // Read strings from the file
                memcpy(str1[*iter], &information[*iter][0], length1);
                memcpy(str2[*iter], &information[*iter][length1 + 1], length2);
                free(information[*iter]);
                *iter = *iter + 1;
        }
        if(!(*iter)) {
                fprintf(stderr,"Error: Empty file!\n");
                exit(EXIT_FAILURE);
        }
        //Frees allocated memory used by information
        free(information);
        //closes the read-in file
        fclose(in);
        return edges;
}
/**
* free_strings() - Frees all memory in the strings given.
* @str1: Array of origin node names.
* @str2: Array of destination node names.
* @iter: Number of elements in str1 and str2.
*
* Returns: Nothing.
*/
void free_strings(char **str1, char **str2, int *iter)
{
        //Free memory of each allocated element in str1 and str2 respectively
        for (int i = *iter - 1; i >= 0; i--)
        {
                free(str1[i]);
                free(str2[i]);
        }
        //Frees str1 & str2
        free(str1);
        free(str2);
}
/**
* nodes_do_not_exist() - Checks if any of the given nodes are null.
* @origin: Origin node.
* @destination: Destination node.
* @node1: Name of origin node.
* @node2: Name of destination node.
*
* Returns: Name of NULL node - if any node is NULL.
           NULL - If no node is NULL
*/
char *nodes_do_not_exist(node *origin, node *destination, char *node1, char *node2)
{
        if (origin == NULL){
                return (node1);
        }
        else if (destination == NULL){
                return (node2);
        }
        else
                return NULL;
}

int main(int argc, const char **argv)
{
        char line[BUFSIZE];
        int edges;
        // int *iter;
        int it = 0;
        int *iter = &it;
        char **str1 = malloc(BUFSIZE);
        char **str2 = malloc(BUFSIZE);

        // Read map file, return nr of edges
        edges = read_map(argv, iter, str1, str2);

        //Build graph from map information
        graph *g = build_graph(str1, str2, count_nodes(str1, str2, edges), edges);

        // Initialize node names as empty
        char node1[BUFSIZE];
        char node2[BUFSIZE];
        set_nodes_to_empty(node1, node2);
        do
        {
                printf("Enter origin and destination (quit to exit): ");
                //Reads in entered origin and destination into line
                fgets(line, sizeof(line), stdin);
                sscanf(line, "%s %s", node1, node2);
                //If user enters quit, will skip steps below and exit loop
                if (!strcmp(node1, "quit"))
                {
                        continue;
                }
                // If any of the nodes are left empty, prompt the user to reenter
                // origin and destination
                else if (node1[0] == '\0' || node2[0] == '\0'){
                        printf("Please enter both an origin and a destination\n\n");
                        continue;
                }
                else
                {       //Creates nodes corresponding to origin and destination
                        node *origin = graph_find_node(g, node1);
                        node *destination = graph_find_node(g, node2);
                        char *non_existent_node = nodes_do_not_exist(origin, destination, node1, node2);
                        if (non_existent_node){
                                printf("Node %s does not exist, try again!\n\n", non_existent_node);
                                set_nodes_to_empty(node1, node2);
                                continue;
                        }
                        if (find_path(g, origin, destination))
                        {       //If there is a path between origin and destination
                                printf("There is a path from %s to %s.\n\n", node1, node2);
                        }
                        else
                        {       //If there is not a path between origin and destination
                                printf("There is no path from %s to %s.\n\n", node1, node2);
                        }
                        set_nodes_to_empty(node1, node2);
                }

        } while (strcmp(node1, "quit"));

        //Kills and free memory of graph
        graph_kill(g);
        // Free all memory for str1 and str2
        free_strings(str1, str2, iter);
        printf("Normal exit.\n");
}
