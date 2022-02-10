# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <string.h>

#define TAM_NAME 15
// const int TAM_NAME=15;
const int MAX_PILOT=100;
const char nome_arquivo[]="files/records.txt";


typedef struct Scores{
	char pil[TAM_NAME];
	int pts;
	bool bateu;
} Scores;

void initPiloto(Scores *piloto){

	(*piloto).pts=0;
	(*piloto).bateu=false;

}


int consultaSave(Scores result[]){
	
	FILE *arq=fopen(nome_arquivo,"r"),*arqw;

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token,nome_s[TAM_NAME];
	char lixo[10];
	int pontos[MAX_PILOT];
	int tam=0;


	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);// retira o \n da primeira linha


	for(int i=0;i<tam;i++){

		fgets(pilotos[i],TAM_NAME,arq);
		token=strtok(pilotos[i],"!");
		
		result[i].pts=atoi((strtok(NULL,"\n")));		
		strcpy(result[i].pil,token);

	}


	fclose(arq);	

	return tam;

}


bool comparaNome(char nome1[], char nome2[]){

	for(int i=0;nome1[i]==nome2[i];i++){
		if(nome1[i]=='\0' && nome2[i]=='\0'){
			return true;
		}
	}
	return false;

}

bool procuraPiloto(char piloto[], int *num){

	FILE *arq=fopen(nome_arquivo,"r");

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token;
	char lixo[10];
	// int pontos[MAX_PILOT];
	int tam=0;


	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);
	// fgets(pilotos[0],100,arq);

	// arquivo vazio
	if(tam==0){
		*num=0;
		return false;
	}




	for(int i=0;i<tam;i++){

		fgets(pilotos[i],TAM_NAME,arq);
		token=strtok(pilotos[i],"!");
		strcpy(pilotos[i],token);

		// printf("\n%i: %s\n",i ,pilotos[i]);

		if(comparaNome(pilotos[i],piloto)){
			*num=i;

			return true;
		}
	}
	fclose(arq);
	if(tam==MAX_PILOT){
		*num=MAX_PILOT-1;
	}else{
		*num=tam;
	}
	return false;
}

void registraPiloto(Scores *gamer,int id,int at){

	FILE *arq=fopen(nome_arquivo,"r"),*arqw;

	char pilotos[MAX_PILOT][TAM_NAME];
	char *token,nome_s[TAM_NAME];
	char lixo[10];
	int pontos[MAX_PILOT];
	int troca=-1;
	int tam=0,pts,pts_s;

	pts=(*gamer).pts;

	fscanf(arq,"%i",&tam);
	fgets(lixo,10,arq);



	for(int i=0;i<tam;i++){

		fgets(pilotos[i],TAM_NAME,arq);
		token=strtok(pilotos[i],"!");
		// fscanf(arq,"%s %i",&lixo[1],&pontos[i]);
		pontos[i]=atoi((strtok(NULL,"\n")));

		
		strcpy(pilotos[i],token);

		// printf("\n%i: %s\n",i,pilotos[i]);
		// printf("\n%i: %i\n",i,pontos[i]);
	}

	if(tam!=MAX_PILOT){
		tam+=at;
	}
	

	fclose(arq);


	//registra recordes
	if(at==0 && pts>pontos[id] || at==1){
		(*gamer).bateu=true;
		pontos[id]=pts;
	}

	strcpy(pilotos[id],(*gamer).pil);
	

	//identifica se há algum piloto com pontuação menor
	for(int i=0;i<tam;i++){
		if(pontos[id]>=pontos[i]){
			troca=i;
			break;
			printf("\nA2: %i\n",i);
		}
	}
	

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
				printf("\nA3\n");
				strcpy(pilotos[i],nome_s);
				pontos[i]=pts_s;
				break;
			}else{
				printf("\nA4\n");
				strcpy(pilotos[i],pilotos[i-1]);
				pontos[i]=pontos[i-1];
			}
		}
		
	}
	
	

	arqw=fopen(nome_arquivo,"w");
	fprintf(arqw, "%i\n", tam);

	for(int i=0;i<tam;i++){
		fprintf(arqw, "%s!%i\n",pilotos[i],pontos[i]);		
	}
	fclose(arqw);


	
}

void registraPontos(Scores *gamer){

	int num=0;

	if(procuraPiloto((*gamer).pil,&num)){
		registraPiloto(gamer,num,0);
	}else{
		registraPiloto(gamer,num,1);
	}


}


// int main(){

// 	// registraPontos("Sergio",11);
// 	// registraPontos("Dagoberto",15);
// 	registraPontos("Pericles",14);
// 	registraPontos("Roberto",16);


// 	return 0;


// }