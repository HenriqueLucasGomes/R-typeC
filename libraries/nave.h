
typedef struct Nave{

	int w,h;
	int posi_x,posi_y;
	int dir_x,dir_y;
	int vel;
	ALLEGRO_COLOR cor;

} Nave;


void initNave(Nave *nave, int altura_tela);
void desenhaNave(Nave nave);
bool atualizaNave(Nave *nave,Inimigo *inimigos);
