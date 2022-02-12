#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

//bibliotecas particulares
#include "libraries/inimigo.h"
#include "libraries/nave.h"
#include "libraries/tiro.h"


const int VELOCIDADE_BLOCO=2;

typedef struct Bloco{
	int x,y;    //define a posição x e y do bloco
	int w,h;    //define sua largura e altura
	ALLEGRO_COLOR cor;
} Bloco;

//inicia o bloco principal
void initBloco(Bloco *bloco, int altura_tela, int largura_tela){
	
	(*bloco).x=largura_tela+rand()%(largura_tela);       //a posição inicial será de no mínimo o tamanho da tela e o máximo a largura de duas telas  
	(*bloco).y=rand()%(3*altura_tela/5);                 //a posição inicial será de no mínimo 0 e no máximo 3/5 da altura da tela
	(*bloco).w=largura_tela+rand()%(largura_tela);       //o minimo de largura é o tamnaho da tela, e o máximo 2 vezes a tela
	(*bloco).h=(altura_tela/5)+rand()%(3*altura_tela/5); //o minimo de altura é 1/5 da altura da tela, e o máximo 3/5 da altura da tela
	(*bloco).cor=al_map_rgb(rand(),rand(),rand());       //sem regras para cores de blocos

}

//responsável por atualizar a posição do bloco e identificar a colisão com um bloco, inimigo ou nave 
bool atualizaBloco(Bloco *bloco, int altura_tela, int largura_tela, Nave *nave, Tiro *tiros, int *qtd_tiros, Inimigo *inimigos){	

	// MOVE BLOCO
	(*bloco).x-=VELOCIDADE_BLOCO;


	// COLISÃO NAVE
	int base_A,base_B;
	int x_esquerda,x_direita,y_superior,y_inferiro,bico_nave;

	//posição das extremidades da nave
	base_A=(*nave).posi_y-((*nave).h/2);
	base_B=(*nave).posi_y+((*nave).h/2);
	bico_nave=(*nave).posi_x;

	//posições de extremidades do bloco
	y_superior=(*bloco).y;
	y_inferiro=(*bloco).y+(*bloco).h;
	x_esquerda=(*bloco).x;
	x_direita=(*bloco).x+(*bloco).w;

	
	//considera que o bico da nave já passou a parte frontal do bloco, mas não a parte traseira
	if(bico_nave>=x_esquerda && bico_nave<x_direita){
		
		//considera que a nave esteja entre a parte superior e inferior do bloco
		if(base_A>y_superior && base_B<y_inferiro){
			//considera um choque frontal entre a nave e o bloco
			if(bico_nave>x_esquerda && (bico_nave-x_esquerda)>=0){
				return false;
			}

		}
		//consideral a nave abaixo do bloco
		if(base_B> y_superior && base_B>y_inferiro){
			//considera um choque lateral inferiro
			if(base_A<y_superior || base_A<y_inferiro){
					return false;
			}
		}
		//consideral a nave acima do bloco
		if(base_A<y_superior && base_A<y_inferiro){
			//considera um choque lateral superior
			if(base_B>y_superior || base_B>y_inferiro){
					return false;
			}
		}

	}
	//considera que a neve esta entre a base superio e inferios do bloco e que a traseira da nave se chocou com o bloco
	if(base_B>y_superior && base_A<y_inferiro && 
	   bico_nave>=x_direita && (bico_nave-x_direita)<=(*nave).w){
		return false;
	}

	//COLISÃO TIROS
	int c=0;//contador
	int linha_sup,linha_inf,linha_dir,linha_esq;

	while(tiros[c].x!=1000){

		//posição das extremidades do tiro
		linha_sup=tiros[c].y-tiros[c].r;
		linha_inf=tiros[c].y+tiros[c].r;
		linha_dir=tiros[c].x+tiros[c].r;		
		linha_esq=tiros[c].x-tiros[c].r;
		
		//considera os tiros que já passaram a parte frontal do bloco, mas não a parte traseira
		if(linha_dir>=x_esquerda && linha_dir<x_direita){
			//considera o tiro abaixo do bloco
			if(linha_inf> y_superior && linha_inf>y_inferiro){
				//considera um choque lateral inferior
				if(linha_sup<y_superior || linha_sup<y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
				}
			}
			//considera o tiro acima do bloco
			if(linha_sup<y_superior && linha_sup<y_inferiro){
				//considera um choque lateral superior
				if(linha_inf>y_superior || linha_inf>y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
				}
			}
			//considera o tiro entre a parte superior e inferior do bloco
			if(linha_inf>y_superior && linha_sup<y_inferiro){
						destroiTiro(tiros,c,qtd_tiros);
			}
		}
		c++;
	}


	//COLISÃO INIMIGOS

	for(int i=0;i<30;i++){		
		
		//posição das extremidades do inimigo	
		linha_sup=inimigos[i].y-inimigos[i].r;
		linha_inf=inimigos[i].y+inimigos[i].r;
		linha_dir=inimigos[i].x+inimigos[i].r;		
		linha_esq=inimigos[i].x-inimigos[i].r;
		
		//considera o inimigo entre a extremidade direita e a esquerda do bloco
		if(linha_dir<x_direita && linha_dir>x_esquerda){
			//considera o inimigo abaixo do bloco
			if(linha_inf>= y_superior && linha_inf>=y_inferiro){
				//considera um choque lateral inferior
				if(linha_sup<=y_superior || linha_sup<=y_inferiro){
						destroiInimigo(inimigos,i);
				}
			}
			//considera o inimigo acima do bloco
			if(linha_sup<=y_superior && linha_sup<=y_inferiro){
				//considera um choque lateral inferior
				if(linha_inf>=y_superior || linha_inf>=y_inferiro){
						destroiInimigo(inimigos,i);
				}
			}
			//considera o inimigo entre a linha superior e inferior
			if(linha_sup>=y_superior && linha_inf<=y_inferiro){
						destroiInimigo(inimigos,i);
			}
		}
		//considera o inimigo entre a extremidade esquerda e direita do bloco
		if(linha_esq>=x_esquerda && linha_esq<=x_direita){
			//considera um choque traseiro com o bloco
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

//responsável por desenhar o bloco
void desenhaBloco(Bloco bloco){
	
	int x=bloco.x,y=bloco.y;

	al_draw_filled_rectangle(x,y,x+bloco.w,y+bloco.h,bloco.cor);

}
