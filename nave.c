#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

//bibliotecas particulares
#include "libraries/inimigo.h"

const int NAVE_W=100;
const int NAVE_H=50;
const int VELOCIDADE_NAVE=4;

typedef struct Nave{
	int w,h;                     //define sua largura e altura
	int posi_x,posi_y;           //define sua posição x e y do ponto principal
	int dir_x,dir_y;             //define sua posição x e y do ponto secundário
	int vel;                     //define sua velociadade
	ALLEGRO_COLOR cor;
} Nave;

//inicia a nave
void initNave(Nave *nave, int altura_tela){

	(*nave).w=NAVE_W;              //largura da nave
	(*nave).h=NAVE_H;              //altura da nave
	(*nave).posi_x=10+(*nave).w;   //inicia a 10 pixels de distância da borda
	(*nave).posi_y= altura_tela/2; //inicia a nave no meio da tela
	(*nave).dir_x=0;               //inicia sem sem movimento
	(*nave).dir_y=0;               //inicia sem sem movimento
	(*nave).vel=VELOCIDADE_NAVE;   //define a velocidade padrão da nave
	(*nave).cor=al_map_rgb(192+rand()%64,192+rand()%64,192+rand()%64);
	
}

//responsável por desenhar a nave
void desenhaNave(Nave nave){
	int x=nave.posi_x,y=nave.posi_y;
	al_draw_filled_triangle(x,y,x-NAVE_W,y-NAVE_H/2,x-NAVE_W,y+NAVE_H/2,nave.cor);
}

//responsável por atualizar a posição da nave e detectar colisões com inimigos
bool atualizaNave(Nave *nave, Inimigo *inimigos){

	//atualiza as posições da nave
	int soma_x=(*nave).posi_x+((*nave).dir_x*(*nave).vel);
	int soma_y=(*nave).posi_y+((*nave).dir_y*(*nave).vel);

	//limita a movimentação da nave
	if(!(soma_x>=958 || (soma_x-NAVE_W)<=2 || (soma_y+NAVE_H/2)>=538 || (soma_y-NAVE_H/2)<=2 )){
		(*nave).posi_x=soma_x;
		(*nave).posi_y=soma_y;
	}

	int base_A,base_B;
	int x_esquerda,x_direita,y_superior,y_inferiro,bico_nave;

	//posição das extremidades da nave
	base_A=((*nave).posi_y-((*nave).h)/2);
	base_B=((*nave).posi_y+((*nave).h/2));
	bico_nave=(*nave).posi_x;

	//percorre a lista de inimigos
	for(int i=0; i<30;i++){

		//posição das extremidades dos inimigos
		y_superior=inimigos[i].y-inimigos[i].r;
		y_inferiro=inimigos[i].y+inimigos[i].r;
		x_direita=inimigos[i].x+inimigos[i].r;
		x_esquerda=inimigos[i].x-inimigos[i].r;

		//considera que o bico da nave está entre a parte frontal e traseira do inimigo
		if(bico_nave>=x_esquerda && bico_nave<x_direita){
			//considera que a nave esteja entre a parte superior e inferior do inimigo
			if(base_B>y_superior && base_A<y_inferiro){
				//considera um choque frontal
				if(bico_nave<x_direita && (bico_nave-x_esquerda)>=0){					
					return false;
				}

			}
			//considera que a nave esteja  na parte inferior do inimigo
			if(base_B> y_superior&& base_B>y_inferiro){
				//considera um choque lateral inferior
				if(base_A<y_superior || base_A<y_inferiro){		
						return false;
				}
			}
			//considera que a nave esteja na parte superior do inimigo
			if(base_A<y_superior && base_A<y_inferiro){
				//considera um choque lateral superior
				if(base_B>y_superior || base_B>y_inferiro){		
						return false;
				}
			}

		}
		//considera um choque traseiro 
		if(base_B>y_superior && base_A<y_inferiro && 
			bico_nave>=x_direita && (bico_nave-x_direita)<=(*nave).w){	
			return false;
		}
	}
	return true;
}