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
 * Implementation of algorithm that takes a set of nodes and edges, creates
 * a graph from this set and determines whether there's a path from a given
 * starting node to a given destination node.
 *
 * Authors: Henrik Linder (tfy18hlr@cs.umu.se)
 *	   		Isak Mikaelsson (tfy20imn@cs.umu.se)
 *
 * Version information:
 *   2021-05-30: v1.0, first public version.
 */

#define BUFSIZE 500
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
 *
 *
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
 *
 *
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
 *
 *
 */
bool line_has_one_string(const char *s)
{
        return number_of_strings(s) == 1;
}
/**
 * line_is_blank - Tests if a given line of string is blank.
 * @s: Pointer to character.
 *
 * Returns: true if line is blank
 * false otherwise.
 *
 *
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
 * false otherwise.
 *
 *
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
 * false otherwise.
 *
 *
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
 * Returns: integer white_space_cnt.
 *
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
 * Returns: the trimmed line
 *
 */
char *trim(char *line)
{       //Remove lines of comment
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
{       //Creates an empty queue
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
 *
 *
 */
int count_nodes(char **str1, char **str2, int edges) {
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

int main(int argc, const char **argv)
{
        char line[BUFSIZE * 3];
        char **information = malloc(BUFSIZE);
        FILE *in;
        int edges, length1, length2;
        int iter = 0;
        int check = 0;
        char **str1 = malloc(BUFSIZE);
        char **str2 = malloc(BUFSIZE);

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

                information[iter] = trim(line);
                // Check that number of whitespaces are correct
                if (count_white_spaces(information[iter]) != 1)
                {
                        fprintf(stderr, "ERROR: Not the correct number of whitespaces!\n");
                        exit(EXIT_FAILURE);
                }
                //Allocates two string elements
                str1[iter] = malloc(BUFSIZE);
                str2[iter] = malloc(BUFSIZE);
                length1 = white_space(information[iter]);
                length2 = strlen(information[iter]) - length1 - 1;
                //Adds a terminator to string
                str1[iter][length1] = '\0';
                str2[iter][length2] = '\0';
                // Read strings from the file
                memcpy(str1[iter], &information[iter][0], length1);
                memcpy(str2[iter], &information[iter][length1 + 1], length2);
                free(information[iter]);
                iter++;
        }
        if(!iter) {
                fprintf(stderr,"Error: Empty file!\n");
                exit(EXIT_FAILURE);
        }
        //Frees allocated memory used by information
        free(information);
        //Builds the graph
        graph *g = build_graph(str1, str2, count_nodes(str1, str2, edges), edges);
        char str11[3 * BUFSIZE];
        char str22[3 * BUFSIZE];
        do
        {
                printf("Enter origin and destination (quit to exit): ");
                //Reads in entered origin and destination into line
                fgets(line, sizeof(line), stdin);
                sscanf(line, "%s %s", str11, str22);
                //If user enters quit, will skip steps below and exit loop
                if (!strcmp(str11, "quit"))
                {
                        continue;
                }
                else
                {       //Creates nodes corresponding to origin and destination
                        node *origin = graph_find_node(g, str11);
                        node *destination = graph_find_node(g, str22);
                        if (origin == NULL)
                        {       //If user enters a node that does not exist, prints error
                                printf("Node %s does not exist, try again!\n", str11);
                                continue;
                        }
                        if (destination == NULL)
                        {       //If user enters a node that does not exist, prints error
                                printf("Node %s does not exist, try again!\n", str22);
                                continue;
                        }
                        if (find_path(g, origin, destination))
                        {       //If there is a path between origin and destination
                                printf("There is a path from %s to %s.\n\n", str11, str22);
                        }
                        else
                        {       //If there is not a path between origin and destination
                                printf("There is no path from %s to %s.\n\n", str11, str22);
                        }
                }

        } while (strcmp(str11, "quit"));
        //Kills and free memory of graph
        graph_kill(g);

       //Free memery of each allocated element in str1 and str2 respectively
        for (int i = iter - 1; i >= 0; i--)
        {
                free(str1[i]);
                free(str2[i]);

        }
        //Frees str1 & str2
        free(str1);
        free(str2);
        //closes the read-in file
        fclose(in);
        printf("Normal exit.\n");
}
