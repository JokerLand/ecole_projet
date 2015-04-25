#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/mman.h>

#include "commun2.h"
#include "client2.h"

#define INSCRIPTION 1
#define INSCRIPTIONOK 2
#define INSCRIPTIONKO 3
#define TUILEPIOCHE 4
#define TUILEOK 5
#define MONSCORE 6
#define PARTIEKO 7
#define LEAVE 8
#define BUFFERSIZE 40



typedef struct message {
	int type;
	int numeroTuile;
	int numeroPlacement;
	char message[BUFFERSIZE];
} message;

message * initMessage();


message * initMessage();
void reinitMessage(message* );
int readMessage(int, message *);
void envoiMessageServeur(client*, message*);
void envoiMessageClient(int  ,message* );
void envoiMessageClients(partie*, message* );