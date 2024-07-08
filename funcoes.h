#ifndef _FUNCOES_H_
#define _FUNCOES_H_
#include <stdbool.h>

typedef struct Item{
  char* pal;
  int cost;
  int posicao;
  struct Item *prox;
} item;

typedef struct No_acervo {
  int  vertice;
  int distancia;
} no_acervo;

typedef struct Acervo {
  int n_vertices_actual;
  int tamanho_maximo;
  int *posicao_acervo;
  no_acervo **vector_acervo;
} acervo;

typedef struct Caminho {
  int dist;
  int elemento_anterior;
  int tamanho_caminho;
} caminho;


/*Funcoes do ficheiro funcoes.c*/

int recolhe_problema(FILE*f, char**, char** , int*); /* funcao que recebe ponteiro para o ficheiro e recolhe as duas palavras + tipo de argumento */
int conta_palavras_mesmo_tamanho(char*, int );
void guarda_palavras_mesmo_tamanho(char* ,int , item** );
int* resolucao_problema(char**, char**, char*, int, int, item***, int*,int* );
void escreve_solucao(FILE*f ,item* grafo,int* ,int, int);
char* cria_nome_saida(char* );
char* adiciona_palavra(char* ,int );
int qsort_comparacao(const void* a, const void* b);
int procura_binaria(item* grafo,int tamanho, char* palavra);
void free_vector_grafos();
void free_lista(item*);
void escreve_solucao2(FILE*, char*, char* );
void escreve_solucao3(FILE*, char*, char* );
void escreve_solucao4(FILE*, char*, char* );

/*Funcoes do ficheiro lista_adj.c*/

int calculate_hamming_dist(char* , char*, int, int );
void cria_lista_adj(item*, int, int, int);
void new_item(item* , int , int, int);

/*Funcoes do ficheiro acervo.c*/

no_acervo* novo_no_acervo(int , int );
acervo* cria_acervo(int );
void troca_no_acervo(no_acervo** , no_acervo** );
void ordena_acervo( acervo* , int );
no_acervo* remove_minimo(acervo* );
void organiza_no( acervo*, int , int );
bool pertence_acervo(acervo *, int );
void imprime_vector(caminho* , int );
int verifica_acervo(acervo*);
int* dijkstra(item*,int,int,int*,int*);
/*int compara_lista(path*, path* );*/
void free_caminho(caminho*, int);
void free_acervo(acervo*, int);

#endif
