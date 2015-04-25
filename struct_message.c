#define INSCRIPTION 1
#define INSCRIPTIONOK 2
#define INSCRIPTIONKO 3
#define TUILEPIOCHE 4
#define TUILEOK 5
#define PARTIEKO 6
#define LEAVE 7
#define BUFFERSIZE 40

typedef struct message {
	int type;
	int numeroTuile;
	int numeroPlacement;
	char message[BUFFERSIZE];
} message;

message * initMessage();