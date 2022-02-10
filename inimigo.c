#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


const int MAX_INIMIGOS=30;
const int MIN_TAM_INIMIGOS=10;
const int MIN_VEL_INIMIGOS=1;


typedef struct Inimigo{
	int x,y;
	int r;
	int v;
	ALLEGRO_COLOR cor;
} Inimigo;

void criaInimigo(Inimigo *inimigos, int id){

	Inimigo inimigo;

	inimigo.r=MIN_TAM_INIMIGOS+rand()%20;
	inimigo.x=960+rand()%960;
	inimigo.y=inimigo.r+rand()%(540-(2*inimigo.r));
	inimigo.v=MIN_VEL_INIMIGOS+rand()%4;
	inimigo.cor=al_map_rgb(192+rand()%64,192+rand()%64,192+rand()%64);


	inimigos[id]=inimigo;
	
	
}

void initInimigo(Inimigo *inimigos){

	for(int i=0; i<MAX_INIMIGOS; i++){
		criaInimigo(inimigos,i);
	}

}

void destroiInimigo(Inimigo *inimigos,int id){

	criaInimigo(inimigos,id);

}

void atualizaInimigo(Inimigo *inimigos){
	for(int i=0;i<MAX_INIMIGOS;i++){
		inimigos[i].x-=inimigos[i].v;
		if((inimigos[i].x+inimigos[i].r)<=0){
			destroiInimigo(inimigos,i);
		}

		int linha_sup,linha_inf,linha_dir,linha_esq,x_esquerda,x_direita,y_superior,y_inferiro;
	
		for(int k=0;k<MAX_INIMIGOS;k++){
		
			
			linha_sup=inimigos[i].y-inimigos[i].r;
			linha_inf=inimigos[i].y+inimigos[i].r;
			linha_dir=inimigos[i].x+inimigos[i].r;		
			linha_esq=inimigos[i].x-inimigos[i].r;

			y_superior=inimigos[k].y-inimigos[k].r;
			y_inferiro=inimigos[k].y+inimigos[k].r;
			x_esquerda=inimigos[k].x-inimigos[k].r;
			x_direita=inimigos[k].x+inimigos[k].r;
			
			if(i!=k){
			
				if(linha_dir<x_direita && linha_dir>(x_esquerda)){
					if(linha_inf>= y_superior && linha_inf>=y_inferiro){
						if(linha_sup<=y_superior || linha_sup<=y_inferiro){
								if(i<k){
									destroiInimigo(inimigos,i);	
								}else{
									destroiInimigo(inimigos,k);	
								}
								
						}
					}
					if(linha_sup<=y_superior && linha_sup<=y_inferiro){
						if(linha_inf>=y_superior || linha_inf>=y_inferiro){
								if(i<k){
									destroiInimigo(inimigos,i);	
								}else{
									destroiInimigo(inimigos,k);	
								}
						}
					}
					if(linha_sup<=y_superior && linha_inf>=y_inferiro){
								if(i<k){
									destroiInimigo(inimigos,i);	
								}else{
									destroiInimigo(inimigos,k);	
								}
					}
				}
				if(linha_esq>=x_esquerda && linha_esq<=x_direita){
					if(linha_sup>=y_superior && linha_inf<=y_inferiro){
						if(i<k){
							destroiInimigo(inimigos,i);	
						}else{
							destroiInimigo(inimigos,k);	
						}
					}
				}
			}

		}//esse
	}
}

void desenhaInimigo(Inimigo *inimigos){

	for(int i=0;i<MAX_INIMIGOS;i++){
		al_draw_filled_circle(inimigos[i].x,inimigos[i].y,inimigos[i].r,inimigos[i].cor);
	}

}
