#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "memoire.h"


#define    	LOCK_PATH	"memoire.h"



void down( int *);
void up(int *);
joueur** lecteur(int);
void redacteur(int, partie*);
void fermetureSem();
void initMySem();
