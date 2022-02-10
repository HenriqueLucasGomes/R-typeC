typedef struct Inimigo{
	int x,y;
	int r;
	int v;
	ALLEGRO_COLOR cor;
} Inimigo;

void criaInimigo(Inimigo *inimigos, int id);
void initInimigo(Inimigo *inimigos);
void destroiInimigo(Inimigo *inimigos,int id);
void atualizaInimigo();
void desenhaInimigo(Inimigo *inimigos);