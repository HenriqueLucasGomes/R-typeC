typedef struct Bloco{
	int x,y;
	int w,h;
	ALLEGRO_COLOR cor;
} Bloco;

void initBloco(Bloco *bloco, int altura_tela, int largura_tela);
bool atualizaBloco(Bloco *bloco, int altura_tela, int largura_tela, Nave *nave, Tiro *tiros, int *qtd_tiros, Inimigo *inimigos);
void desenhaBloco(Bloco bloco);
