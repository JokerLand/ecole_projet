#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "messages2.h"

#define TIMEOUT_SEC 15

void commencerPartie();
int nouveauJoueur(char *, int , int);
void initServeur(int *);
void fin(int);
