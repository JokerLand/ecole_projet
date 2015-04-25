all: messages2.o client2 serveur2

serveur2 : serveur2.o
	gcc -o serveur2 serveur2.o messages2.o
	
client2 : client2.o
	gcc -o client2 client2.o messages2.o
	
serveur2.o : serveur2.c
	gcc -c serveur2.c -o serveur2.o

client2.o : client2.c
	gcc -c  client2.c -o client2.o
	
messages2.o : messages2.c
	gcc -c messages2.c -o messages2.o