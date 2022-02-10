#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "libraries/inimigo.h"
#include "libraries/nave.h"

const int VELOCIDADE_TIRO=10;
const int RAIO_PEQUENO=3;
const int RAIO_GRANDE=8;
const int TAXA_PTS=5;

typedef struct Tiro{

	int x,y;
	int r;
	ALLEGRO_COLOR cor;

} Tiro;


void initTiros(Tiro *tiros){

	//Tiro tiro;
	for(int i=0; i<100; i++){
		tiros[i].x=1000;
		tiros[i].y=1000;
		tiros[i].r=RAIO_PEQUENO;
		tiros[i].cor=al_map_rgb(255,0,0);
		// tiros[i]=&tiro;
	}

}

void destroiTiro(Tiro *tiros, int id, int *qtd_tiros){
	
	while(tiros[id].x!=1000){
		tiros[id]=tiros[id+1];
		id++;
	}

	*qtd_tiros-=1;	
}

int controleTiros(Tiro *tiros, int id, int *qtd_tiros){
	
	if(tiros[id].x==1000){
		return 100;
	}

	// 0 -> TIRO MAIS DISTANTE
	destroiTiro(tiros,0,qtd_tiros);

	// for(int i=0;i<100;i++){
	// 	printf("\n%i: %i\n",i, tiros[i].x);
	// }

	return id;
}


void tiroPequeno(Nave nave,Tiro *tiros,int *qtd_tiros,int *tam_tiro,bool *permission){


	
	
	if(*tam_tiro>=4 && *permission){

		Tiro tiro;
		tiro.x=nave.posi_x+RAIO_PEQUENO;
		tiro.y=nave.posi_y;
		tiro.r=RAIO_PEQUENO+(*tam_tiro/2);
		tiro.cor=al_map_rgb(255,0,0);
		
		tiros[*qtd_tiros]=tiro;
		*qtd_tiros+=1;
		*tam_tiro=3;
		*permission=false;
	}
	

}

void desenhaCarregador(Nave *nave, int tam_tiro){
	int r=RAIO_PEQUENO+(tam_tiro/2);
	al_draw_filled_circle((*nave).posi_x+r,(*nave).posi_y,r,al_map_rgb(255,0,0));
}

void atualizaTiro(Tiro *tiros, int *qtd_tiros, Inimigo *inimigos,Nave *nave,int *pts){

	int linha_sup,linha_inf,linha_dir,linha_esq,x_direita,x_esquerda,y_superior,y_inferiro;

	// if(*qtd_tiros>0){
	// 	al_draw_filled_circle((*nave).posi_x,(*nave).posi_y,4,tiros[1].cor);
	// }


	
	for(int i=0;i<100;i++){
		if(tiros[i].x<960){
			tiros[i].x+=VELOCIDADE_TIRO;
			al_draw_filled_circle(tiros[i].x,tiros[i].y,tiros[i].r,tiros[i].cor);	

			for(int k=0;k<30;k++){
				
				linha_sup=tiros[i].y-tiros[i].r;
				linha_inf=tiros[i].y+tiros[i].r;
				linha_dir=tiros[i].x+tiros[i].r;		
				linha_esq=tiros[i].x-tiros[i].r;
				

				y_superior=inimigos[k].y-inimigos[k].r;
				y_inferiro=inimigos[k].y+inimigos[k].r;
				x_direita=inimigos[k].x-inimigos[k].r;
				x_esquerda=inimigos[k].x+inimigos[k].r;
				

				
				if(linha_dir>=x_direita){
					if(linha_inf> y_superior && linha_inf>y_inferiro){
						if(linha_sup<y_superior || linha_sup<y_inferiro){
								*pts+=TAXA_PTS*inimigos[k].r;
								destroiInimigo(inimigos,k);
								if(tiros[i].r!=RAIO_PEQUENO+15){
									destroiTiro(tiros,i,qtd_tiros);	
								}
								
						}
					}
					if(linha_sup<y_superior && linha_sup<y_inferiro){
						if(linha_inf>y_superior || linha_inf>y_inferiro){
								*pts+=TAXA_PTS*inimigos[k].r;
								destroiInimigo(inimigos,k);
								if(tiros[i].r!=RAIO_PEQUENO+15){
									destroiTiro(tiros,i,qtd_tiros);	
								}
						}
					}
					if(linha_inf>y_superior && linha_sup<y_inferiro){
								*pts+=TAXA_PTS*inimigos[k].r;
								destroiInimigo(inimigos,k);
								if(tiros[i].r!=RAIO_PEQUENO+15){
									destroiTiro(tiros,i,qtd_tiros);	
								}
					}
				}
			}
		

		}else{
			i=controleTiros(tiros,i,qtd_tiros);
			
		}
		

	}

}

