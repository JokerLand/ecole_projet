#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "commun.h"

#define ADRESSE "serveur.h"
#define POINTEURJOUEUR 0
#define TAILLE_INITIAL 5

int initMemoirePartagee();
void fermerMemoirePartagee(int, int);
void writeToMemory(partie *);
joueur * readMemory(int);
void liaisonMemoire();
/* int allouerPointeur(joueur ** , int , int ); */