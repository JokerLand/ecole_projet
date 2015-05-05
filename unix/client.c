// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// programme principale du client

#include "client.h"
#include "messages.h"
#include "commun.h"


client * c;

int main(int argc, char *argv[]){

	int sockfd;
	struct sockaddr_in that;
	char buffer[BUFFERSIZE];
	int sizeMessage = 0;
	int boolean = 1;
	int nombreTours = 0;
	int score = 0;

	fd_set fsread;
	message *messageEcriture;
	message *messageLecture;


	if(argc < 2 || argc >3){
    	fprintf(stderr, "Usage : %s  port [fichier]\n", argv[0]);
		exit(1);	
	}

	// Attribution des actions à effectuer lors de la capture d'un signal
    if(signal(SIGINT, fin)==SIG_ERR){
       	perror("Signal: SIGINT");
    }
	
	c=initClient(atoi(argv[1]));
	
	// Connection au serveur
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	c->sck=sockfd;
	that.sin_family = AF_INET;
	that.sin_port = htons(c->port);
	that.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(sockfd,(struct sockaddr *) &that, sizeof(that)) < 0){
		printf("CLIENT - Problem client\n");
		close(sockfd);
		exit(1);
	}

	
	//Initialisation des messages
	messageEcriture = initMessage();
	messageLecture = initMessage();
	
	
	// Inscritpion et connection du client : 
	
	//on demande au client d'entrer son nom
	fprintf(stdout,"CLIENT - Entrer votre pseudo\n> ");
	SYS(sizeMessage = read(0,buffer,BUFFERSIZE));
	
	if( buffer[sizeMessage-1] == '\n' )  buffer[sizeMessage-1] = '\0';
	
	messageEcriture->type=INSCRIPTION;
	strncpy(messageEcriture->message, buffer, BUFFERSIZE);
	
	printf("CLIENT - Envoi du message\n");
	SYS(send(sockfd, messageEcriture, sizeof(message), 0));
	
	if(recv(sockfd, messageLecture, sizeof(message), 0)==-1){
	    //La connexion a été refusée par le serveur
	    fprintf(stderr,"CLIENT - Le serveur a refusé la connexion\n");
	}
	
	
	if (messageLecture->type == INSCRIPTIONOK) {
	        boolean=0;
		printf("CLIENT - Inscription ok\n");
	} else {
		if(messageLecture->type == INSCRIPTIONKO)
			printf("CLIENT - Inscription ko\n");
		fin(0);
	}
		
	while(1) {
		recv(sockfd, messageLecture, sizeof(message), 0);
		
		switch(messageLecture->type) {
			case FERMERCLIENT :
				fin(0);
				break;
			
			case TUILEPIOCHE :
				printf("Tuile %d\n La placée à l'indice :", messageLecture->numeroTuile);
				SYS(sizeMessage = read(0,buffer,BUFFERSIZE));
				messageEcriture->type=TUILEOK;
				messageEcriture->numeroPlacement = atoi(buffer);
				break;
				
			case MONSCORE :
				printf("Votre score est : %d\n", messageLecture->message);
				break;
		}
	}
	
	
}//END MAIN



client * initClient(int port){
    client* c;
    int optval=1;
    
	if((c=(client *)malloc(sizeof(client)))==NULL){
        return NULL;//Erreur de malloc
    }
    
	c->port=port;
    
	if( (c->sck = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
        perror("Erreur de création du socket\n");
        exit(1);
    }
    
	setsockopt (c->sck, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof (optval));
    
	return c;
}


int eteindreClient(client* c){
    SYS(close(c->sck));
    exit(1);
}

void fin(int unused){
    eteindreClient(c);
}