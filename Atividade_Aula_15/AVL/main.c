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
    libera_NO(*raiz);//libera cada nó
    free(raiz);//libera a raiz
}

//Funções
//Calcula a altura do nó
int alt_NO(NO* no) {
	if(no == NULL)
		return -1;
	else
		return no->alt;
}


//calcula o fator de balanceamento de um nó
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

//Rotação simples a direita
void RotacaoSD(ArvAVL* raiz) { //RotacaoLeft Left
	NO *no;
	no = (*raiz)->esq;
	(*raiz)->esq = no->dir;
	no->dir = (*raiz);

	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->esq), (*raiz)->alt) + 1;

	*raiz = no;
}

//Rotação simples a esquerda
void RotacaoSE(ArvAVL* raiz) { //RotacaoRight Right
	NO *no;
	no = (*raiz)->dir;
	(*raiz)->dir = no->esq;
	no->esq = (*raiz);

	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->dir), (*raiz)->alt) + 1;

	*raiz = no;
}

//Rotação dupla a direita
 void RotacaoDD(ArvAVL* raiz) { //RotacaoLeft Right
	RotacaoSE(&(*raiz)->esq);
	RotacaoSD(raiz);
}

//Rotação dupla a esquerda
void RotacaoDE(ArvAVL* raiz) { //RotacaoRight Left
	RotacaoSD(&(*raiz)->dir);
	RotacaoSE(raiz);
}

/* "Inserção AVL"
	Para inserir um valor "V" na árvore:
	- "raiz" é NULL: insira o nó;
	- "V" é "menor" do que a raiz:
		- vá para a sub-arvore a esquerda;
	- "V" é maior do que a raiz:
		- vá para a subarvore a direita;
	-aplique o método "recursivamente";

	- Ao voltar na recursão, recalcule as alturas
	de cada sub-árvore;
	- Aplique a rotação necessária se o
	fator de balanceamento passar a ser +2 ou -2;
*/

//--------------------
//----------- INSERÇÃO
//--------------------
int insere_ArvAVL(ArvAVL* raiz, int valor) {
	int res;
	if(*raiz == NULL) { //árvore vazia ou nó folha
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

/* "Remoção AVL"
	Existem 3 tipos de remoção:
	- Nó folha (sem filhos);
	- Nó que possuí 1 filho;
	- Nó que possuí 2 filhos;

	Os 3 tipos de remoção trabalham juntos. A remoção sempre remove um elemento
	específico da árvore, o qual pode ser um nó folha, ter 1 ou dois filhos.

	Cuidado:
	- Não podemos remover um nó de uma árvore vazia;
	- Removendo o último nó a árvore fica vazia;

	Balanceamento:
	- Valem as mesmas regras da inserção;
	- "Remover" um nó da subarvore a direita equivale a
	"inserir" um nó na subárvore da esquerda

*/
//--------------------
//------------ REMOÇÃO
//--------------------
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){// valor não existe
	    printf("valor não existe!!\n");
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
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){// nó tem 1 filho ou nenhum
			NO* oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		}else { // nó tem 2 filhos
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

//Impressao agradável
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



