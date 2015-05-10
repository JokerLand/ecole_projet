// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// Lis et écris dans la mémoire partagée

#include "memoire.h"
joueur *_tabJoueurs = NULL;
static int _idMemoirePartagee = 0;
int _taillePhysique = 0;


int initMemoirePartagee(){
    int id;
    joueur * ptr;
	key_t key_lock;
	
	
	SYS(key_lock = ftok(ADRESSE,(key_t)1)); // obtention de cle
	SYS(id = shmget(key_lock,(sizeof(joueur) * TAILLE_INITIAL),IPC_CREAT|0644)); //creation de mutex
	_taillePhysique = TAILLE_INITIAL;
	
	if(id == -1) {
		perror("Erreur lors de la creation de l'adresse memoire partagée" );
        exit(1); 
	
	}
	
	/* Initialisation de la memoire partagée */ 
	
    if((ptr =( (joueur *) shmat(id,NULL,0))) == (joueur*) -1){
        perror("Erreur lors de la lecture de la mémoire");
        exit(-1);
    }
	
    memset(ptr, 0, sizeof(joueur*)*5);
	
	_idMemoirePartagee = id;
	
    return id;
}



void fermerMemoirePartagee(int key, int id){
	SYS(shmctl(id, IPC_RMID, NULL));
}


void writeToMemory(partie *p) { 
	
	joueur * bal = NULL;
	joueur *bal2 = NULL;
	joueur** j = p->joueurs;
	int i =0;
	if(_tabJoueurs == NULL) {
		liaisonMemoire(); 
	
	}
	if((_tabJoueurs = (joueur*)malloc(sizeof(joueur)*p->inscrits)) == NULL) {
			perror("Erreur d'allocation de memoire dans le main");
			exit(1);
	}	
	_taillePhysique = p->inscrits;
	
	
	for(bal = p->joueurs[0], i = 0, bal2 = _tabJoueurs; i <(p->inscrits); i++, bal++, bal2++){
		memcpy(bal2, bal, sizeof(joueur));
	}
}



joueur * readMemory(int taille) {
	
	joueur *monTabJoueurs = NULL;
	int i = 0, taillePhysique= 0;

	
	if(_tabJoueurs == NULL) {
		liaisonMemoire();
	
	}
	
	
	
	monTabJoueurs = (joueur*) malloc(sizeof(joueur)*taille);
	for(i = 0; i <taille; i++) {
		 monTabJoueurs[i] = _tabJoueurs[i];
	}
	
	
	
	return monTabJoueurs;
}

void liaisonMemoire() {
	key_t key_lock;
	if(_idMemoirePartagee != 0) {
		_tabJoueurs =(joueur *) shmat(_idMemoirePartagee,0,SHM_RND);
	} else {
		SYS(key_lock = ftok(ADRESSE,(key_t)1)); // obtention de cle
		_tabJoueurs =(joueur *) shmat(key_lock,0,SHM_RND)
	}
	
	
}



