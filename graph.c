#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

/* initialise an empty graph */
/* return pointer to initialised graph */
Graph *init_graph(void)
{
	return (Graph *) initialise_linked_list();
}

/* release memory for graph */
void free_graph(Graph *graph)
{
	Node *next;

	if (!graph)
		return;

	while (graph->head) {
		next = graph->head->next;
		free_vertex(graph->head->data);
		free_node(graph->head);
		graph->head = next;
	}
	free(graph);
}

/* initialise a vertex */
/* return pointer to initialised vertex */
Vertex *init_vertex(int id)
{
	Vertex *vertex;

	if (!(vertex = (Vertex *) malloc(sizeof(Vertex)))) {
		fprintf(stderr, "error: unable to initialise vertex.\n");
		exit(EXIT_FAILURE);
	}
	vertex->id = id;
	vertex->edges = initialise_linked_list();

	return vertex;
}

/* release memory for initialised vertex */
void free_vertex(Vertex *vertex)
{
	Node *next;

	if (!vertex)
		return;

	while (vertex->edges->head) {
		next = vertex->edges->head->next;
		free_edge((Edge *) vertex->edges->head->data);
		free_node(vertex->edges->head);
		vertex->edges->head = next;
	}
	free(vertex->edges);
	free(vertex);
}

/* initialise an edge. */
/* return pointer to initialised edge. */
Edge *init_edge(void)
{
	Edge *edge;

	if (!(edge = (Edge *) malloc(sizeof(Edge)))) {
		fprintf(stderr, "error: unable to initialise edge.\n");
		exit(EXIT_FAILURE);
	}
	edge->vertex = NULL;
	edge->weight = 0.0;

	return edge;
}

/* release memory for initialised edge. */
void free_edge(Edge *edge)
{
	if (!edge)
		return;
	free(edge);
}

/* remove all edges from vertex with id from to vertex with id to from graph. */
void remove_edge(Graph *graph, int from, int to)
{
	Node *next, *node;
	Vertex *src, *dest;
	
	if (!graph)
		return;
	if (!(src = find_vertex(graph, from)) || \
			!(dest = find_vertex(graph, to)))
		return;

	for (node = src->edges->head; node; node = next) {
		if ((Vertex *) node->data == dest) {
			if (node == src->edges->head) {
				next = node->next;
				free_edge((Edge *) node->data);
				free_node(node);
			} 
			else if (node == src->edges->tail) {
				next = node->prev;
				free_edge((Edge *) node->data);
				free_node(node);
			} else {
				next = node->next;
				free_edge((Edge *) node->data);
				node->prev->next = node->next;
				node->next->prev = node->prev;
				free_node(node);
			}
		} else {
			next = node->next;
		}
	}
}

/* remove all edges from vertex with specified id. */
void remove_edges(Graph *graph, int id)
{	
	Node *next, *node;
	Vertex *vertex;

	if (!graph || !(vertex = find_vertex(graph, id)) || \
			!vertex->edges)
		return;

	for (node = vertex->edges->head; node; node = next) {
		next = node->next;
		free_edge((Edge *) node->data);
		free_node(node);
	}
	vertex->edges->head = vertex->edges->tail = NULL;
}

/* output all vertices and edges in graph. */
/* each vertex in the graphs should be printed on a new line */
/* each vertex should be printed in the following format: */
/* vertex_id: edge_to_vertex[weight] edge_to_vertex[weight] ... */
/* for example: */
/* 1: 3[1.00] 5[2.00] */
/* indicating that vertex id 1 has edges to vertices 3 and 5 */
/* with weights 1.00 and 2.00 respectively */
/* weights should be output to two decimal places */
void print_graph(Graph *graph)
{
	int i, j, vertex_count, edge_count, *vertices;
	Edge **edges;

	if (!graph)
		return;
	vertex_count = edge_count = 0;

	vertices = get_vertices(graph, &vertex_count);
	for (i = 0; i < vertex_count; i++) {
		edges = get_edges(graph, find_vertex(graph, vertices[i]), \
				&edge_count);
		printf("%d:", vertices[i]);
		for (j = 0; j < edge_count; j++)
			printf(" %d[%.2f]", edge_destination(edges[j]), \
					edge_weight(edges[j]));
		printf("\n");
		free(edges);
	}
	free(vertices);
}

/* find vertex with specified id in graph. */
/* return pointer to vertex, or NULL if no vertex found. */
Vertex *find_vertex(Graph *graph, int id)
{
	Node *next;

	if (!graph)
		return NULL;

	for (next = graph->head; next; next = next->next) {
		if ((Vertex *) next->data)
			if (((Vertex *) next->data)->id == id)
				return (Vertex *) next->data;
	}
	return NULL;
}

/* create and add vertex with specified id to graph. */
/* return pointer to vertex or NULL if an error occurs. */
/* if vertex with id already exists, return pointer to existing vertex. */
Vertex *add_vertex(Graph *graph, int id)
{
	Vertex *vertex;

	if (!graph)
		return NULL;
	if ((vertex = find_vertex(graph, id)))
		return vertex;

	vertex = init_vertex(id);
	append_linked_list(graph, (void *) vertex);

	return vertex;
}

/* remove vertex with specified id from graph. */
/* remove all edges between specified vertex and any other vertices in graph. */
void remove_vertex(Graph *graph, int id)
{
	Node *next, *node;
	Vertex *vertex;

	if (!graph || !graph->head)
		return;
	if (!(vertex = find_vertex(graph, id)))
		return;
	node = NULL;

	for (node = graph->head; node; node = node->next)
		if ((Vertex *) node->data == vertex)
			break;
	for (next = graph->head; next; next = next->next)
		remove_edge(graph, ((Vertex *) next->data)->id, id);
	remove_edges(graph, id);

	if (vertex->edges)
		free(vertex->edges);
	free(vertex);
	if (graph->head == node)
		remove_head_linked_list(graph);
	else if (graph->tail == node)
		remove_tail_linked_list(graph);
	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
		free_node(node);
	}
}

/* add directed edge with specified weight between vertex with id from */
/* to vertex with id to. */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple vertices between the same pair of vertices are allowed. */
/* return pointer to edge, or NULL if an error occurs found. */
Edge *add_edge(Graph *graph, int from, int to, double weight)
{
	Edge *edge;
	Vertex *src, *dest;

	if (!graph)
		return NULL;
	src = add_vertex(graph, from);
	dest = add_vertex(graph, to);

	edge = init_edge();
	edge->weight = weight;
	edge->vertex = dest;
	append_linked_list(src->edges, (void *) edge);

	return edge;
}

/* add two edges to graph, one from vertex with id from to vertex with id to, */
/* and one from vertex with id to to vertex with id from. */
/* both edges should have the same weight */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple vertices between the same pair of vertices are allowed. */
void add_edge_undirected(Graph *graph, int from, int to, double weight)
{
	add_edge(graph, from, to, weight);
	add_edge(graph, to, from, weight);
}

/* return array of node ids in graph. */
/* array of node ids should be dynamically allocated */
/* set count to be the number of nodes in graph */
/* return NULL if no vertices in graph */
int *get_vertices(Graph *graph, int *count)
{
	Node *next;
	int *vertices, i;

	if (!graph || !graph->head)
		return NULL;

	for (next = graph->head, *count = 0; next; next = next->next, (*count)++);
	if (!(vertices = (int *) malloc(sizeof(int) * *count))) {
		fprintf(stderr, "error: unable to allocate array for node ids.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0, next = graph->head; i < *count; i++, next = next->next) {
		vertices[i] = ((Vertex *) next->data)->id;
	}
	return vertices;
}

/* return array of pointers to edges for a given vertex. */
/* array of edges should be dynamically allocated */
/* set count to be number of edges of vertex */
/* return NULL if no edges from/to vertex */
Edge **get_edges(Graph *graph, Vertex *vertex, int *count)
{
	int i, j;
	Node *next;
	Edge **edges, *edge;

	if (!graph || !graph->head || !vertex->edges) 
		return NULL;

	for (next = vertex->edges->head, i = 0; next; next = next->next, i++);
	for (next = graph->head, j = 0; next; next = next->next)
		if (get_edge(graph, ((Vertex *) next->data)->id, vertex->id))
			j++;

	if ((j += i) == 0)
		return NULL;
	
	if (!(edges = (Edge **) malloc(sizeof(Edge *) * j))) {
		fprintf(stderr, "error: unable to alloacte array for edge pointers.\n");
		exit(EXIT_FAILURE);
	}
	for (*count = 0, next = vertex->edges->head; *count < i; next = next->next) {
		edges[(*count)++] = (Edge *) next->data;
	}
	for (next = graph->head; (*count) < j; next = next->next) {
		if ((edge = get_edge(graph, ((Vertex *) next->data)->id, vertex->id))) {
			edges[(*count)++] = edge;
		}
	}
	return edges;
}

/* return pointer to edge from vertex with id from, to vertex with id to. */
/* return NULL if no edge */
Edge *get_edge(Graph *graph, int from, int to)
{
	Node *next;
	Vertex *src, *dest;

	src = find_vertex(graph, from);
	dest = find_vertex(graph, to);
	if (!src || !dest)
		return NULL;
	next = src->edges->head;
	for (next = src->edges->head; next; next = next->next) {
		if (((Edge *) next->data)->vertex == dest) {
			return (Edge *) next->data;
		}
	}
	return NULL;
}

/* return id of destination node of edge. */
int edge_destination(Edge *edge)
{
	return edge->vertex->id;
}

/* return weight of edge. */
double edge_weight(Edge *edge)
{
	return edge->weight;
}
