// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// fonction utile pour l'envoi et la réception des messages

#include "messages.h"

message * initMessage(){
    message * m;
    if((m=(message *)malloc(sizeof(message)))==NULL){
        return NULL;
    }
    reinitMessage(m);
    return m;
}


void reinitMessage(message* m){
    m->type=-1;
	m->numeroTuile = -1;
	m->numeroPlacement = -1;
    strncpy(m->message,"\0",1);
}

int readMessage(int file, message * m){
    int errno;
    int taille;
    reinitMessage(m);
    errno = 0;
    taille=recv(file, m, sizeof(message), 0);
    if(taille==-1){
        if(errno==EAGAIN){
            SYS(kill(SIGPIPE,getpid()));
        }else{
            perror("Erreur ");
        }
    }
    return taille;
}

void envoiMessageServeur(client* s, message* m){
    if(send(s->sck,m,sizeof(message), 0)==-1){
        fprintf(stderr,"Erreur d'envoi du message au Serveur\n");
        exit(1);
    }
    reinitMessage(m);
}



void envoiMessageClient(int socket ,message* m){
    if(send(socket,m,sizeof(message),0)==-1){
        fprintf(stderr,"Erreur d'envoi du message au Client\n");
        exit(1);
    }
}

void envoiMessageClients(partie* p, message* m){
	int i;
	joueur** j = p->joueurs;
    for (i=0; i<(p->inscrits); i++) {
        if(j[i]->etat==ACTIF){
            envoiMessageClient(j[i]->socket,m);
        }
    }
    reinitMessage(m);
}
