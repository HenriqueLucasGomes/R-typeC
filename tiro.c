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

	int x,y;    //define sua posição x e y
	int r;      //define seu raio
	ALLEGRO_COLOR cor;

} Tiro;

//responsável por iniciar a lista de tiros
void initTiros(Tiro *tiros){

	for(int i=0; i<100; i++){
		tiros[i].x=1000;                  //valor default usado para identificar um tiro não registrado
		tiros[i].y=1000;                  //valor default usado para identificar um tiro não registrado
		tiros[i].r=RAIO_PEQUENO;          //todos os tiros iniciam com a cor vermelha
		tiros[i].cor=al_map_rgb(255,0,0); //todos os tiros possuem cor vermelha
	}

}

//responsável por iniciar a lista de tiros
void destroiTiro(Tiro *tiros, int id, int *qtd_tiros){
	
	//sobrescreve o tiro a ser eliminado com o pŕoximo item da lista até o fim da mesma
	while(tiros[id].x!=1000){//o valor 1000 indica o fim da lista de tiros com um tiro nulo
		tiros[id]=tiros[id+1];
		id++;
	}

	*qtd_tiros-=1;	
}

//responsável por identificar se o tiro em questão chegou ao fim da tela po se é um tiro nulo que representa o final da lista
int controleTiros(Tiro *tiros, int id, int *qtd_tiros){

	//caso seja um tiro nulo	
	if(tiros[id].x==1000){
		return 100;
	}

	//a posição 0 sempre será o tiro mais distante da lista
	destroiTiro(tiros,0,qtd_tiros);

	return id;
}

//responsável registrar o tiro
void atiraTiro(Nave nave,Tiro *tiros,int *qtd_tiros,int *tam_tiro,bool *permission){

	//registrar tiros com mais de 4 de raio e se for permitido
	if(*tam_tiro>=4 && *permission){


		//cria novo tiro
		Tiro tiro;
		tiro.x=nave.posi_x+RAIO_PEQUENO;
		tiro.y=nave.posi_y;
		tiro.r=RAIO_PEQUENO+(*tam_tiro/2);
		tiro.cor=al_map_rgb(255,0,0);
		
		//registra novo tiro na lista
		tiros[*qtd_tiros]=tiro;
		
		*qtd_tiros+=1;
		*tam_tiro=3;
		*permission=false;
	}
	

}

//desenha o carregamento do tiro carregado
void desenhaCarregador(Nave *nave, int tam_tiro){
	int r=RAIO_PEQUENO+(tam_tiro/2);
	al_draw_filled_circle((*nave).posi_x+r,(*nave).posi_y,r,al_map_rgb(255,0,0));
}

//responsável por atualizar a posição dos tiros, detectar inimigos atingidos e eliminar tiros 
void atualizaTiro(Tiro *tiros, int *qtd_tiros, Inimigo *inimigos,Nave *nave,int *pts){

	int linha_sup,linha_inf,linha_dir,linha_esq,x_direita,x_esquerda,y_superior,y_inferiro;

	//faz a leitura de toda a lista
	for(int i=0;i<100;i++){
		if(tiros[i].x<960){//lê apenas tiros dentro da tela
			
			//atualiza a posição de cada tiro
			tiros[i].x+=VELOCIDADE_TIRO;
			al_draw_filled_circle(tiros[i].x,tiros[i].y,tiros[i].r,tiros[i].cor);	

			for(int k=0;k<30;k++){//detecta se aquele tiro atingiu algum inimigo
				
				//posição das extremidades do tiro
				linha_sup=tiros[i].y-tiros[i].r;
				linha_inf=tiros[i].y+tiros[i].r;
				linha_dir=tiros[i].x+tiros[i].r;		
				linha_esq=tiros[i].x-tiros[i].r;
				
				//posição das extremidades dos inimigos
				y_superior=inimigos[k].y-inimigos[k].r;
				y_inferiro=inimigos[k].y+inimigos[k].r;
				x_direita=inimigos[k].x+inimigos[k].r;
				x_esquerda=inimigos[k].x-inimigos[k].r;
				
				//considera que o tiro esteja entre as linhas direita e esquerda do inimigo   
				if(linha_dir>=x_esquerda && linha_esq<=x_direita){
					//considera que o tiro esteja na parte inferior do inimigo
					if(linha_inf> y_superior && linha_inf>y_inferiro){
						//considera um choque lateral inferior
						if(linha_sup<y_superior || linha_sup<y_inferiro){
							//atualiza s pontos com base no tamanho do inimigo
							*pts+=TAXA_PTS*inimigos[k].r;
							
							//destroi o inimigo
							destroiInimigo(inimigos,k);
							
							//se o tiro for um tiro totalmente carregado ele não é eliminado
							if(tiros[i].r!=RAIO_PEQUENO+15){
								destroiTiro(tiros,i,qtd_tiros);	
							}								
						}
					}
					//considera que o tiro esteja na parte superior do inimigo
					if(linha_sup<y_superior && linha_sup<y_inferiro){
						//considera um choque superior inferior
						if(linha_inf>y_superior || linha_inf>y_inferiro){
							//atualiza s pontos com base no tamanho do inimigo
							*pts+=TAXA_PTS*inimigos[k].r;
							
							//destroi o inimigo
							destroiInimigo(inimigos,k);
							
							//se o tiro for um tiro totalmente carregado ele não é eliminado
							if(tiros[i].r!=RAIO_PEQUENO+15){
								destroiTiro(tiros,i,qtd_tiros);	
							}
						}
					}
					//considera que o tiro esteja entre a parte inferior e superior do inimigo
					if(linha_inf>y_superior && linha_sup<y_inferiro){
						//atualiza s pontos com base no tamanho do inimigo
						*pts+=TAXA_PTS*inimigos[k].r;
							
						//destroi o inimigo
						destroiInimigo(inimigos,k);
							
						//se o tiro for um tiro totalmente carregado ele não é eliminado
						if(tiros[i].r!=RAIO_PEQUENO+15){
							destroiTiro(tiros,i,qtd_tiros);	
						}	
					}
				}
			}
		}else{
			//encerra a leitura da lista ou elimina o tiro mais distante
			i=controleTiros(tiros,i,qtd_tiros);		
		}
	}
}

