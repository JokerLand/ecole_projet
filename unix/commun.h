// Projet Unix 2014-2015
// GILLES Gaëtan & STEENPUT Mathieu
// libraire commune du projet

#ifndef Unix_projet
#define Unix_projet

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE !FALSE
#define ACTIF 0 // Si le joueur est toujours connecte
#define DECO -1 // Si le joueur est deconnecte
#define CONNECTION -2 //Le joueur est en cours d'inscription
#define INITSCORE -1
#define MAX_JOUEURS 2
#define COMMENCEE 1

#define SYS(CALL) (((CALL) == -1)? (perror(#CALL ":Error"), exit(1)):0)

typedef unsigned char boolean;

typedef struct joueur {
	int socket;
	char nom[29];
	int score;
	int etat;
} joueur;

typedef struct partie {
        joueur ** joueurs;
        int maxjoueurs;
        int inscrits;
        boolean commencee;
        int memoire;
		int numeroTour;
		int actif; //nombre de joueurs encore actif dans la partie
		int etat;
} partie;

#endif