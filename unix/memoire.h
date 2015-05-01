#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "partie.h"

#define ADRESSE 0x02200000

int initMemoirePartagee();
void fermerMemoirePartagee(int, int);
void writeToMemory(joueur **, partie *);
joueur ** readMemory(int);
