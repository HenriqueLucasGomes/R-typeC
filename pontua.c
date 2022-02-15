# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <string.h>

#define TAM_NAME 15

const int MAX_PILOT=100;
const char nome_arquivo[]="files/records.txt";

typedef struct Scores{
	char pil[TAM_NAME]; //nome do piloto
	int pts;            //pontos do piloto
	bool bateu;         //considera se bateu ou não seu record
} Scores;

//inicia os valores de piloto
void initPiloto(Scores *piloto){
	(*piloto).pil[0]='\0'; //define o caracter inicial do piloto como sendo \0
	(*piloto).pts=0;       //define pontos inicias como 0
	(*piloto).bateu=false; //define que o piloto não bateu seu record
}

//responsável por consultar as informações do rarquivo de records
int consultaSave(Scores result[]){
	
	//abre o arquivo
	FILE *arq=fopen(nome_arquivo,"r"),*arqw;

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token,nome_s[TAM_NAME];
	char lixo[10];
	int pontos[MAX_PILOT];
	int tam=0;

	//lê a primeira linha com o tamanho da lista
	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);// retira o \n da primeira linha

	//faz a leitura de cada linha do arquivo
	for(int i=0;i<tam;i++){
		
		//leem toda a linha
		fgets(pilotos[i],TAM_NAME,arq);
		
		//cortam a lista até o nome
		token=strtok(pilotos[i],"!");
		
		//cortam a lista do último ponto até o fim
		result[i].pts=atoi((strtok(NULL,"\n")));

		//armazenam o nome na váriável correta		
		strcpy(result[i].pil,token);
	}

	//fecha o arquivo
	fclose(arq);	

	//tamanho da lista
	return tam;

}

//compara se duas strings enviadas são iguais
bool comparaNome(char nome1[], char nome2[]){

	//comparam caracter por caracter
	for(int i=0;nome1[i]==nome2[i];i++){
		if(nome1[i]=='\0' && nome2[i]=='\0'){
			return true;
		}
	}
	return false;

}

//procura por um piloto requisitado e retorna se ele se encontra na lista e a sua posição
bool procuraPiloto(char piloto[], int *num){

	//abre o arquivo
	FILE *arq=fopen(nome_arquivo,"r");

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token;
	char lixo[10];
	int tam=0;

	//lê a primeira linha com o tamanho da lista
	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);// retira o \n da primeira linha

	//caso arquivo esteja vazio
	if(tam==0){
		*num=0;
		//fecha o arquivo
		fclose(arq);
		return false;
	}

	//faz a leitura de cada linha do arquivo
	for(int i=0;i<tam;i++){

		//leem toda a linha
		fgets(pilotos[i],TAM_NAME,arq);

		//cortam a lista até o nome
		token=strtok(pilotos[i],"!");

		//armazenam o nome na váriável correta		
		strcpy(pilotos[i],token);

		//compara os nomes até encontrar, ou não, um nome igual 
		if(comparaNome(pilotos[i],piloto)){
			*num=i;
			//fecha o arquivo
			fclose(arq);
			return true;
		}
	}

	//fecha o arquivo
	fclose(arq);

	
	
	if(tam==MAX_PILOT){
		//caso o tamanho tenha chegado ao limite então a localizção do novo piloto será a mesma do último da lista
		*num=MAX_PILOT-1;
	}else{
		//caso não tenha achado um piloto compatível então a localização do novo piloto será o tamanho da lista
		*num=tam;
	}
	return false;//piloto não encontrado
}

//responsável por registrar o piloto
void registraPiloto(Scores *gamer,int id,int at){

	//abre o aarquivo
	FILE *arq=fopen(nome_arquivo,"r"),*arqw;

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token,nome_s[TAM_NAME];
	char lixo[10];
	int pontos[MAX_PILOT];
	int troca=-1;
	int tam=0,pts,pts_s;

	//salva os pontos registrados pelo jogador
	pts=(*gamer).pts;

	//lê a primeira linha com o tamanho da lista
	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);// retira o \n da primeira linha

	//faza leitura do arquivo linha por linha
	for(int i=0;i<tam;i++){

		//leem toda a linha
		fgets(pilotos[i],TAM_NAME,arq);

		//cortam a lista até o nome
		token=strtok(pilotos[i],"!");

		//cortam a lista do último ponto até o fim
		pontos[i]=atoi((strtok(NULL,"\n")));

		//armazenam o nome na váriável correta	
		strcpy(pilotos[i],token);
	}

	//caso o tamanho da lista esteja no máximo ela não sofre nenhum acrescimo
	if(tam!=MAX_PILOT){
		tam+=at;
	}

	//fecha o arquivo
	fclose(arq);


	//caso haja recorde o valor dos pontos é substituido e registrado
	if(at==0 && pts>pontos[id] || at==1){
		(*gamer).bateu=true;
		pontos[id]=pts;
	}

	//registra o nome do piloto na lista
	strcpy(pilotos[id],(*gamer).pil);
	

	//identifica se há algum piloto com pontuação menor
	for(int i=0;i<tam;i++){
		if(pontos[id]>=pontos[i]){
			troca=i;
			break;
			printf("\nA2: %i\n",i);
		}
	}
	
	//troca as posições dos pilotos de modo a deixar sempre as maiores pontuações acima
	if(troca>=0){
		
		//salva os dados do jogador registrado
		strcpy(nome_s,pilotos[id]);
		pts_s=pontos[id];
		
		//retira o jogador registrado da lista movendo os demais -1 posição
		for(int i=id;i<tam-1;i++){
			strcpy(pilotos[i],pilotos[i+1]);
			pontos[i]=pontos[i+1];
		}

		//procura pelo piloto com pontuação menor que a do jogador registrado, ao passo que move os demais -1 posição
		for(int i=tam-1;i>=0;i--){
			if(troca==i){
				strcpy(pilotos[i],nome_s);
				pontos[i]=pts_s;
				break;
			}else{
				strcpy(pilotos[i],pilotos[i-1]);
				pontos[i]=pontos[i-1];
			}
		}	
	}
	
	//abre arquivo
	arqw=fopen(nome_arquivo,"w");

	//regitra o tamnho da lista no arquivo
	fprintf(arqw, "%i\n", tam);

	//regitra cada piloto e pontuação da lista no arquivo
	for(int i=0;i<tam;i++){
		fprintf(arqw, "%s!%i\n",pilotos[i],pontos[i]);		
	}

	//fecha o arquivo
	fclose(arqw);	
}

//responsável por realizar o registro dos pilotos novos e recorrentes
void registraPontos(Scores *gamer){

	//posição default
	int num=0;

	//define se é um piloto novo ou recorrente
	if(procuraPiloto((*gamer).pil,&num)){
		registraPiloto(gamer,num,0);
	}else{
		registraPiloto(gamer,num,1);
	}
}