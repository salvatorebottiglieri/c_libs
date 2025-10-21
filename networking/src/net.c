#include "../include/sep_net.h"
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



#define LISTEN_PORT 8080
#define SEND_BUFFER_SIZE 512

/*------------------------------------------- HELPER FUNCTIONS ---------------------------------------------------- */

/**
 * Checks if a given Peer object is valid.
 * A Peer object is considered valid if its name, id, ip and port are not null.
 * @param peer The Peer object to check
 * @return true if the Peer object is valid, false otherwise
 */
bool is_valid(char *peer_id){
    return true;
}

int create_socket(int domain){
    int sockfd = socket(domain, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }
    return sockfd;
}

int close_socket(int sockfd){
    if (close(sockfd) < 0){
        perror("close failed");
        return -1;
    }
    return 0;
}


/*------------------------------------------- FUNCTIONS RESERVED TO ACCESS POINTS ---------------------------------------------------- */

bool is_part_of_the_net(char *peer_id){
    return false;
    
}




/*------------------------------------------- PUBLIC API ---------------------------------------------------- */

bool is_connected_to(Peer* self, Peer* peer2){
    return strcmp(self->pal, peer2->id) == 0;
    
}

void become_a_receiver(Peer *self){
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    sockfd = create_socket(AF_INET);
    if (sockfd < 0){
        perror("socket creation failed");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(self->port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
    }

    if (listen(sockfd, 10) < 0) {
        perror("listen failed");
    }

    clilen = sizeof(cliaddr);
    self->in_connection_socket_fd = sockfd;
    self->clilen = clilen;
}



/**
 * Connects to a peer identified by the given id.
 * The function creates a new socket using AF_INET and connects to the peer using the peer's ip and port.
 * If the connection is successful, the socket file descriptor is stored in the self->out_connection_socket_fd field.
 * @param self The Peer object to connect from
 * @param peer_id The id of the peer to connect to
 * @return 0 if the connection is successful, -1 otherwise
 */
int connect_to(Peer *self,char* peer_id){
    if (!is_valid(peer_id)){return -1;}
    Peer* peer = get_peer_from(peer_id);
    struct sockaddr_in servaddr;
    int socket = create_socket(AF_INET);
    if (socket < 0){
        return -1;
    }
    self->out_connection_socket_fd = socket;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(peer->port);
    inet_pton(AF_INET, peer->ip, &(servaddr.sin_addr));
    if(connect(self->out_connection_socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        close_socket(self->out_connection_socket_fd);
        self->out_connection_socket_fd = -1;
        return -1;
    }

    return 0;
}




ssize_t send_to(Peer* peer, char* data, size_t size){
    char* buffer = data;
    size_t num_of_sending = 1;
    ssize_t bytes_sended = 0;
    size_t bytes_to_send = SEND_BUFFER_SIZE;

    if (size > bytes_to_send){
        num_of_sending = size / bytes_to_send;
    }

    for (size_t i = 0; i < num_of_sending; i++){

        ssize_t chunk_sent = send(peer->out_connection_socket_fd, buffer+bytes_sended, bytes_to_send, 0);
        if (chunk_sent < 0) {perror("send failed");return -1;}

        if((size - bytes_sended) < SEND_BUFFER_SIZE){
            bytes_to_send = size - bytes_sended;
        }

        bytes_sended += chunk_sent;
        
    }

    return bytes_sended;
}



void* accept_connections(void* arg){

    Peer *self = (Peer*) arg;
    int sockfd = self->in_connection_socket_fd;
    char *buffer = self->buffer;
    struct sockaddr_in cliaddr;
    socklen_t clilen = self->clilen;
    int connfd;

    while(1){
        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept failed");
            pthread_exit(NULL);
        }
        int n = read(connfd, buffer, 512);
        if (n < 0) {
            perror("read failed");
            pthread_exit(NULL);
        }


    }
    close(connfd);
    close(sockfd);
}
