#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "libraries/inimigo.h"

const int NAVE_W=100;
const int NAVE_H=50;
const int VELOCIDADE_NAVE=4;


typedef struct Nave{

	int w,h;
	int posi_x,posi_y;
	int dir_x,dir_y;
	int vel;
	ALLEGRO_COLOR cor;

} Nave;


void initNave(Nave *nave, int altura_tela){

	(*nave).w=NAVE_W;
	(*nave).h=NAVE_H;
	(*nave).posi_x=10+(*nave).w;
	(*nave).posi_y= altura_tela/2;
	(*nave).dir_x=0;
	(*nave).dir_y=0;
	(*nave).vel=VELOCIDADE_NAVE;
	(*nave).cor=al_map_rgb(192+rand()%64,192+rand()%64,192+rand()%64);
	
}

void desenhaNave(Nave nave){

	int x=nave.posi_x,y=nave.posi_y;

	al_draw_filled_triangle(x,y,x-NAVE_W,y-NAVE_H/2,x-NAVE_W,y+NAVE_H/2,nave.cor);

}

bool atualizaNave(Nave *nave, Inimigo *inimigos){

	int soma_x=(*nave).posi_x+((*nave).dir_x*(*nave).vel);
	int soma_y=(*nave).posi_y+((*nave).dir_y*(*nave).vel);

	if(!(soma_x>=958 || (soma_x-NAVE_W)<=2 || (soma_y+NAVE_H/2)>=538 || (soma_y-NAVE_H/2)<=2 )){
		(*nave).posi_x=soma_x;
		(*nave).posi_y=soma_y;
	}

	int base_A,base_B;
	int x_direita,x_esquerda,y_superior,y_inferiro;

	base_A=((*nave).posi_y-((*nave).h)/2);
	base_B=((*nave).posi_y+((*nave).h/2));

	for(int i=0; i<30;i++){

		y_superior=inimigos[i].y-inimigos[i].r;
		y_inferiro=inimigos[i].y+inimigos[i].r;
		x_direita=inimigos[i].x-inimigos[i].r;
		x_esquerda=inimigos[i].x+inimigos[i].r;


		if((*nave).posi_x>=x_direita && (*nave).posi_x<x_esquerda){
			
			if(base_B>y_superior && base_A<y_inferiro){
				if((*nave).posi_x<x_esquerda && ((*nave).posi_x-x_direita)>=0){
					
					return false;
				}

			}
			if(base_B> y_superior&& base_B>y_inferiro){
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
			(*nave).posi_x>=x_esquerda && ((*nave).posi_x-x_esquerda)<=(*nave).w){
			
			return false;
		}

	}

	return true;
}