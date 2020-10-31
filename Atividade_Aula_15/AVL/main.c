#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include <time.h>

ArvAVL* cria_ArvAVL(){
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO(NO* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvAVL(ArvAVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);//libera cada n�
    free(raiz);//libera a raiz
}

//Fun��es
//Calcula a altura do n�
int alt_NO(NO* no) {
	if(no == NULL)
		return -1;
	else
		return no->alt;
}


//calcula o fator de balanceamento de um n�
int fatorBalanceamento_NO(NO* no) {
	return labs(alt_NO(no->esq) - alt_NO(no->dir));
}

int maior(int x, int y) {
	if(x > y)
		return x;
	else
		return y;
}

NO* procuraMenor(NO* atual){
    NO* no1 = atual;
    NO* no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

//Rota��o simples a direita
void RotacaoSD(ArvAVL* raiz) { //RotacaoLeft Left
	NO *no;
	no = (*raiz)->esq;
	(*raiz)->esq = no->dir;
	no->dir = (*raiz);

	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->esq), (*raiz)->alt) + 1;

	*raiz = no;
}

//Rota��o simples a esquerda
void RotacaoSE(ArvAVL* raiz) { //RotacaoRight Right
	NO *no;
	no = (*raiz)->dir;
	(*raiz)->dir = no->esq;
	no->esq = (*raiz);

	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->dir), (*raiz)->alt) + 1;

	*raiz = no;
}

//Rota��o dupla a direita
 void RotacaoDD(ArvAVL* raiz) { //RotacaoLeft Right
	RotacaoSE(&(*raiz)->esq);
	RotacaoSD(raiz);
}

//Rota��o dupla a esquerda
void RotacaoDE(ArvAVL* raiz) { //RotacaoRight Left
	RotacaoSD(&(*raiz)->dir);
	RotacaoSE(raiz);
}

/* "Inser��o AVL"
	Para inserir um valor "V" na �rvore:
	- "raiz" � NULL: insira o n�;
	- "V" � "menor" do que a raiz:
		- v� para a sub-arvore a esquerda;
	- "V" � maior do que a raiz:
		- v� para a subarvore a direita;
	-aplique o m�todo "recursivamente";

	- Ao voltar na recurs�o, recalcule as alturas
	de cada sub-�rvore;
	- Aplique a rota��o necess�ria se o
	fator de balanceamento passar a ser +2 ou -2;
*/

//--------------------
//----------- INSER��O
//--------------------
int insere_ArvAVL(ArvAVL* raiz, int valor) {
	int res;
	if(*raiz == NULL) { //�rvore vazia ou n� folha
		NO *novo;
		novo = (NO*)malloc(sizeof(NO));
		if(novo == NULL)
			return 0;

		novo->info = valor;
		novo->alt = 0;
		novo->esq = NULL;
		novo->dir = NULL;
		*raiz = novo;
		return 1;
	}

	NO *atual = *raiz;
	if(valor < atual->info) {
		if((res=insere_ArvAVL(&(atual->esq), valor)) == 1) {
			if(fatorBalanceamento_NO(atual) >=2) {
				if(valor < (*raiz)->esq->info) {
					RotacaoSD(raiz);
				} else {
					RotacaoDD(raiz);
				}
			}
		}
	}
	else {
		if(valor > atual->info) {
			if((res=insere_ArvAVL(&(atual->dir), valor))==1) {
				if(fatorBalanceamento_NO(atual) >= 2) {
					if((*raiz)->dir->info < valor) {
						RotacaoSE(raiz);
					} else {
						RotacaoDE(raiz);
					}
				}
			}
		} else {
			printf("Valor duplicado!!\n");
			return 0;
		}
	}

	atual->alt = maior(alt_NO(atual->esq), alt_NO(atual->dir)) + 1;

	return res;
}

/* "Remo��o AVL"
	Existem 3 tipos de remo��o:
	- N� folha (sem filhos);
	- N� que possu� 1 filho;
	- N� que possu� 2 filhos;

	Os 3 tipos de remo��o trabalham juntos. A remo��o sempre remove um elemento
	espec�fico da �rvore, o qual pode ser um n� folha, ter 1 ou dois filhos.

	Cuidado:
	- N�o podemos remover um n� de uma �rvore vazia;
	- Removendo o �ltimo n� a �rvore fica vazia;

	Balanceamento:
	- Valem as mesmas regras da inser��o;
	- "Remover" um n� da subarvore a direita equivale a
	"inserir" um n� na sub�rvore da esquerda

*/
//--------------------
//------------ REMO��O
//--------------------
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){// valor n�o existe
	    printf("valor n�o existe!!\n");
	    return 0;
	}

    int res;
	if(valor < (*raiz)->info){
	    if((res = remove_ArvAVL(&(*raiz)->esq, valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(alt_NO((*raiz)->dir->esq) <= alt_NO((*raiz)->dir->dir))
                    RotacaoSE(raiz);
                else
                    RotacaoDE(raiz);
            }
	    }
	}

	if((*raiz)->info < valor){
	    if((res = remove_ArvAVL(&(*raiz)->dir, valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq) )
                    RotacaoSD(raiz);
                else
                    RotacaoDD(raiz);
            }
	    }
	}

	if((*raiz)->info == valor){
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){// n� tem 1 filho ou nenhum
			NO* oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		}else { // n� tem 2 filhos
			NO* temp = procuraMenor((*raiz)->dir);
			(*raiz)->info = temp->info;
			remove_ArvAVL(&(*raiz)->dir, (*raiz)->info);
            if(fatorBalanceamento_NO(*raiz) >= 2){
				if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq))
					RotacaoSD(raiz);
				else
					RotacaoDD(raiz);
			}
		}
		if (*raiz != NULL)
            (*raiz)->alt = maior(alt_NO((*raiz)->esq),alt_NO((*raiz)->dir)) + 1;
		return 1;
	}

	(*raiz)->alt = maior(alt_NO((*raiz)->esq),alt_NO((*raiz)->dir)) + 1;

	return res;
}

// ###############################################################################################

//Impressao agrad�vel
void paddind(char ch, int n) {
	int i;
	for(i = 0; i < n; i++)
		putchar(ch);
}
//Imprimir arvore
void printfArvPritty(NO *raiz, int level) {
	if( raiz == NULL) {
		paddind('\t', level);
		puts(" ");
	} else {
		printfArvPritty(raiz->dir, level+1);
		paddind('\t', level);
		printf("%d\n", raiz->info);
		printfArvPritty(raiz->esq, level+1);
	}
}

int main() {
	srand(time(0));

    ArvAVL* avl;
    int res, i;

    avl = cria_ArvAVL();

    //InserindoprintfArvPritty  N numeros distintos
    int N = 15; ///Numero de valores a inserir
	int random_numbers[N];
	int count = 0;

    //Gerando valores distintos
	while(count < N) {
		int randNum =(rand() % N) + 1;
		int found = 0;
		//Verificando se numero eh distinto
		for (i = 0; i < N; i++) {
			if(random_numbers[i] == randNum) {
				found =1;
				break;
			}
		}
		if(found != 1) {
			random_numbers[count] = randNum;
			//inserindo o valor distinto na arvore avl
			res = insere_ArvAVL(avl, randNum);
			count++;
		}
	}

    //imprimir a arvore desde o nivel 0
    printfArvPritty(*avl, 0);

    //excluir a arvore
    libera_ArvAVL(avl);

	return 0;
}



