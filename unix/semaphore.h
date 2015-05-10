// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// Bibliothèque de semaphore.c

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "memoire.h"
#include "client.h"

#define    	LOCK_PATH	"memoire.h"



void down( int *);
void up(int *);
joueur * lecteur(int, client *);
void redacteur(partie*);
void fermetureSem();
void initMySem();
void rcPlus();
void rcMoins();
