// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// programme principale du serveur

#include "serveur.h"



partie p ;



int main (int argc, char *argv[]) {
	int n= 0, temp = 0, sockfd, maxSocket = 0, s2 = 0, t = 0 ;
	struct sockaddr_in skaddrServer, skaddrClient;
	struct timeval tv;
	char buffer[BUFFERSIZE] = "";
	boolean alarmDemarree = FALSE;
	fd_set rdfsAccept, readset, bal;
	message *messageLecture;
	message *messageEcriture;
	joueur j;

	if(argc < 2 || argc >3){
		fprintf(stderr, "Usage : %s  port [fichier]\n", argv[0]);
		exit(1);
	}

	initServeur(&sockfd);
	maxSocket = sockfd;

	//Init timeout
	tv.tv_sec  = TIMEOUT_SEC;
	tv.tv_usec = 0;

	//Initialisation des messages
	messageLecture = initMessage();
	messageEcriture = initMessage();

	//Ecoute du serveur
	bzero(&skaddrServer,sizeof(skaddrServer));
	skaddrServer.sin_family = AF_INET;
	skaddrServer.sin_port = htons(atoi(argv[1]));
	skaddrServer.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd,(struct sockaddr *)&skaddrServer,sizeof(skaddrServer)) < 0){
		perror("Erreur de bind\n");
		exit(1);
	}

	if(listen(sockfd, 5) <0){
		fprintf(stderr, "Erreur de listen\n");
		exit(1);
	}
	FD_ZERO(&rdfsAccept); // ensemble rdfsAccept remis à zero
	FD_SET(sockfd, &rdfsAccept); // Ajout du sockfd à l'ensemble rdfsAccept

    while (1) {
		int i = 0;
		joueur** j2 = p.joueurs;
		reinitMessage(messageLecture);
		reinitMessage(messageEcriture);

		tv.tv_sec  = TIMEOUT_SEC;//

		printf("SERV - Attente de connexion\n");
		if((n = select(maxSocket +1, &rdfsAccept, NULL, NULL, &tv)) < 0 ) {
			if(errno != EINTR) {
				perror("select()\n");
				exit(errno);
			}
		}else if(n == 0) { //TimeOut du select 
			joueur** j = p.joueurs;
			int i = 0;
			printf("INFO - TIME OUT\n");
			FD_ZERO(&rdfsAccept); // ensemble rdfsAccept remis à zero
			for(i = 0; i<p.inscrits; i++) {
				FD_SET(j[i]->socket, &rdfsAccept);
			}

			FD_SET(sockfd, &rdfsAccept); // Ajout du sockfd à l'ensemble rdfsAccept
			// Ici on s'occupe que de l'inscirption pour le moment, donc je relance direct le timer :
			tv.tv_sec  = TIMEOUT_SEC;
		
		} else {
			int i = 0;
			if(FD_ISSET(sockfd, &rdfsAccept)) {
				
				if(p.inscrits == MAX_JOUEURS) { // Refuser immédiatement le client
					printf("SERV - Joueur à tente de se connecter mais a ete refuser par le serveur\n");
					messageEcriture->type = INSCRIPTIONKO;
					envoiMessageClient(s2, messageEcriture);
					SYS(close(s2));
				} else {
					
					temp = sizeof(skaddrClient);

					if((s2 = accept(sockfd, (struct sockaddr *) &skaddrClient, &t)) == -1){
						perror("SERV - Erreur d'accept\n");
						exit(1);
					}

					printf("SERV - Un joueur connecte\n");
					
					ajouterClient(s2);
					maxSocket = s2 > maxSocket ? s2 : maxSocket;
					s2 = 0;
				}
				
				
			}else {
		
		

				for(i = 0; i < p.inscrits; i++) {
					int socketJoueur = p.joueurs[i]->socket;
					int tailleMessage;
					
					if(FD_ISSET(socketJoueur, &rdfsAccept)){
						tailleMessage = readMessage(socketJoueur, messageLecture);

						switch(messageLecture->type) {
							
							case INSCRIPTION : 	
												nouveauJoueur(messageLecture->message, socketJoueur, CONNECTION);
												printf("SERV - %s inscrit\n", messageLecture->message);

												messageEcriture->type = INSCRIPTIONOK;
												envoiMessageClient(socketJoueur, messageEcriture);

												if(!alarmDemarree) { //TODO remplacer le 1 par une constance
													printf("INFO - Lancement de l'alarm\n");
													alarm(10); // TODO remplacer par une constance
													alarmDemarree = TRUE;
												}
												break;
												
							case INSCRIPTIONOK : break;
							case INSCRIPTIONKO : break;
							case TUILEPIOCHE : break;
							case TUILEOK : break;
							case MONSCORE : break;
							case PARTIEKO : break;
							case LEAVE : break;
							case FERMERCLIENT : break;
							case BUFFERSIZE : break;
						}//END SWICH
						
						
					}			
				}
		
			}
		
		
		
		

		} 
		FD_ZERO(&rdfsAccept); // ensemble rdfsAccept remis à zero
		for(i = 0; i<p.inscrits; i++) {
				FD_SET(j2[i]->socket, &rdfsAccept);
			}

		FD_SET(sockfd, &rdfsAccept); // Ajout du sockfd à l'ensemble rdfsAccept
		
	}//END WHILE (1)



}// END MAIN

/**
return : retourne 0 si un problème est survenu, sinon 1;
*/

int nouveauJoueur(char *nom, int socket, int etat) {
	//TODO Véirifier si un joueur ne possède pas déjà le même nom.
	
	joueur * j;
	int idJoueur = chercherJoueurParSocket(socket);

	if(idJoueur == -1) {
		printf("Problème !\n");
	}
	j = p.joueurs[idJoueur];
	
	j->etat = etat;
	j->socket = socket;

	if(nom != NULL) {
		strcpy(j->nom, nom);
	}

	j->score = INITSCORE;
	p.joueurs[p.inscrits] = j;
	//p.inscrits++;

	return 1;
}

int chercherJoueurParSocket(int socket) {
	joueur ** bal;
	bal = p.joueurs;
	int i = 0;
	
	for(i = 0; i < p.inscrits; i++) {
		if(p.joueurs[i]->socket == socket) return i;
	}
	
	return -1;
}


int ajouterClient(int socket) {
	joueur *j ;

	if((j = (joueur*) malloc(sizeof(joueur))) == NULL) {
		perror("Erreur d'allocation de memoire dans le main");
		exit(1);
	}

	j->etat = CONNECTION;
	j->socket = socket;
	strcpy(j->nom, "");

	j->score = INITSCORE;
	p.joueurs[p.inscrits] = j;
	p.inscrits++;


	
	return 1;
}

void commencerPartie(){
	// Méthode qui s'occupera de lancer la partie. Pour l'instant, le serveur se coupe simplement.
	int i = 0;
	int id = 0;
	joueur ** j;
	
	for(i = 0; i < p.inscrits; i ++) {
		p.joueurs[i]->score = 100*random();
	}
	
	id = initMemoirePartagee();
	
	redacteur(id, &p);
	
	
	printf("Ok ?\n");
	printf("Test lecture des scores");
	j = lecteur(id);
	
	for(i = 0; i < p.inscrits; i ++) {
		printf("Nom : %s --- socre : %d",p.joueurs[i]->nom, p.joueurs[i]->score);
	}
	fermetureSem() ;
	fermerMemoirePartagee(0,id);
}

void initServeur(int *sockfd) {

// Affectation des signaux utilisés
	if(signal(SIGALRM, commencerPartie)==SIG_ERR){
		perror("Signal : SIGALRM");
	}
		// Control-C
	if(signal(SIGINT, fin)==SIG_ERR){
			perror("signal: SIGINT");
	}

	SYS(*sockfd = socket(AF_INET, SOCK_STREAM, 0));
		//Init tableau joueurs
	if((p.joueurs = (joueur**) malloc(sizeof(joueur*)*p.maxjoueurs)) == NULL) {
		perror("Erreur d'allocation de memoire dans le main");
		exit(1);
	}
	
	

}


void fin(int socket){
	//SYS(close(socket));// Faudrait pas que je ferme tout les sockets enregistrer ?

	// On avertit aux clients que c'est fini.
	int i = 0;
	message *messageEcriture;
	messageEcriture = initMessage();
	joueur** j = p.joueurs;
	for(i = 0; i<p.inscrits; i++) {
		messageEcriture->type = FERMERCLIENT;
		envoiMessageClient(j[i]->socket, messageEcriture);
		SYS(close(j[i]->socket));
	}
	SYS(close(socket));
    exit(1);
}
