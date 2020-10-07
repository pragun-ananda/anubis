// CLIENT IMPLEMENTATION

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

// data structure to store important client information
struct Client {
    int port;
    int socket;
    struct sockaddr_in address;
    struct hostent *host;
    int length;
};

// forward declaration for a method that will connect client with a server
int connectToServer(struct Client *client, char *argOne, char *argTwo);

// MAIN METHOD
int main (int argc, char** argv) {
    struct Client client;

    // check for valid # of command line parameters
    if (argc != 4) {
        printf("%s Invalid Number of Command Line Parameters", argv[0]);
        return -1;
    }

    // get port number
    client.port = sscanf(argv[2], "%d", &client.port);
    if (client.port <= 0) {
        fprintf(stderr, "%s Error: Invalid Port Number ", argv[0]);
        return -1;
    }

    // create a TCP socket
    client.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client.socket <= 0) {
        fprintf(stderr, "%s: Error: Cannot Create the Socket", argv[0]);
        return -1;
    }

    // connect to server 
    int result = connectToServer(&client, argv[0], argv[1]);

    // handle errors 
    if (result == -1) {
        fprintf(stderr, "%s: Error Unknown Host %s\n", argv[0], argv[1]);
        return result;
    } else if (result == -2) {
        fprintf(stderr, "%s: error: cannot connect to host %s\n", argv[0], argv[1]);
        return result;
    }

    // send text to server
    client.length = strlen(argv[3]);
    write(client.socket, &client.length, sizeof(int));
    write(client.socket, argv[3], client.length);

    // close socket at the end 
    close(client.socket);
    return 0;
}


// first handles server address and then makes a connection
int connectToServer(struct Client *client, char *argOne, char *argTwo) {
    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(client->port);
    client->host = gethostbyname(argOne[1]);
    if (client->host == 0) {
        return -1;
    }
    memcpy( &client->address.sin_addr, client->host->h_addr_list[0], client->host->h_length );
	
    // make connection
    if ( connect(client->socket, (struct sockaddr *)&client->address, sizeof(client->address)) ) {
	return -2;
    }
    return 0;
}
