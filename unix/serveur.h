// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// libraire du serveur

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

#include "messages.h"
#include "memoire.h"

#define TIMEOUT_SEC 15

void commencerPartie();
int nouveauJoueur(char *, int , int);
void initServeur(int *);
void fin(int);
int ajouterClient(int);
int chercherJoueurParSocket(int);
