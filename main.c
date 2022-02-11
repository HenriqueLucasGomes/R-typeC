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
#include "libraries/bloco.h"
#include "libraries/pontua.h"

const float FPS = 100;  
const int SCREEN_W = 960;//largura
const int SCREEN_H = 540;//altura

ALLEGRO_FONT *FONTE;

//structs
Tiro tiros[100];
Inimigo inimigos[30];
Scores result[MAX_PILOT];
Scores jogador_atual;
Nave nave;
Bloco bloco;



void initGlobais(){
	initTiros(tiros);
	initNave(&nave,SCREEN_H);
	initBloco(&bloco,SCREEN_H,SCREEN_W);
	initInimigo(inimigos);
	initPiloto(&jogador_atual);
}

void desenhaCenario(){
	al_clear_to_color(al_map_rgb(64,64,64));
}

//desenha a pontuação na tela
void desenhaScore(int pontos){

	char score[]="Score: ";
	char text[30];

	sprintf(text, "%d", pontos);//transforma pontos em uma string no vetor text
	strcat(score,text);//concatena as informações de score com text
	
	al_draw_text(FONTE,al_map_rgb(200, 200, 200),0,0,0,score);//desenha na tela o score do jogador
	
}

//tela de registro de nome
void telaInicial(){

	al_clear_to_color(al_map_rgb(21, 26, 60));//define uma cor mais cinzenta

	al_draw_filled_rectangle(280,150,280+400,150+240,al_map_rgb(0,0,0));//quadro principal
	al_draw_text(FONTE,al_map_rgb(72, 58, 246),280+65,150+20,0,"Say your pilot name:");//titulo
	al_draw_text(FONTE,al_map_rgb(72, 58, 246),480,270,ALLEGRO_ALIGN_CENTRE,jogador_atual.pil);//nome do piloto


}

//desenha o parabens caso quebre o record
void telaParabens(Scores gamer){

	char text[35];//titulo com parabens
	char rec[]="New record ";
	char pontos[20];

	strcpy(text,"Congratulations ");//copia uma string na outra
	strcat(text,gamer.pil);//copia uma string na outra

	sprintf(pontos, "%d", gamer.pts);//transforma int em uma string no vetor pontos
	strcat(rec,pontos);//concatena uma string a outra
	strcat(rec," !!!");//concatena uma string a outra

	al_clear_to_color(al_map_rgb(21, 26, 60));//quadro principal

	al_draw_filled_rectangle(280,150,280+400,150+240,al_map_rgb(0,0,0));//quadro principal
	al_draw_text(FONTE,al_map_rgb(72, 58, 246),480,170,ALLEGRO_ALIGN_CENTRE,text);//frase de parabens
	al_draw_text(FONTE,al_map_rgb(72, 58, 246),480,270,ALLEGRO_ALIGN_CENTRE,rec);//valor do rec

}

//desenha a tela final com a lista de pontuações
void telaFinal(Scores result[],int tam){
		
		char linha[300];//armazena '.'
		char pontos[20];//amazena os pontos do jogador
		int qtd_esp=240;//limite de '.'
		int cont=0,tam_pil=0,tam_pts=0;// variaveis temporárias
		
		FONTE = al_load_font("fonts/game_over.ttf", 50, 1); 
		al_clear_to_color(al_map_rgb(1,1,1));//define a cor como preto

		//desena na tela todos os jogadores já registrados e seus records
		for (int i=0;i<tam; i++){

			sprintf(pontos, "%d", result[i].pts);//transforma int em uma string no vetor pontos

			tam_pil=strlen(result[i].pil);//tamanho
			tam_pts=strlen(pontos);//tamanho

			//preenche com '.'
			for(int k=0;k<qtd_esp-(5*tam_pil)-(4*tam_pts);k++){//preenche até o limite menos o tamanhos dos outros vetores
				linha[k]='.';
				linha[k+1]='\0';//define o final da string
			}
			
			al_draw_text(FONTE,al_map_rgb(255, 255, 255),5,i*30,0,result[i].pil);//desenha o nome 
			al_draw_text(FONTE,al_map_rgb(255, 255, 255),490,i*30,ALLEGRO_ALIGN_CENTER,linha);//desenha os '.' 
			al_draw_text(FONTE,al_map_rgb(255, 255, 255),955,i*30,ALLEGRO_ALIGN_RIGHT,pontos);//desenha a pontuação

			linha[0]='\0';//redefine o final da string
		}

}

int main(){


	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	
	//inicializa o allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
	//inicializa o módulo de primitivas do Allegro
   	 if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    	}	
	
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	
	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
	timer = al_create_timer(2.0 / FPS);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    FONTE = al_load_font("fonts/game_over.ttf", 70, 1);   
	if(FONTE == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

	
	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	
	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	
	//inicia as variáveis globais
	initGlobais();
	
	// inicia o temporizador
	al_start_timer(timer);


	//variaveis para controle
	int qtd_pilotos=0;  //quantidade de pilotos registrados
	int qtd_tiros=0;    //quantidade de tiros registrados
	int tam_tiro=3;     //controla o tamnho do tiro
	int cont=0;         //controla o tamanho do nome registrado

	
	// int qtd_inimigos=0;

	bool playing = true;   //encerra o jogo
	bool registry = true;  //encerra a tela inicial
	bool final = true;     //encerra a tela final
	bool save = false;     //define se algo a salvar
	bool fim_rec=true;     //encerra a tela de parabens
	bool permission=false; //permite que o tiro seja dado
	bool soma=false;       //permite que o carregamentodo tiro seja feito



	while(registry){

		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			
			//limita o tamanho de nome
			if(cont>=TAM_NAME){
				cont=TAM_NAME;
			}

			//define manualmente o final do nome
			jogador_atual.pil[TAM_NAME]='\0';

			desenhaCenario();
			telaInicial(jogador_atual.pil);

			al_flip_display();
		}


		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing=false;
			registry=false;
			final=false;
		}

		//se o tipo de evento for um pressionar de uma tecla
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_A:
					jogador_atual.pil[cont]='A';
					cont++;
				break;
				case ALLEGRO_KEY_B:
					jogador_atual.pil[cont]='B';
					cont++;
				break;
				case ALLEGRO_KEY_C:
					jogador_atual.pil[cont]='C';
					cont++;
				break;
				case ALLEGRO_KEY_D:
					jogador_atual.pil[cont]='D';
					cont++;
				break;
				case ALLEGRO_KEY_E:
					jogador_atual.pil[cont]='E';
					cont++;
				break;
				case ALLEGRO_KEY_F:
					jogador_atual.pil[cont]='F';
					cont++;
				break;
				case ALLEGRO_KEY_G:
					jogador_atual.pil[cont]='G';
					cont++;
				break;
				case ALLEGRO_KEY_H:
					jogador_atual.pil[cont]='H';
					cont++;
				break;
				case ALLEGRO_KEY_I:
					jogador_atual.pil[cont]='I';
					cont++;
				break;
				case ALLEGRO_KEY_J:
					jogador_atual.pil[cont]='J';
					cont++;
				break;

				case ALLEGRO_KEY_K:
					jogador_atual.pil[cont]='K';
					cont++;
				break;
				case ALLEGRO_KEY_L:
					jogador_atual.pil[cont]='L';
					cont++;
				break;
				case ALLEGRO_KEY_M:
					jogador_atual.pil[cont]='M';
					cont++;
				break;
				case ALLEGRO_KEY_N:
					jogador_atual.pil[cont]='N';
					cont++;
				break;
				case ALLEGRO_KEY_O:
					jogador_atual.pil[cont]='O';
					cont++;
				break;

				case ALLEGRO_KEY_P:
					jogador_atual.pil[cont]='P';
					cont++;
				break;
				case ALLEGRO_KEY_Q:
					jogador_atual.pil[cont]='Q';
					cont++;
				break;
				case ALLEGRO_KEY_R:
					jogador_atual.pil[cont]='R';
					cont++;
				break;
				case ALLEGRO_KEY_S:
					jogador_atual.pil[cont]='S';
					cont++;
				break;
				case ALLEGRO_KEY_T:
					jogador_atual.pil[cont]='T';
					cont++;
				break;

				case ALLEGRO_KEY_U:
					jogador_atual.pil[cont]='U';
					cont++;
				break;
				case ALLEGRO_KEY_V:
					jogador_atual.pil[cont]='V';
					cont++;
				break;
				case ALLEGRO_KEY_W:
					jogador_atual.pil[cont]='W';
					cont++;
				break;

				case ALLEGRO_KEY_X:
					jogador_atual.pil[cont]='X';
					cont++;
				break;
				case ALLEGRO_KEY_Y:
					jogador_atual.pil[cont]='Y';
					cont++;
				break;
				case ALLEGRO_KEY_Z:
					jogador_atual.pil[cont]='Z';
					cont++;
				break;
				case ALLEGRO_KEY_SPACE:
					jogador_atual.pil[cont]=' ';
					cont++;
				break;
				case ALLEGRO_KEY_BACKSPACE:
					cont--;
					jogador_atual.pil[cont]='\0';
				break;
				case ALLEGRO_KEY_ENTER:
					//indica que alguém foi registrado
					save=true;
					registry=false;
				break;

			}
		
		}
	}


	while(playing){
	
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
	
	
		
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			
			desenhaCenario();
			desenhaScore(jogador_atual.pts);
			desenhaBloco(bloco);

			//atualiza a posição do bloco e informa se o nave se chocou com o bloco
			playing=atualizaBloco(&bloco,SCREEN_H,SCREEN_W,&nave,tiros,&qtd_tiros,inimigos);
		
			//trata de encerrrar while antes que o valor de playing seja indevidamente alterado
			if(!playing){
				break;
			}

			desenhaNave(nave);

			//atualiza a posição da nave e informa se ela se chocou com um inimigo
			playing=atualizaNave(&nave,inimigos);

			//realiza o carregamnto do tiro caso soma seja verdadeiro
			if(soma){
				//não permite que o tiro passe de 30 de raio
				if(tam_tiro<30){
					tam_tiro++;	
				}
				//desenha o carregamento do tiro
				desenhaCarregador(&nave,tam_tiro);
			}else{
				//atira caso soma seja falso e permission seja verdadeiro
				atiraTiro(nave,tiros,&qtd_tiros,&tam_tiro,&permission);
			}
			
			atualizaTiro(tiros,&qtd_tiros,inimigos,&nave,&jogador_atual.pts);	
			
			desenhaInimigo(inimigos);
			atualizaInimigo(inimigos);
			
			al_flip_display();
		}
		
		
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing=false;
			final=false;
			fim_rec=false;
		}
		
		//se o tipo de evento for um pressionar de uma tecla
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_W:
					nave.dir_y--;

				break;

				case ALLEGRO_KEY_S:
					nave.dir_y++;
				break;
				
				case ALLEGRO_KEY_A:
					nave.dir_x--;
				break;
				
				case ALLEGRO_KEY_D:
					nave.dir_x++;
				break;

				case ALLEGRO_KEY_SPACE:
					soma=true;
				break; 
			}
		
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_W:
					nave.dir_y++;
				break;

				case ALLEGRO_KEY_S:
					nave.dir_y--;
				break;
				
				case ALLEGRO_KEY_A:
					nave.dir_x++;
				break;
				
				case ALLEGRO_KEY_D:
					nave.dir_x--;
				break;

				case ALLEGRO_KEY_SPACE:
					permission=true;
					soma=false;
				break; 
			}
		
		}
	
	}
	
	//caso tenha sidofeito um registro ele é salvo no arquivo
	if(save){
		registraPontos(&jogador_atual);
		qtd_pilotos=consultaSave(result);
	}

	//mostra a tela de parabens caso haja quebrado o record
	while(jogador_atual.bateu && fim_rec){

		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		telaParabens(jogador_atual);
		al_flip_display();

		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			fim_rec=false;
			final=false;
		}

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_ENTER:
					//aguarda pelo enter para prosseguir
					fim_rec=false;
				break;
			}
		}

	}

	//mostra a tela final
	telaFinal(result,qtd_pilotos);
	al_flip_display();

	//aguarda o enter do usuário para fechar o programa
	while(final){

		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			final=false;
		}

		//se o tipo de evento for um pressionar de uma tecla
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_ENTER:
					final=false;
				break;
			}
		}
			
	}
	
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
	return 0;
}
