#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * Peer represents a peer in the network. It contains
 * information about the peer itself and how to connect to it.
 *
 * @field name The name of the peer, maximum 256 bytes
 * @field id The id of the peer as a string, maximum 8 bytes
 * @field sockfd The socket file descriptor used to connect to the peer, when unused is set to -1
 * @field ip The ip address of the peer as a string
 * @field port The port number of the peer
 */
typedef struct {
    char* id; // 8 byte
    char *pal; // 8 byte
    int out_connection_socket_fd; // 8 byte
    int listening_socket_fd; // 8 byte
    int in_connection_socket_fd; // 8 byte
    size_t clilen; 
    const char* ip; // 8 byte 
    uint16_t port; // 2 bytes
    char name[256]; //256 bytes
    char buffer[512]; //512 bytes
}Peer;

typedef struct {
    const char* ip;
    uint16_t port;
}ConnectionParams;



int connect_to(Peer *self,char* peer_id);
int is_alive(Peer* peer);
void become_a_receiver(Peer *self);
ConnectionParams* get_peer_from(char *id);

bool is_connected_to(Peer* self, Peer* peer2);
ssize_t send_to(Peer* peer, char* data, size_t size);
void* accept_connections(void* arg);





#endif