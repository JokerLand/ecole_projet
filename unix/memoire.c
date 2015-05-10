#include "memoire.h"
joueur *_tabJoueurs = NULL;
int _idMemoirePartagee = 0;
int _taillePhysique = 0;


int initMemoirePartagee(){
    int id;
    joueur ** ptr;
	key_t key_lock;
	
	
	SYS(key_lock = ftok(ADRESSE,(key_t)1)); // obtention de cle
	SYS(id = shmget(key_lock,(sizeof(joueur*) * 5),IPC_CREAT|0644)); //creation de mutex
/*     if ((id = shmget(ADRESSE, (sizeof(joueur*) * 5), IPC_CREAT | 0600)) == -1){ 
        perror("Erreur lors de la creation de l'adresse memoire partagée" );
        exit(-1);    
    } */
	
	_taillePhysique = 1;
	
	if(id == -1) {
		perror("Erreur lors de la creation de l'adresse memoire partagée" );
        exit(1); 
	
	}
	
	/* Initialisation de la memoire partagée */ 
	
    if((ptr =( (joueur **) shmat(id,NULL,0))) == (joueur**) -1){
        perror("Erreur lors de la lecture de la mémoire");
        exit(-1);
    }
    memset(ptr, 0, sizeof(joueur*)*5);
	
	_idMemoirePartagee = id;
	
    return id;
}



void fermerMemoirePartagee(int key, int id){
   // if (key) {
        // id = shmget(key, 0, 0);
        // if (id == -1)
            // return ;
    // }
	SYS(shmctl(id, IPC_RMID, NULL));
	

}


void writeToMemory(partie *p) { 
	
	joueur ** bal = NULL;
	joueur *bal2 = NULL;
	int i =0;
	if(_tabJoueurs == NULL) {
		liaisonMemoire(); 
	
	}
	printf("L'écriture %d ? \n",_taillePhysique);
	
	for(bal = p->joueurs, i = 0, bal2 = _tabJoueurs; i <(p->inscrits); i++, bal++, bal2++){
		
		printf("iii : %d\n",i);
		/*if(i == _taillePhysique) {
			printf("On agrandis ! \n");
		//_taillePhysique = allouerPointeur(_tabJoueurs, POINTEURJOUEUR, p->inscrits);
			if((_tabJoueurs = (joueur*)malloc(_tabJoueurs, sizeof(joueur)*_taillePhysique*2)) == NULL) {
												perror("Erreur d'allocation de memoire dans le main");
												exit(1);
			}	
			_taillePhysique = _taillePhysique*2;
		}*/
		memcpy(bal2, bal, sizeof(joueur));
	}
}


/*void readMemory(int id, void * tabJoueurs) {*/
joueur * readMemory(int taille) {
	
	joueur *monTabJoueurs = NULL;
	int i = 0, taillePhysique= 0;
	//taillePhysique = allouerPointeur(monTabJoueurs, POINTEURJOUEUR, taille);
	
	if(_tabJoueurs == NULL) {
		liaisonMemoire();
	
	}
	printf("Test lecture %d - %d \n", taillePhysique, taille);
	printf("Prout\n");
	monTabJoueurs = (joueur*) malloc(sizeof(joueur)*5);
	for(i = 0; i <taille; i++) {
		printf("%d - \n",i);
			
			 if(i == taillePhysique) {
				 //taillePhysique = allouerPointeur(monTabJoueurs, POINTEURJOUEUR, i);
			 
			 }
			 
			 
			 
			
			 printf("Test lecture2 %d - %d \n", taillePhysique, taille);
			 printf("Ouesh maguele sjkdfhskdjf2?\n"); 
			 printf("score : %d \n",_tabJoueurs[i].score );
			 monTabJoueurs[i] = _tabJoueurs[i];
			
		
	}
	printf("\nFin lecture memoire.c \n");
	
	return monTabJoueurs;
}

void liaisonMemoire() {
	printf("Yop ? Liaison ? \n");
	_tabJoueurs =(joueur *) shmat(_idMemoirePartagee,0,SHM_RND);
}


int allouerPointeur(joueur * pointeur, int type, int taille) {
	int uneTaille = 10;
	int taillePhysiqueTab = 0;
	printf("Ouesh ?\n"); 
	switch(type) {
		case POINTEURJOUEUR : //malloc joueur 
											if(taille != 0 && taille > 0) {
												uneTaille = taille;
												taillePhysiqueTab = taille*2;
												printf("Ouesh  2?\n"); 
											}
											
											printf("Ouesh 3 ?\n"); 
											printf("uneTaille %d\n", uneTaille);
											if((pointeur = (joueur*)malloc( sizeof(joueur)*uneTaille*2)) == NULL) {
												printf("Euf 41 ?\n\n\n\n"); 
												perror("Erreur d'allocation de memoire dans le main");
												printf("Euf 41 ?\n\n\n\n"); 
												exit(1);
											}	
											printf("Ouesh 41 ?\n\n\n\n"); 
											printf("Ouesh 4 ?\n\n\n\n"); 
											//pointeur = (joueur*) pointeur;
											break;
		
	
	}

	return taillePhysiqueTab;
	
}



