#include "commun.h"
#include "semaphore.h"



int initMemoirePartagee(){
    int id;
    joueur ** ptr;
	
    if ((id = shmget(ADRESSE, (sizeof(joueur*) * 5), IPC_CREAT | 0600)) == -1){ 
        perror("Erreur lors de la creation de l'adresse memoire partagée" );
        exit(-1);    
    }
    if((ptr = (joueur **) shmat(id,NULL,0)) ==  -1){
        perror("Erreur lors de la lecture de la mémoire");
        exit(-1);
    }
    memset(ptr, 0, sizeof(joueur*));
    return id;
}



void fermerMemoirePartagee(int key, int id){
   if (key) {
        id = shmget(key, 0, 0);
        if (id == -1)
            return ;
    }
	SYS(shmctl(id, IPC_RMID, NULL));
	

}


void writeToMemory(joueur **j, partie *p) { 
	joueur ** bal, **bal2;
	int i;
	for(bal = p->joueurs, i = 0, bal2 = j; i <(p->inscrits); i++, bal++, bal2++){
		memcpy(bal2, bal, sizeof(joueur*));
	}
}


joueur ** readMemory(int id) {
	joueur ** j;
	printf("Test lecture\n");
	if((j = (joueur **) shmat(id,NULL,0))==(joueur **) -1){
        perror("Erreur lors de la lecture de la mémoire");
        exit(-1);
    }
	
	return j;
}
