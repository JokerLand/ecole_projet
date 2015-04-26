// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// libraire du programme client


#ifndef Projet_Unix_client_h
#define Projet_Unix_client_h


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>


typedef struct client {
    int port;
    int sck;
    int idMem;
    int plateau[20];
}client;

void fin (int);
client * initClient(int);

#endif