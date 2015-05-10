# Projet Unix 2014-2015
# GILLES Gaëtan & STEENPUT Mathieu
# makefile du projet unix

all: memoire.o semaphore.o messages.o client serveur

serveur : serveur.o
	gcc -o serveur serveur.o messages.o memoire.o semaphore.o
	
client : client.o
	gcc -o client client.o messages.o memoire.o
	
serveur.o : serveur.c
	gcc -c serveur.c -o serveur.o

client.o : client.c
	gcc -c  client.c -o client.o
	
messages.o : messages.c
	gcc -c messages.c -o messages.o
	
memoire.o : memoire.c
	gcc -c memoire.c -o memoire.o

semaphore.o : semaphore.c
	gcc -c semaphore.c -o semaphore.o
	
clear:
	rm -rf *.o