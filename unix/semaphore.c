// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// S'occupe de la gestion des sémaphore
#include "commun.h"
#include "semaphore.h"
#include "messages.h"


key_t key_lock;
int mutex = -1, mem = -1;
int rc = 0; /*nb de process lisant ou voulant lire */
struct sembuf sop;



void initMySem(){
	sop.sem_num = 1;
	sop.sem_flg = 0;
	SYS(key_lock = ftok(LOCK_PATH,0)); // obtention de cle
	SYS(mem = semget(key_lock,1,IPC_CREAT|0644)); //creation de mutex
	SYS(semctl(mem,0,SETVAL,1)); // init de mutex
	
	SYS(key_lock = ftok(LOCK_PATH,1)); // obtention de cle
	SYS(mutex = semget(key_lock,1,IPC_CREAT|0644)); //creation de mem
	SYS(semctl(mutex,0,SETVAL,1)); // init de mem
}

joueur * lecteur(int taille, client * c){
	// /* joueur ** tabJoueurs; */
	joueur * tabJoueurs;
	message *m;

	if(c != NULL) {
		
		reinitMessage(m);
		m->type = RCPLUS;
		envoiMessageServeur(c, m);
	} else {
		rcPlus(); //C'est le serveur
	
	}
	
	//lire les donnees
	tabJoueurs = readMemory(taille); // lis tout les scores de tout les joueurs
	
	if(c != NULL) {
		
		reinitMessage(m);
	m->type = RCMOINS;
		envoiMessageServeur(c, m);
	} else {
		rcMoins(); //C'est le serveur
	
	}
	return tabJoueurs;

}

void down(int * semaphore) {
	sop.sem_num = 0;
    sop.sem_op = -1; // down
    sop.sem_flg = 0;
    SYS(semop(*semaphore, &sop, 1));

}

void up (int * semaphore) {
	sop.sem_num = 0;
    sop.sem_op = 1;
    sop.sem_flg = 0;
    SYS(semop(*semaphore, &sop, 1));
}

void redacteur(partie * p){
	joueur ** j;
	/*if((j = (joueur **) shmat(id,NULL,0))==(joueur **) -1){
	        perror("Erreur lors de la lecture de la mémoire");
	       	exit(-1);
	}*/
	if(mem == -1) {
		initMySem();
	}
	
	down(&mem);
	writeToMemory(p);
    up(&mem);
	
}

void fermetureSem() {
    SYS(semctl(mutex,0,IPC_RMID,0)); // destruction de mutex
    SYS(semctl(mem,0,IPC_RMID,0)); // destruction de mem
}

void rcPlus() {
down(&mutex);//zone critique acces a rc
	rc++;
	if(rc == 1){ // Si c'est le premier le lecteur ...
	    down(&mem);
	}
	up(&mutex); //liberer l'acces exclusif de rc

}

void rcMoins() {
down(&mutex);
    	rc--;
    if(rc==0) { // Si c'est le dernier lecteur ...
        up(&mem); 
    }
    up(&mutex); // libérer l'accas exclusif de rc
}
