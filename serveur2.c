#include "serveur2.h"



partie p ; 



int main (int argc, char *argv[]) {
	int n= 0, temp = 0, sockfd, maxSocket = 0, s2 = 0, t = 0 ;
	struct sockaddr_in skaddrServer, skaddrClient;
	struct timeval tv;
	char buffer[BUFFERSIZE] = "";
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
		reinitMessage(messageLecture);
		reinitMessage(messageEcriture);
		
		tv.tv_sec  = TIMEOUT_SEC;// 
		
		printf("SERV - Attente de connexion\n");
		if((n = select(maxSocket +1, &rdfsAccept, NULL, NULL, &tv)) < 0 ) {
			if(errno != EINTR) {
				perror("select()\n");
				exit(errno);
			}
		}else if(n == 0) {
			printf("\nTIME OUT\n");
			// Ici on s'occupe que de l'inscirption pour le moment, donc je relance direct le timer :
			tv.tv_sec  = TIMEOUT_SEC;
		}
		
		if(p.inscrits == MAX_JOUEURS) { // Refuser immédiatement le client
				printf("SERV - Joueur à tente de se connecter mais a ete refuser par le serveur");
				messageEcriture->type = INSCRIPTIONKO;
				envoiMessageClient(s2, messageEcriture);
				SYS(close(s2));
		
		} else {
			printf("Yop !\n");
			if(FD_ISSET(sockfd, &rdfsAccept) ) { //Je check si le socket est bien dans l'ensemble pour ne pas devoir attendre une connection, histoire que le accepte ce passe directement
             
			int tailleMessage;
			temp = sizeof(skaddrClient);
            
            if((s2 = accept(sockfd, (struct sockaddr *) &skaddrClient, &t)) == -1){
                perror("SERV - Erreur d'accept\n");
				exit(1);
			}
            
            printf("SERV - Un joueur connecte\n");
			// Réception du premier message
			tailleMessage = readMessage(s2, messageLecture);
			if(messageLecture->type == INSCRIPTION) {
				nouveauJoueur(messageLecture->message, s2, CONNECTION);
				printf("SERV - %s inscrit", messageLecture->message);
				
				messageEcriture->type = INSCRIPTIONOK; 
				envoiMessageClient(s2, messageEcriture);
				
				if(p.inscrits == 1) {
					alarm(10); // TODO remplacer par une constance
				}
            
				maxSocket = s2 > maxSocket ? s2 : maxSocket;
				s2 = 0;
			} else {
				// S'il demande pas l'inscritpion en premier, c'est que c'est un gars chelou, je préfère ne pas discuter avec lui
				SYS(close(s2));
				
			}
        }
		}
		
	}//END WHILE (1)
	
	
	
}// END MAIN

/**
return : retourne 0 si un problème est survenu, sinon 1;
*/

int nouveauJoueur(char *nom, int socket, int etat) {
	//TODO Véirifier si un joueur ne possède pas déjà le même nom.
	
	joueur * j;
	if((j = (joueur*) malloc(sizeof(joueur))) == NULL) {
		perror("Erreur d'allocation de memoire dans le main");
		exit(1);
	}
	j->etat = etat;
	j->socket = socket;
	if(nom != NULL) {
		strcpy(j->nom, nom);
	}
	j->score = INITSCORE;
	p.joueurs[p.inscrits] = j;
	p.inscrits++;
	
	return 1;
}

void commencerPartie(){
	// Méthode qui s'occupera de lancer la partie. Pour l'instant, le serveur se coupe simplement.
}

void initServeur(int *sockfd) {

// Affectation des signaux utilisés
	if(signal(SIGALRM, fin)==SIG_ERR){
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
	SYS(close(socket));// Faudrait pas que je ferme tout les sockets enregistrer ? 
    exit(1);
}