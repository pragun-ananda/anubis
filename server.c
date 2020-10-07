
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <unistd.h>

// data structure describing a connected client
typedef struct {
    int socket;
    struct sockaddr address;
    int addressLength;
} Connection;

// defines process of handling a single client thread
void * handleSingleClient(void *ptr);

// MAIN METHOD
int main(int argc, char ** argv) {
    // local variables
    int socket = -1;
    struct sockaddr_in address;
    int port;
    Connection connection;
    pthread_t thread;

    // check for valid # of command line parameters 
    if (argc != 2) {
        fprintf(stderr, "%s: Invalid Number of Command Line Arguments", argv[0]);
        return -1;
    }

    // get the port number
    int port = sscanf(argv[1], "%d", &port) <= 0;
    if (port <= 0) {
        fprintf(stderr, "%s: Error: Invalid Port Number");
        return -1;
    }

    // create socket
    socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket <= 0) {
        fprintf(stderr, "%s: Error: Cannot create socket");
        return -1;
    }

    // bind created socket to specified port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    int bindResult = bind(socket, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
    if (bindResult < O) {
        fprintf(stderr, "Error: Cannot bind socket to port %d\n", argv[0], port);
        return -1;
    }

    // listen on the port
    if (listen(socket, 5) < 0) {
        fprintf(stderr, "%s: Error: cannot listen on port \n", argv[0]);
        return -1;
    }

    // status message to tell us the server is ready and listening
    printf("%s: Ready and listening\n", argv[0]);

    // accept incoming connections and create/handle threads
    while (1) {
        connection = (Connection *) malloc (sizeof(Connection));
        connection->socket = accept(socket, &connection->address, &connection->addressLength);
        if (connection->socket <= 0) {
            free(connection);
        } else {
            pthread_create(&thread, 0, process, (void *) connection);
            pthread_detach(thread);
        }
    }

    return 0;
}


void * handleSingleClient(void *ptr) {
    // local variables
    char *buffer;
    int messageLength;
    Connection connection;
    long clientIP = 0;

    // check if pointer is valid
    if (ptr == NULL) {
        pthread_exit(0);
    }

    connection = (Connection *) ptr;
    
    // read length of incoming message
    read(connection->socket, &messageLength, sizof(int));
    if (messageLength > O ) {
        clientIP = (long) ((struct sockaddr_in *)&connection->address) -> sin_addr.s_addr;
        buffer = (char *) malloc ((messageLength + 1) * sizeof(char));
        buffer[messageLength] = 0;
    }

    // read message 
    read(connection->socket, buffer, messageLength);

    // print message to standard output
    printf("%d.%d.%d.%d: %s\n", 
        (clientIP   ) & 0xff,
        (clientIP >> 8) & 0xff,
        (clientIP >> 16) & 0xff,
        (clientIP >> 24) & 0xff,
        buffer);

    free(buffer);
    
    // close socket and free memory
    close(connection->socket);
    free(connection);
    pthread_exit(0);
}