/*
 *  dijkstra.c
 *  ProgrammingPortfolio
 *
 *  Created by Steven Bagley on 08/12/2021.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"

/* return the number of non-zero elements in an array */
int array_contents(const int *, const int);

/* return the id / index of the minimum disctance */
int minimum_distance(int *, const int);

/* index query replaced with 0 */
void remove_element(int *, const int);

/* return true if the query is in the array */
int in_array(const int *, const int, int);

/* find shortest paths between source node id and all other nodes in graph. */
/* upon success, returns an array containing a table of shortest paths.  */
/* return NULL if *graph is uninitialised or an error occurs. */
/* each entry of the table array should be a Path */
/* structure containing the path information for the shortest path between */
/* the source node and every node in the graph. If no path exists to a */
/* particular destination node, then next should be set to -1 and weight */
/* to DBL_MAX in the Path structure for this node */
Path *dijkstra(Graph *graph, int id, int *pnEntries)
{
	int i, j, vertex_count, *ids, *distance, *previous, *set;
	Edge *edge;
	Node *node;
  Path *table;
	Vertex *vertex, *source;

	/* sanity check */
	if (!graph)
		return NULL;
	ids = get_vertices(graph, &vertex_count);
	if (!(source = find_vertex(graph, id)))
		return NULL;
	distance = (int *) malloc(sizeof(int) * (vertex_count));
	previous = (int *) malloc(sizeof(int) * (vertex_count));
	set = (int *) malloc(sizeof(int) * (vertex_count));
	table = (Path *) malloc(sizeof(Path) * (vertex_count));

	if (!(distance && previous && table && set)) {
		if (table) free(table);
		if (distance) free(distance);
		if (previous) free(previous);
		if (set) free(set);
		fprintf(stderr, "error: unable to initialise dynamic memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = j = 0; i < vertex_count; i++) {
		distance[i] = INT16_MAX;
		previous[i] = -1;
		if (ids[i] != id)
			set[j++] = ids[i];
	}
	distance[id] = 0;
	previous[id] = 0;

	while (array_contents(set, vertex_count - 1))
	{
		if (!(i = minimum_distance(distance, vertex_count)))
			break;
		vertex = find_vertex(graph, i);
		remove_element(set, i);

		for (node = vertex->edges->head->data; node; node = node->next) {
			edge = (Edge *) node->data;
			if (in_array(set, edge_destination(edge), vertex_count)) {
				j = distance[i] + edge_weight(edge);
				if (j < distance[edge_destination(edge)]) {
					distance[edge_destination(edge)] = j;
					previous[edge_destination(edge)] = i;
				}
			}
		}
	}

	for (i = 0; i < vertex_count; i++)
	{
		j = i;
		while (previous[j] != id)
			j = previous[j];
		table[i].next_hop = j;
		table[i].weight = distance[i];
	}

  return table;
}

int array_contents(const int *array, const int size)
{
	int i, j;
	for (i = j = 0; i < size; i++) if (array[i]) j++;
	return j;
}

int minimum_distance(int *distances, const int size)
{
	int i, min;
	for (i = min = 0; i < size; i++)
		if (distances[i] < distances[min] && distances[i] != 0)
			min = i;
	distances[min] = 0;
	return min;
}

void remove_element(int *array, const int query)
{
	array[query] = 0;
}

int in_array(const int *array, const int query, int size)
{
	while (size) if (array[--size] == query) return 1;
	return 0;
}

