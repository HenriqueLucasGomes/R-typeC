typedef struct Tiro{
	int x,y;
	int r;
	ALLEGRO_COLOR cor;
} Tiro;

void initTiros(Tiro *tiros);
void destroiTiro(Tiro *tiros, int id, int *qtd_tiros);
int controleTiros(Tiro *tiros, int id, int *qtd_tiros);

void atiraTiro(Nave nave,Tiro *tiros,int *qtd_tiros,int *tam_tiro,bool *permission);
void desenhaCarregador(Nave *nave, int tam_tiro);
void atualizaTiro(Tiro *tiros, int *qtd_tiros, Inimigo *inimigos,Nave *nave,int *pts);
 