#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "saco.h"
#include "jogadas.h"
#include "dicionario.h"



jogo_t *inicio_jogo (FILE *arq) { //iniciar o jogo
	saco_t *saco = inicializar_saco();
	jogo_t *jogo = cria_jogo(saco,arq);
	def_quant_jog(jogo);
	return jogo;
}

jogo_t *cria_jogo(saco_t *Saco, FILE *arq){
	jogo_t *jogo = (jogo_t *) malloc(sizeof(jogo_t));
	if (jogo == NULL)
	{
		printf("Erro, sem memoria!\n");
		exit(1);
	}
	jogo->inicio = NULL;
	jogo->fim = NULL;
	jogo->atual = NULL;
	jogo->num_jogadores=0;
	jogo->saco = Saco;
	jogo->pula_vez=0;
	inicia_tabuleiro(jogo->tabuleiro);
	jogo->dicionario = inicia_dicionario(arq);
	return jogo;
}

void inicia_tabuleiro(char tabuleiro[15][15]){
	for (int i = 0; i<15; i++)
		for (int j = 0; j<15; j++)
			tabuleiro[i][j]='-';
}

void def_quant_jog (jogo_t *jogo) { //definir quantas pessoas vao jogar (2 a 4)
	int jog;
	printf("Digite a quantidade de jogadores de 2 a 4\n");
	scanf("%d", &jog);
	if(jog>=2 && jog<=4) {
		while (jogo->num_jogadores != jog){
			insere_jogador(jogo);
		}
	}
	else{
		printf("Erro, quantidade de jogadores nao disponivel, tente de novo\n");
		def_quant_jog(jogo);
	}
}

void imprime_tabuleiro(char tabuleiro[15][15]){
	printf("  |");
	for(int i=0; i<15; i++){
		//if(i<10)
			printf("%c |",i+97);
		//else
			//printf("%d|",i);
	}
	printf("\n");
	for (int i = 0; i<15; i++){
		if(i<10)
			printf("%d |",i);
		else
			printf("%d|",i);
		for (int j = 0; j<15; j++){
			if(tabuleiro[i][j] == '-')
				printf("  |");
			else 
				printf("%c |",tabuleiro[i][j]);
		}
		printf("\n  ");
		for(int d = 0; d<15; d++){
			printf(" - ");
		}
		printf("\n");
	}
}



void insere_jogador(jogo_t *jogo){  //inserir jogador em jogo

	peca_t *novo = (peca_t *) malloc(sizeof(peca_t));
	jogador_t *jogador = (jogador_t *) malloc(sizeof(jogador_t));

	if (novo == NULL || jogador == NULL){
		printf("Erro, sem memoria\n");
		exit(1);
	}

	jogador->prox = NULL;
	jogador->jogador_pontos = 0;

	if (jogo->inicio == NULL){
		jogador->prev = NULL;
		jogo->inicio = jogador;
		jogo->atual = jogador;
		jogador->jogador_num = 1;
	}

	if (jogo->inicio != NULL)	{
		jogador->prev=jogo->fim;
		//jogador->jogador_num = jogo->fim->jogador_num+1;
	}

	if (jogo->fim != NULL){
		jogo->fim->prox = jogador; 
	}

	if(jogador->jogador_num == 0) {
		int num = jogador->prev->jogador_num;
		jogador->jogador_num = num + 1;
	} 

	jogo->fim = jogador;
	jogo->num_jogadores++;

	for(int i = 0; i<20; i++) { //adicionar as 7 primeiras pecas do jogador
		novo = remove_saco(jogo->saco);
		jogador->suporte[i] = novo;
	}

}

void trocar_todas_pecas(jogo_t *jogo){ //inserir novas pecas no suporte e devolver as antigas de forma aleatoria
	jogo->pula_vez=0;

	for(int d=0; d<20; d++) {

		int pos = rand() % jogo->saco->num_elementos;
		//printf("rand = %d\n",pos);

		peca_t *toChange;
		peca_t *novo = jogo->atual->suporte[d];
		peca_t *aux_prev = jogo->saco->inicio;
		peca_t *aux_prox = aux_prev->prox;

		for(int i=0; i<pos; i++){
			aux_prox = aux_prox->prox;
			aux_prev = aux_prev->prox;
		}

		if(aux_prox == NULL){
			jogo->saco->fim = novo;
			novo->prox = aux_prox;
			novo->prev = aux_prev;
			aux_prev->prox = novo;
		}

		else{
			novo->prox = aux_prox;
			novo->prev = aux_prev;
			aux_prev->prox = novo;
			aux_prox->prev = novo;
		}

		jogo->atual->suporte[d] = remove_saco(jogo->saco);
		jogo->saco->num_elementos++;
	}
}



void trocar_peca(jogo_t *jogo){
	jogo->pula_vez=0;

	if(jogo->saco->num_elementos < 2) {
		printf("Nao ha mais pecas para serem trocadas, selecione outra jogada\n");
		return;
	}

	else{
		int pos = rand() % jogo->saco->num_elementos;
		//printf("rand = %d\n",pos);
		int pos_letra;

		printf("Digite o numero que indica a posicao da letra que voce deseja trocar: \n");

		scanf("%d", &pos_letra);
		if(pos_letra < 1 || pos_letra > 20) {
			printf("Essa posicao nao existe, tente de novo\n");
			trocar_peca(jogo);
		}

		peca_t *novo = jogo->atual->suporte[pos_letra - 1];
		peca_t *aux_prev = jogo->saco->inicio;
		peca_t *aux_prox = aux_prev->prox;

		if(novo->letra == '-'){
			printf("Essa peca nao pode ser trocada, tente de novo\n");
			trocar_peca(jogo);
		}

		for(int i=0; i<pos; i++){
			aux_prox = aux_prox->prox;
			aux_prev = aux_prev->prox;
		}

		if(aux_prox == NULL){
			jogo->saco->fim = novo;
			novo->prox = aux_prox;
			novo->prev = aux_prev;
			aux_prev->prox = novo;
		}

		else{
			novo->prox = aux_prox;
			novo->prev = aux_prev;
			aux_prev->prox = novo;
			aux_prox->prev = novo;
		}

		jogo->atual->suporte[pos_letra - 1] = remove_saco(jogo->saco);
		jogo->saco->num_elementos++;
	}

}

void teste_palavra (jogo_t *jogo, peca_t novas_pecas[20], int ins, char palavra[16], char sentido, int x, int y, int pos_novo[20]) { //vai checar se a palavra formada existe

	int b = busca(jogo->dicionario,palavra);
		if(b == 1){ //palavra existe
			peca_t *toFree; //vai ter que trocar as pecas do suporte que foram utilizadas
			printf("Palavra %s existe\n",palavra);
			for(int v=0; v<20; v++){
				toFree = jogo->atual->suporte[v];
				if(toFree->letra == '-'){
					jogo->atual->jogador_pontos = jogo->atual->jogador_pontos + toFree->ponto;
					free(toFree);
					jogo->atual->suporte[v] = remove_saco(jogo->saco);
				}	
			}
			return;
		}
		else{ //palavra nao existe
			peca_t *toRedo; //vai ter que voltar as pecas que foram retiradas do suporte
			printf("Palavra %s nao existe\n",palavra);
			for(int v=0; v<20; v++){
				toRedo = jogo->atual->suporte[v];
				if(toRedo->letra == '-'){ //letra dessa posicao foi retirada
					for(int k=0; k<ins; k++){
						if(novas_pecas[k].ponto == toRedo->ponto && novas_pecas[k].letra != '-'){
							toRedo->letra = novas_pecas[k].letra;
							novas_pecas[k].letra = '-';
							break;
						}
					}
				}	
			}
			if(sentido == 'v'){ //se o sentido for pra baixo, vai excluir as letras que foram colocadas no tabuleiro
				for(int v=0; v<ins; v++){
					y=pos_novo[v];
					jogo->tabuleiro[y][x] = '-';
				}
			}
			else if(sentido == '>'){ //caso o sentido for para o lado
				for(int v=0; v<ins; v++){
					x=pos_novo[v];
					jogo->tabuleiro[y][x] = '-';
				}
			}

			return;
		}
}

void add_palavra(char sentido, int x, int y, jogo_t *jogo){ //vai colocar as pecas no tabuleiro

	peca_t novas_pecas[20]; //vetor que vai guardar a informacao das pecas colocadas
	int pos_novo[20];  //vetor que vai guardar em quais posicoes foram colocadas as novas pecas
	char palavra[16]; //palavra que vai ser criada
	int cont_palavra = 0; //contador das letras da palavra
	int pos_letra; //leitor da posicao da letra no suporte
	int score = 0; //pontuacao feita pelo jogador
	int ins = 0; //quantidade de insercoes feitas

	for(int j=0; j<20; j++){
		novas_pecas[j].letra = '-';
		novas_pecas[j].ponto = 0;
	}

	printf("Posicao: %c %d , sentido: %c\n",x+97,y,sentido);

		for(int j=0; j<20; j++){

			imprime_tabuleiro(jogo->tabuleiro);
			printf("Suas pedras são:\n");
		    for (int v=0; v<; v++){
		    	printf("%c ",jogo->atual->suporte[v]->letra);
		    }
		    printf("\n");
		    for (int v=0; v<7; v++){
		      printf("%d ",v+1);
		    }
		    printf("\n");

			if(jogo->tabuleiro[y][x] != '-'){ //ja tem uma letra inserida na posicao
				palavra[cont_palavra] = jogo->tabuleiro[y][x];
				cont_palavra++;
			}

			else{
				printf("Escolha qual letra colocara na posicao atual %c %d digitando a posicao dela no suporte\n",x+97,y);
				printf("Se tiver finalizado a jogada, digite 0\n");
				scanf("%d",&pos_letra);


				if(pos_letra == 0){
					teste_palavra (jogo,novas_pecas,ins,palavra,sentido,x,y,pos_novo);
					return;
				}

				if(pos_letra < 0 || pos_letra > 7) {
					printf("Essa posicao nao existe, tente de novo\n");
					j--;
				}

				else if (jogo->atual->suporte[pos_letra - 1]->letra != '-') { 
					jogo->tabuleiro[y][x] = jogo->atual->suporte[pos_letra - 1]->letra;
					palavra[cont_palavra] = jogo->atual->suporte[pos_letra - 1]->letra;
					novas_pecas[ins].letra = jogo->atual->suporte[pos_letra - 1]->letra;
					novas_pecas[ins].ponto = jogo->atual->suporte[pos_letra - 1]->ponto;
					jogo->atual->suporte[pos_letra - 1]->letra = '-'; //indicar que removeu a letra do suporte (n por completo)

					if(sentido == 'v'){ //se o sentido for pra baixo, a proxima posicao ira pra baixo
						pos_novo[ins] = y; //guardar a posicao em que a peca que estava no suporte foi colocada
						ins++;
					}
					else if(sentido == '>'){ //caso o sentido for para o lado
						pos_novo[ins] = x;
						ins++;
					}
					cont_palavra++;	
				}

				
			}

			if(sentido == 'v'){ //se o sentido for pra baixo, a proxima posicao ira pra baixo
				y++;
				if(y>14){ //atingiu o fim do tabuleiro
					teste_palavra (jogo,novas_pecas,ins,palavra,sentido,x,y,pos_novo);
					return;
				} 
					
			}
			else if(sentido == '>'){ //caso o sentido for para o lado
				x++;
				if(x>14){
					teste_palavra (jogo,novas_pecas,ins,palavra,sentido,x,y,pos_novo);
					return;
				} 
			}

		}

		teste_palavra (jogo,novas_pecas,ins,palavra,sentido,x,y,pos_novo);

		/*printf("%s\n",palavra);
		for(int j=0; j<ins; j++){
			printf("%d ",pos_novo[j]);
		}
		printf("\n"); */
}

void formar_palavra(jogo_t *jogo){ //vai pegar as indicacoes para que uma palavra seja formada
	jogo->pula_vez=0;
	printf("Escolha a posicao de inicio da palavra no formato a 0\n");
	
	char sent;
	char xx;
	int x;
	int y;

	scanf(" %c %d",&xx, &y);

	x = xx-97;

	if(x<0 || x>14){
		printf("Essa posicao nao existe, tente novamente\n");
		formar_palavra(jogo);
	}

	if(y<0 || y>14){
		printf("Essa posicao nao existe, tente novamente\n");
		formar_palavra(jogo);
	}

	//printf("Posicao: %d %d\n",x,y);

	printf("Escolha o sentido que a palavra sera formada: > ou v\n");

	scanf(" %c",&sent);

	if(sent != 'v' && sent != '>'){
		printf("Sentido nao existe, tente novamente\n");
		formar_palavra(jogo);
	}

	//printf("lido %c\n",sent);

	//printf("Posicao: %d %d , sentido: %c\n",x,y,sent);

	add_palavra(sent,x,y,jogo);
	
}



void pular_vez(jogo_t *jogo, FILE *arq){
	jogo->pula_vez++;
	if(jogo->pula_vez == (jogo->num_jogadores)*2){
		printf("Todos os jogadores pularam a vez por mais de duas rodadas\n");
		fim_jogo(jogo, arq);
	}
}



void jogada(jogo_t *jogo, FILE *arq) {
	printf("Jogador %d sua vez de jogar\n",jogo->atual->jogador_num);
	printf("Sua pontuacao eh: %d\n",jogo->atual->jogador_pontos);

	imprime_tabuleiro(jogo->tabuleiro);

	printf("Suas pedras são:\n");
	//int i = 0;
    for (int d = 0; d < 20; d++){
    	printf("%c ",jogo->atual->suporte[d]->letra);
    }
    printf("\n");
    for (int d = 0; d < 20; d++){
      printf("%d ",d+1);
    }
    printf("\n");


    if(jogo->saco->num_elementos < 20){
    	if (jogo->saco->num_elementos == 0){
    		for(int i=0; i<jogo->num_jogadores; i++){
    			printf("As pecas acabaram, cada jogador tera mais uma chance de jogada\n");
	    		printf("Para formar uma palavra digite 1\n");
				printf("Para pular sua vez digite 2\n");
				printf("Para sair do jogo digite 0\n");

				int opt;

				scanf("%d",&opt);

				if(opt==1)
					formar_palavra(jogo);
				else if(opt==2)
					pular_vez(jogo,arq);
				else if(opt==0){
					fim_jogo(jogo,arq);
				}
				else{
					printf("Opcao invalida, tente de novo\n");
					jogada(jogo,arq);
				}
				if(jogo->atual != jogo->fim)
					jogo->atual = jogo->atual->prox;
				else
					jogo->atual = jogo->inicio;
    		}
    		fim_jogo(jogo,arq);
    	}

    	else{
    		printf("Restam menos de sete pecas, nao se pode mais trocar todas as pecas\n");
    		printf("Para formar uma palavra digite 1\n");
			printf("Para pular sua vez digite 2\n");
			printf("Para trocar apenas uma pedra digite 3\n");
			printf("Para sair do jogo digite 0\n");

			int opt;

			scanf("%d",&opt);

			if(opt==1)
				formar_palavra(jogo);
			else if(opt==2)
				pular_vez(jogo,arq);
			else if(opt==3)
				trocar_peca(jogo);
			else if(opt==0){
				fim_jogo(jogo,arq);
			}
			else{
				printf("Opcao invalida, tente de novo\n");
				jogada(jogo,arq);
			}
    	}

    }

    else {
    	printf("Para formar uma palavra digite 1\n");
		printf("Para pular sua vez digite 2\n");
		printf("Para trocar apenas uma pedra digite 3\n");
		printf("Para trocar todas as suas pedras digite 4\n");
		printf("Para sair do jogo digite 0\n");

		int opt;

		scanf("%d",&opt);

		if(opt==1)
			formar_palavra(jogo);
		else if(opt==2)
			pular_vez(jogo,arq);
		else if(opt==3)
			trocar_peca(jogo);
		else if(opt==4)
			trocar_todas_pecas(jogo);
		else if(opt==0){
			fim_jogo(jogo,arq);
		}
		else{
			printf("Opcao invalida, tente de novo\n");
			jogada(jogo,arq);
		}
    }

	if(jogo->atual != jogo->fim)
		jogo->atual = jogo->atual->prox;
	else
		jogo->atual = jogo->inicio;

}

void imprime_ranking(jogo_t *jogo){
	jogador_t *atual = jogo->inicio;
	jogador_t *toComp = atual->prox;
	jogador_t aux;

	while (toComp != NULL) {
		while (toComp != NULL) {
			//printf("toComp = %d\n",toComp->jogador_num);
			if(atual->jogador_pontos < toComp->jogador_pontos){ //vai jogar a pontuacao menor pra direita
				//printf("pontuacao atual eh menor\n");
				aux.jogador_pontos = toComp->jogador_pontos;
				aux.jogador_num = toComp->jogador_num;
				//printf("aux %d pontos= %d\n",aux.jogador_num,aux.jogador_pontos);
				toComp->jogador_pontos = atual->jogador_pontos;
				toComp->jogador_num = atual->jogador_num;
				atual->jogador_pontos = aux.jogador_pontos;
				atual->jogador_num = aux.jogador_num;
				toComp = toComp->prox;
			}
			else{
				toComp = toComp->prox;
			}
		}
		atual = atual->prox;
		toComp = atual->prox;
	}


	atual=jogo->inicio;
	int i=1;

	printf("Vencedor é o jogador %d!\n",atual->jogador_num);
	printf("\n");
	printf("Ranking:\n");
	printf("\n");

	while(atual != NULL){
		printf("%d - jogador %d com %d pontos\n",i,atual->jogador_num,atual->jogador_pontos);
		atual=atual->prox;
		i++;
	}
}


void fim_jogo(jogo_t *jogo, FILE *arq) {
	printf("\n");
	printf("Fim do jogo\n");
	printf("\n");
	imprime_ranking(jogo);
	destroy_jogo(jogo);
	fclose(arq);
	exit(1);
}


void destroy_suporte(jogador_t *jogador){ 								//isso ta dando free de verdade??????
	for(int i = 0; i<20; i++)
	free(jogador->suporte[i]);
}



void destroy_jogo(jogo_t *jogo){
	destroy_saco (jogo->saco);
	destroy_dicionario (jogo->dicionario);

	jogador_t *toVisit = jogo -> inicio;
	jogador_t *toFree;

	while (toVisit != NULL) {
		toFree = toVisit;
		toVisit = toVisit->prox;
		destroy_suporte(toFree);
		free(toFree);
	}

	free(jogo);
}

