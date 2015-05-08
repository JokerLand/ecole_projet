#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "commun.h"

#define ADRESSE "serveur.h"

int initMemoirePartagee();
void fermerMemoirePartagee(int, int);
void writeToMemory(joueur **, partie *);
void readMemory(int, void *);
