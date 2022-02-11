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
	int x,y; //posição x e y
	int r;   //raio
	int v;   //velocidade
	ALLEGRO_COLOR cor;
} Inimigo;

//responsável por criar um inimigo novo
void criaInimigo(Inimigo *inimigos, int id){

	Inimigo inimigo;

	inimigo.r=MIN_TAM_INIMIGOS+rand()%20;                              //tamanho mínimo definido por MIN_TAM_INIMIGOS e máximo sendo 20
	inimigo.x=960+rand()%960;                                          //poisição mínima tamanho da tela 
	inimigo.y=inimigo.r+rand()%(540-(2*inimigo.r));                    //posição mínima raio e máxima altura da tela menos 2 raios
	inimigo.v=MIN_VEL_INIMIGOS+rand()%4;                               //velocidade mínima definida por MIN_VEL_INIMIGOS e máxima sendo 4
	inimigo.cor=al_map_rgb(192+rand()%64,192+rand()%64,192+rand()%64);

	inimigos[id]=inimigo;// adiciona novo inimigo a lista 	
}

//responsável por iniciar a lista de inimigos
void initInimigo(Inimigo *inimigos){
	for(int i=0; i<MAX_INIMIGOS; i++){
		criaInimigo(inimigos,i);
	}
}

//responsável por destruir inimigos específicados
void destroiInimigo(Inimigo *inimigos,int id){
	criaInimigo(inimigos,id);// ao criar um inimigo na mesma posição do anterior, ele é sobrescrito
}

//responsável por atualizar a posição dos inimigos, dectar colisões entre inimigos
void atualizaInimigo(Inimigo *inimigos){
	for(int i=0;i<MAX_INIMIGOS;i++){
		
		//atualiza posição
		inimigos[i].x-=inimigos[i].v;

		//detecta se o inimigo chegou ao final da tela
		if((inimigos[i].x+inimigos[i].r)<=0){
			destroiInimigo(inimigos,i);
		}

		//COLISÃO INIMIGOS | INIMIGOS

		int linha_sup,linha_inf,linha_dir,linha_esq,x_esquerda,x_direita,y_superior,y_inferiro;
	
		for(int k=0;k<MAX_INIMIGOS;k++){		
			
			//posição das extremidades do inimigo 1
			linha_sup=inimigos[i].y-inimigos[i].r;
			linha_inf=inimigos[i].y+inimigos[i].r;
			linha_dir=inimigos[i].x+inimigos[i].r;		
			linha_esq=inimigos[i].x-inimigos[i].r;

			//posição das extremidades do inimigo 2
			y_superior=inimigos[k].y-inimigos[k].r;
			y_inferiro=inimigos[k].y+inimigos[k].r;
			x_esquerda=inimigos[k].x-inimigos[k].r;
			x_direita=inimigos[k].x+inimigos[k].r;
			
			if(i!=k){//não detectar colisão consigo mesmo
			
				//TESTE O IGUAL AQUI <= e >=
 				if(linha_dir<x_direita && linha_dir>x_esquerda){
					//
					if(linha_inf>= y_superior && linha_inf>=y_inferiro){
						//
						if(linha_sup<=y_superior || linha_sup<=y_inferiro){
							//destroi o menor inimigo
							if(inimigos[i].r<inimigos[k].r){
								destroiInimigo(inimigos,i);	
							}else{
								destroiInimigo(inimigos,k);	
							}
								
						}
					}
					//
					if(linha_sup<=y_superior && linha_sup<=y_inferiro){
						//
						if(linha_inf>=y_superior || linha_inf>=y_inferiro){
							//destroi o menor inimigo
							if(inimigos[i].r<inimigos[k].r){
								destroiInimigo(inimigos,i);	
							}else{
								destroiInimigo(inimigos,k);	
							}
						}
					}
					//
					if(linha_sup<=y_superior && linha_inf>=y_inferiro){
						//destroi o menor inimigo
						if(inimigos[i].r<inimigos[k].r){
							destroiInimigo(inimigos,i);	
						}else{
							destroiInimigo(inimigos,k);	
						}
					}
				}
				//
				if(linha_esq>=x_esquerda && linha_esq<=x_direita){
					//
					if(linha_sup>=y_superior && linha_inf<=y_inferiro){
						//destroi o menor inimigo
						if(inimigos[i].r<inimigos[k].r){
							destroiInimigo(inimigos,i);	
						}else{
							destroiInimigo(inimigos,k);	
						}
					}
				}
			}
		}
	}
}

//responsável por desenhar os inimigos
void desenhaInimigo(Inimigo *inimigos){
	for(int i=0;i<MAX_INIMIGOS;i++){
		al_draw_filled_circle(inimigos[i].x,inimigos[i].y,inimigos[i].r,inimigos[i].cor);
	}

}
