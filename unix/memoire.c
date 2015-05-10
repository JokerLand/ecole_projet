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
	printf("ptr %d\n",ptr);
	
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
	int i =0;
	if(_tabJoueurs == NULL) {
		printf("ecriture\n");
		liaisonMemoire(); 
	
	}
	if((_tabJoueurs = (joueur*)malloc(sizeof(joueur)*p->inscrits)) == NULL) {
			perror("Erreur d'allocation de memoire dans le main");
			exit(1);
	}	
	_taillePhysique = p->inscrits;
	
	joueur** j = p->joueurs;
    for (i=0; i<(p->inscrits); i++) {
  printf("ok1\n");
  printf("%s\n", j[i]->nom);
	}
	
	
	for(bal = p->joueurs[0], i = 0, bal2 = _tabJoueurs; i <(p->inscrits); i++, bal++, bal2++){
		printf("donné de merde : %s  - %d\n", bal->nom, bal->score);
		//printf("
		
		memcpy(bal2, bal, sizeof(joueur));
	}
}



joueur * readMemory(int taille) {
	
	joueur *monTabJoueurs = NULL;
	int i = 0, taillePhysique= 0;

	
	if(_tabJoueurs == NULL) {
		printf("lecture\n");
		liaisonMemoire();
	
	}
	
	
	
	monTabJoueurs = (joueur*) malloc(sizeof(joueur)*taille);
	printf("_____ %d\n",_tabJoueurs);
	
	for(i = 0; i <taille; i++) {
		 monTabJoueurs[i] = _tabJoueurs[i];
		 printf("monTabJoueur %s - %d\n",monTabJoueurs[i].nom, monTabJoueurs[i].score);
		 printf("_tabJoueurs %s - %d\n",_tabJoueurs[i].nom, _tabJoueurs[i].score);
	}
	
	
	
	printf("tabJoueur %d\n",monTabJoueurs);
	return monTabJoueurs;
}

void liaisonMemoire() {
	printf("_sdjfsdsdfs %d\n", _idMemoirePartagee);
	SYS(_tabJoueurs =(joueur *) shmat(_idMemoirePartagee,0,SHM_RND));
	printf("liaison %d\n",_tabJoueurs);
}



