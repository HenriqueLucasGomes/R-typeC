#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "libraries/inimigo.h"
#include "libraries/nave.h"
#include "libraries/tiro.h"


const int VELOCIDADE_BLOCO=2;

typedef struct Bloco{
	int x,y;
	int w,h;
	ALLEGRO_COLOR cor;
} Bloco;


void initBloco(Bloco *bloco, int altura_tela, int largura_tela){
	
	(*bloco).x=largura_tela+rand()%(largura_tela);
	(*bloco).y=rand()%(3*altura_tela/5);
	(*bloco).w=largura_tela+rand()%(largura_tela);
	(*bloco).h=altura_tela/5+rand()%(3*altura_tela/5);
	(*bloco).cor=al_map_rgb(rand(),rand(),rand());

}

 
bool atualizaBloco(Bloco *bloco, int altura_tela, int largura_tela, Nave *nave, Tiro *tiros, int *qtd_tiros, Inimigo *inimigos){	

	// MOVE BLOCO
	(*bloco).x-=VELOCIDADE_BLOCO;



	// COLISÃO NAVE
	int base_A,base_B;
	int x_esquerda,x_direita,y_superior,y_inferiro,bico_nave;

	base_A=(*nave).posi_y-((*nave).h/2);
	base_B=(*nave).posi_y+((*nave).h/2);
	bico_nave=(*nave).posi_x;

	y_superior=(*bloco).y;
	y_inferiro=(*bloco).y+(*bloco).h;
	x_esquerda=(*bloco).x;
	x_direita=(*bloco).x+(*bloco).w;

	

	if(bico_nave>=x_esquerda && bico_nave<x_direita){
		
		if(base_A>y_superior && base_B<y_inferiro){
			if(bico_nave>x_esquerda && (bico_nave-x_esquerda)>=0){
				return false;
			}

		}
		if(base_B> y_superior && base_B>y_inferiro){
			if(base_A<y_superior || base_A<y_inferiro){

					return false;
			}
		}
		if(base_A<y_superior && base_A<y_inferiro){
			if(base_B>y_superior || base_B>y_inferiro){

					return false;
			}
		}

	}
	if(base_B>y_superior && base_A<y_inferiro && 
		(*nave).posi_x>=x_direita && ((*nave).posi_x-x_direita)<=(*nave).w){

		return false;
	}

	//COLISÃO TIROS
	int c=0;
	int linha_sup,linha_inf,linha_dir,linha_esq;//x_esquerda,x_direita,y_superior,y_inferiro;

	while(tiros[c].x!=1000){
		
		// linha_sup=tiros[c].y-tiros[c].r;
		// linha_inf=tiros[c].y+tiros[c].r;
		// linha_dir=tiros[c].x+tiros[c].r;


		linha_sup=tiros[c].y-tiros[c].r;
		linha_inf=tiros[c].y+tiros[c].r;
		linha_dir=tiros[c].x+tiros[c].r;		
		linha_esq=tiros[c].x-tiros[c].r;

		// y_superior=inimigos[k].y-inimigos[k].r;
		// y_inferiro=inimigos[k].y+inimigos[k].r;
		// x_esquerda=inimigos[k].x-inimigos[k].r;
		// x_direita=inimigos[k].x+inimigos[k].r;

		
		if(linha_dir>=x_esquerda && linha_dir<x_direita){
			if(linha_inf> y_superior && linha_inf>y_inferiro){
				if(linha_sup<y_superior || linha_sup<y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
				}
			}
			if(linha_sup<y_superior && linha_sup<y_inferiro){
				if(linha_inf>y_superior || linha_inf>y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
				}
			}
			if(linha_inf>y_superior && linha_sup<y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
			}
		}
		c++;
	}

	//COLISÃO INIMIGOS
	// int linha_sup,linha_inf,linha_dir,linha_esq;


	for(int i=0;i<30;i++){		
			
		linha_sup=inimigos[i].y-inimigos[i].r;
		linha_inf=inimigos[i].y+inimigos[i].r;
		linha_dir=inimigos[i].x+inimigos[i].r;		
		linha_esq=inimigos[i].x-inimigos[i].r;
		
		
		if(linha_dir<x_direita && linha_dir>(x_esquerda)){
			if(linha_inf>= y_superior && linha_inf>=y_inferiro){
				if(linha_sup<=y_superior || linha_sup<=y_inferiro){
						destroiInimigo(inimigos,i);
				}
			}
			if(linha_sup<=y_superior && linha_sup<=y_inferiro){
				if(linha_inf>=y_superior || linha_inf>=y_inferiro){
						destroiInimigo(inimigos,i);
				}
			}
			if(linha_sup<=y_superior && linha_inf>=y_inferiro){
						destroiInimigo(inimigos,i);
			}
		}
		if(linha_esq>=x_esquerda && linha_esq<=x_direita){
			if(linha_sup>=y_superior && linha_inf<=y_inferiro){
				destroiInimigo(inimigos,i);
			}
		}

	}



	// CRIA NOVO BLOCO
	if((*bloco).x+(*bloco).w<=0){
		initBloco(bloco,altura_tela,largura_tela);
	}


	return true;

}

void desenhaBloco(Bloco bloco){
	
	int x=bloco.x,y=bloco.y;

	al_draw_filled_rectangle(x,y,x+bloco.w,y+bloco.h,bloco.cor);

}
