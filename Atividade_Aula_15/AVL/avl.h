//typedef struct NO* ArvAVL;

typedef struct NO {
	int info;
	int alt;
	struct NO *esq;
	struct NO *dir;
}NO, *ArvAVL; //*ArvAVL é ponteiro, qnd se declara ArvAVL* raiz é ponteiro para ponteiro

ArvAVL* cria_ArvAVL();
void libera_NO(NO* no);
void libera_ArvAVL(ArvAVL* raiz);
int alt_NO(NO* no);
int fatorBalancemanto_NO(NO* no);
int maior(int x, int y);
NO* procuraMenor(NO* atual);
void RotacaoSD(ArvAVL* raiz);
void RotacaoSE(ArvAVL* raiz);
void RotacaoDD(ArvAVL* raiz);
void RotacaoDE(ArvAVL* raiz);
int insere_ArvAVL(ArvAVL* raiz, int valor);
int remove_ArvAVL(ArvAVL *raiz, int valor);
void paddind(char ch, int n);
void printfArvPritty(NO *raiz, int level);
