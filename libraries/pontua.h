#define TAM_NAME 15
#define  MAX_PILOT 100

typedef struct Scores{
	char pil[TAM_NAME];
	int pts;
	bool bateu;
} Scores;

void initPiloto(Scores *piloto);
void registraPontos(Scores *gamer);
int consultaSave(Scores result[]);