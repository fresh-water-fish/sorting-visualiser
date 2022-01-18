/*
 *  NetworkServer.c
 *
 */

/* You will need to include these header files to be able to implement the TCP/IP functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

/* You will also need to add #include for your graph library header files */
#include "graph.h"
#include "dijkstra.h"

/* define messages */
#define OK "+OK"
#define ERR "-ERR"
#define GREETING_MSG OK" - the server is connected.\r\n\0"
#define QUIT_MSG "QUIT\r\n\0"
#define QUIT_RSP "... closing the connection\r\n\0"

/* function to continually communicate with a given client */
int com_with_client(int, struct sockaddr_in *, struct sockaddr_in *);
/* read a line from a network - practically entirely by Dr. Steven Bagley */
int read_line_network(int, char *, char *, int);
/* tokenise string with space delim */
int string_to_tokens(char *, char *, char *, char *);


/* add network to graph */
int net_add(Graph *networks, int clientFD, char *msg);
/* delete network to graph */
int net_delete(Graph *networks, int clientFD, char *msg);
/* list the networks */
int net_list(Graph *networks, int clientFD, char *msg);
/* add a route betweens networks */
int route_add(Graph *networks, int clientFD, char *msg);
/* delete a route betweens networks */
int route_delete(Graph *networks, int clientFD, char *msg);
/* show routes from source network */
int route_show(Graph *networks, int clientFD, char *msg);
/* show the next hop from source to destination network */
int route_hop(Graph *networks, int clientFD, char *msg);
/* show routing table from source network */
int route_table(Graph *networks, int clientFD, char *msg);

/* convert int to a string */
void kr_itoa(int , char []);
/* reverse a string */
void kr_reverse(char []);
/* --> kr_ functions are from Kernighan and Ritchie */

int main(int argc, const char * argv[])
{
	int serverSocket = -1;
	struct sockaddr_in *serverAddress;

	if (argc != 2)
		return 1;

	const char *port = argv[1];

	printf("Programming Portfolio 2021 Implementation\n");
	printf("=========================================\n\n");
	/* initialise the server socket */
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* initialise the socket address */
	memset(serverAddress, 0, sizeof(*serverAddress));
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(atoi(port));
	serverAddress->sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket, (struct sockaddr *) serverAddress, sizeof(serverAddress));
	listen(serverSocket, 15);

	struct sockaddr_in *clientAddress;
	socklen_t clientLength = sizeof(*clientAddress);
	int clientSocket;

	while (1)
	{
		/* connect to a client */
		clientSocket = accept(serverSocket, (struct sockaddr *) clientAddress, &clientLength);

		if (clientSocket < 0) {
			fprintf(stderr, "Invalid socket (%d) by client %d\n", clientSocket, clientAddress->sin_addr.s_addr);
			return 1;
		}
		/* communicate with said client */
		com_with_client(clientSocket, clientAddress, serverAddress);

	}
	/* close the file desciptor */
	close(serverSocket);
	
	return 0;
}

int com_with_client(int c, struct sockaddr_in *client, struct sockaddr_in *server)
{
	char buff[512], command[16], src[4], dest[4], weight[16], serverMsg[512];
	int c_len, sid, did, network_count = 0, entries = 0, *verticesIDs;
	Edge *edge;
	Graph *networks = init_graph();

	/* greet the client */
	write(c, GREETING_MSG, strlen(GREETING_MSG));
	
	/* read until QUIT is read */
	while (1)
	{
		/* read message */
		read_line_network(c, buff, command, c_len);

		/* if QUIT_MSG then close connection */
		if (strcmp(QUIT_MSG, strtok(buff, " "))) break;

		/* add a network to graph */
		else if (!strcmp("NET-ADD\0", command)) {
			net_add(networks, c);
		}

		/* remove a network from graph */
		else if (!strcmp("NET-DELETE\0", command)) {
			strcpy(src, buff + c_len + 1);
			sid = atoi(src);
			if (find_vertex(networks, sid)) {
				remove_vertex(networks, sid);
				strcpy(serverMsg, OK" Removed \0");
				strcpy(serverMsg + strlen(serverMsg), src);
				strcpy(serverMsg + strlen(serverMsg), ".\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			} else {
				strcpy(serverMsg, ERR" Invalid network.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			}
		}

		/* list the networks */
		else if (!strcmp("NET-LIST\0", command)) {
			verticesIDs = get_vertices(networks, &network_count);
			kr_itoa(network_count, src);
			strcpy(serverMsg, ERR" ");
			strcpy(serverMsg + strlen(serverMsg), src);
			strcpy(serverMsg + strlen(serverMsg), ".\r\n\0");
			write(c, serverMsg, sizeof(serverMsg));
			/* print each network id */
			for (int i = 0; i < network_count; i++) {
				kr_itoa(verticesIDs[i], src);
				strcpy(serverMsg, src);
				strcpy(serverMsg + strlen(serverMsg), "\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			}
			free(verticesIDs);
		}

		/* add bidirectional route between two networks of given weight */ 
		else if (!strcmp("ROUTE-ADD\0", command)) {
			if (string_to_tokens(buff + c_len + 1, src, dest, weight) != 3) {
				strcpy(serverMsg, ERR" Invalid args count.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
				break;
			}
			sid = atoi(src);
			did = atoi(dest);
			/* one network may not exist */
			if (!find_vertex(networks, sid) || !find_vertex(networks, did)) {
				strcpy(serverMsg, ERR" Network doesn't exist.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			}
			/* the link may exist and should be removed then added */
			else {
				if ((edge = get_edge(networks, sid, did))) {
					remove_edge(networks, sid, did);
					remove_edge(networks, did, sid);
				}
				add_edge_undirected(networks, sid, did, strtod(weight, NULL));
				strcpy(serverMsg, OK" Route added.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			}
		}

		/* remove bidirectional route between networks */
		else if (!strcmp("ROUTE-DELETE\0", command)) {
			if (string_to_tokens(buff + c_len + 1, src, dest, weight) != 2) {
				strcpy(serverMsg, ERR" Invalid args read.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
				break;
			}
			sid = atoi(src);
			did = atoi(dest);
			/* one network may not exist */
			if (!find_vertex(networks, sid) || !find_vertex(networks, did)) {
				strcpy(serverMsg, ERR" Network doesn't exist.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			} else {
				remove_edge(networks, sid, did);
				remove_edge(networks, did, sid);
				strcpy(serverMsg, OK" Route removed.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			}
		}

		/* show the routes */
		else if (!strcmp("ROUTE-SHOW\0", command)) {
			Edge **edges;
			strcpy(src, buff + c_len + 1);
			sid = atoi(src);

			if (!(edges = get_edges(networks, find_vertex(networks, sid), &did))) {
				strcpy(serverMsg, ERR" Invalid network.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			} else {
				strcpy(serverMsg, OK" ");
				kr_itoa(did, dest);
				strcpy(serverMsg + strlen(serverMsg), dest);
				strcpy(serverMsg + strlen(serverMsg), ".\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
				/* for each edge */
				for (int i = 0; i < did; i++) {
					kr_itoa(edge_destination(edges[i]), dest);
					kr_itoa(edge_weight(edges[i]), weight); /* change to support proper float */
					strcpy(serverMsg, src);
					strcpy(serverMsg + strlen(serverMsg), " \0");
					strcpy(serverMsg + strlen(serverMsg), dest);
					strcpy(serverMsg + strlen(serverMsg), " \0");
					strcpy(serverMsg + strlen(serverMsg), weight);
					strcpy(serverMsg + strlen(serverMsg), ".00\r\n\0");
					write(c, serverMsg, sizeof(serverMsg));
				}
			}
			free(edges);
		}

		/* respond with the next hop for dest from src */
		else if (!strcmp("ROUTE-HOP\0", command)) {
			if (string_to_tokens(buff + c_len + 1, src, dest, weight) != 2) {
				strcpy(serverMsg, ERR" Invalid args.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			} else {
				sid = atoi(src);
				did = atoi(dest);
				if (!find_vertex(networks, sid) || !find_vertex(networks, did) || (find_vertex(networks, sid) == find_vertex(networks, did))) {
					strcpy(serverMsg, ERR" Invalid network ids.\r\n\0");
					write(c, serverMsg, sizeof(serverMsg));
				} else {
					Path *table = dijkstra(networks, sid, &network_count);
					strcpy(serverMsg, OK" ");
					kr_itoa(table[sid].next_hop, dest);
					free(table);
					strcat(serverMsg, dest);
					strcat(serverMsg, "\r\n\0");
					write(c, serverMsg, sizeof(serverMsg));
				}
			}
		}

		/* respond with the routing table */
		else if (!strcmp("ROUTE-TABLE\0", command)) {
			strcpy(src, buff + c_len + 1);
			sid = atoi(src);
			if (!find_vertex(networks, sid)) {
				strcpy(serverMsg, ERR" Invalid network.\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
			} else {
				Path *table = dijkstra(networks, sid, &entries);
				verticesIDs = get_vertices(networks, &network_count);
				kr_itoa(entries - 1, dest);
				strcpy(serverMsg, OK" \0");
				strcat(serverMsg, dest);
				strcat(serverMsg, "\r\n\0");
				write(c, serverMsg, sizeof(serverMsg));
				for (int i = 0; i < network_count; i++) {
					strcpy(serverMsg, src);
					strcat(serverMsg, " -> ");
					kr_itoa(verticesIDs[i], dest);
					strcat(serverMsg, dest);
					strcat(serverMsg, ", next hop ");
					kr_itoa(table[verticesIDs[i]].next_hop, dest);
					strcat(serverMsg, ", weight ");
					/* and for weight */
					/**/
					strcat(serverMsg, "\r\n\0");
					write(c, serverMsg, sizeof(serverMsg));
				}
				free(table);
				free(verticesIDs);
			}
		}

		/* the client sent a command which does not exist */
		else {
			strcpy(serverMsg, ERR" Invalid command.\r\n\0");
			write(c, serverMsg, sizeof(serverMsg));
		}

	}
	/* respond to quit message with single line response */
	write(c, QUIT_RSP, strlen(QUIT_RSP));

	/* close file descriptor and free memory */
	close(c);
	free_graph(networks);

	return 0;
}

int string_to_tokens(char *str, char *src, char *dest, char *weight)
{
	char *ptr = str, *p;
	int n = 1;
	
	while (*ptr) {
		if (*ptr == ' ') {
			*p = 0;
			if (n == 1) { p = dest; n = 2; }
			else { p = weight; n = 3; }
		}
		*p = *ptr;
		ptr++; p++;
	}
	*p = 0;
	return n;
}

/* a practically entirely Steven Bagley prgram */
int read_line_network(int c, char *buf, char *command, int cLen)
{
	int i, b = 0, count = 0;
	char line[513], *p = buf, *cPtr = command;
	ssize_t n;

	cLen = 0;

	do {
		n = read(c, line, 512);

		for (i = 0; i < n; i++) {
			*p = line[i];
			if (p != buf && *p == 10 && *(p-1) == 13) {
				*(p-1) = 0;
				count = 1;
				break;
			}
			/* until space is input, fill in the command */
			if (*p == ' ') {
				b = 1;
				*cPtr = 0;
			}
			if (!b) {
				*cPtr = *p;
				cPtr++;
				cLen++;
			}
			p++;
		}
		*p = 0;
	} while(!count && n > 0);

	return 0;
}

int net_add(Graph *networks, int c, char *msg)
{
	char *src = strtok(NULL, " ");
	int sid = atoi(src);
	if (!find_vertex(networks, sid)) {
		if (!add_vertex(networks, sid)) {
			strcpy(msg, ERR" Failed to add network.\r\n\0");
			return write(c, msg, strlen(msg));
		} else {
			strcpy(msg, OK" Added \0");
			strcat(msg, src);
			strcat(msg, ".\r\n\0");
			return write(c, msg, strlen(msg));
		}
	} else {
		strcpy(msg, ERR" Network already added.\r\n\0");
		return write(c, msg, strlen(msg));
	}
}

int net_delete(Graph *networks, int c, char *msg)
{
	char *src = strtok(NULL, " ");
	int sid = atoi(src);
	if (find_vertex(networks, sid)) {
		remove_vertex(networks, sid);
		strcpy(msg, OK" Removed \0");
		strcat(msg, src);
		strcat(msg, ".\r\n\0");
		return write(c, msg, strlen(msg));
	} else {
		strcpy(msg, ERR" Invalid network.\r\n\0");
		return write(c, msg, strlen(msg));
	}
}

int net_list(Graph *networks, int c, char *msg)
{
	char *src = strtok(NULL, " ");
	int network_count = 0, *verticesIDs = get_vertices(networks, &network_count);
	kr_itoa(network_count, src);
	strcpy(msg, ERR" ");
	strcat(msg, src);
	strcat(msg, ".\r\n\0");
	write(c, msg, strlen(msg));
	/* print each network id */
	for (int i = 0; i < network_count; i++) {
		kr_itoa(verticesIDs[i], src);
		strcpy(msg, src);
		strcat(msg, "\r\n\0");
		write(c, msg, strlen(msg));
	}
	free(verticesIDs);
}

int route_add(Graph *networks, int c, char *msg)
{
	char *src = strtok(NULL, " "), *dest = strtok(NULL, " "), *weight  =strtok(NULL, " ");
	if (!src || !dest || !weight) {
		strcpy(msg, ERR" Invalid args count.\r\n\0");
		return write(c, msg, strlen(msg));
	}
	int sid = atoi(src);
	int did = atoi(dest);
	/* one network may not exist */
	if (!find_vertex(networks, sid) || !find_vertex(networks, did)) {
		strcpy(msg, ERR" Network doesn't exist.\r\n\0");
		write(c, msg, strlen(msg));
	}
	/* the link may exist and should be removed then added */
	else {
		if ((edge = get_edge(networks, sid, did))) {
			remove_edge(networks, sid, did);
			remove_edge(networks, did, sid);
		}
		add_edge_undirected(networks, sid, did, strtod(weight, NULL));
		strcpy(serverMsg, OK" Route added.\r\n\0");
		write(c, serverMsg, sizeof(serverMsg));
	}
}

/* * * * * * * * DISCLAIMER * * * * * * * */
/* itoa and reverse function taken from Kernighan and Ritchie's */
/* The C Programming Language book */
/* itoa:  convert n to characters in s */
void kr_itoa(int n, char s[])
{
	int i, sign;
	
	if ((sign = n) < 0)  /* record sign */
		n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	kr_reverse(s);
}  
/* reverse:  reverse string s in place */
void kr_reverse(char s[])
{
	int i, j;
	char c;
	
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
