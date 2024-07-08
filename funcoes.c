#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"


/******************************************************************************
 * conta_palavras_mesmo_tamanho
 *
 * Abre o ficheiro dicionario e conta o numero de palavras com o mesmo tamanho.
 * 
 *****************************************************************************/
int conta_palavras_mesmo_tamanho(char* arg, int tamanho_original) {
  FILE *f;
  f = fopen ( arg, "r");
  int a=1, tamanho_actual,n=0;
  char palavra[50];

  a=fscanf(f,"%s",palavra);
  while(a==1) {
    tamanho_actual=strlen(palavra);
    if(tamanho_actual==tamanho_original)
      n++;
    a=fscanf(f,"%s",palavra);
  }

  fclose(f);
  return n;
}


/******************************************************************************
 * guarda_palavras_mesmo_tamanho
 *
 * Percorre o fichiro dicionario e adiciona as palavras que tenham o mesmo
 * tamanho que o tamanho da palavra inicial.
 * 
 *****************************************************************************/
void guarda_palavras_mesmo_tamanho(char* arg, int tamanho_original, item** grafo) {
  FILE *f;
  f = fopen(arg, "r");
  int p = 1, tamanho_actual = 0;
  char palavra[50];

  while(fscanf(f,"%s",palavra)==1) {
    tamanho_actual=strlen(palavra);
    if(tamanho_actual==tamanho_original) {
      (*grafo)[p].pal=adiciona_palavra(palavra,tamanho_original);
      p++;
    }
  }
    return;
}


/******************************************************************************
 * adiciona_palavra
 * 
 * Aloca memoria e coloca palavra na mesma e retorna o apontador para essa
 * memoria.
 * 
 *****************************************************************************/
char* adiciona_palavra(char* palavra, int tamanho) {
  char* nova_palavra;
  nova_palavra=(char*)calloc(tamanho+1,sizeof(char));
  strcpy(nova_palavra,palavra);
  return nova_palavra;
}


/******************************************************************************
 * qsort_comparacao
 * 
 *
 * 
 *****************************************************************************/
int qsort_comparacao(const void* a, const void* b) {
  item* pa = (item*)a;
  item* pb = (item*)b;

  return strcmp(pa->pal,pb->pal);
}


/******************************************************************************
 * resolucao_problema
 * 
 * funcao que liga o algoritmo a funcao main
 * 
 *****************************************************************************/
int* resolucao_problema(char** pal1 , char** pal2, char* arg, int tamanho,int n_mutacoes_max, item***vector_grafos,int* custo_final, int* tamanho_caminho_final) {
  int i, n, n1, n2 = 0;

  if(vector_grafos[tamanho-1][n_mutacoes_max]==NULL) { /*caso ainda nao haja grafo para o problema actual, conta numero de palavras do dicionario com o mesmo*/
    n = conta_palavras_mesmo_tamanho(arg,tamanho);     /*tamanho, e por fim aloca memoria para alocar essas palavras na estrutura item*/
    item* grafo = (item*)malloc((n+1)*sizeof(item));

    /*Primeiro elemento da tabela serve apenas para guardar o tamanho da tabela (numero de palavras existentes na tabela)*/
    grafo[0].cost=n;
    grafo[0].posicao=-5;
    grafo[0].prox=NULL;
    grafo[0].pal="a"; /*Criar uma palavra 'a' de tamanho "tamanho" e po la na posicao 0.pal desta */

    guarda_palavras_mesmo_tamanho(arg,tamanho,&grafo); /*coloca na tabela as palavras com tamanho igual as palavras iniciais*/
    qsort(grafo,n+1,sizeof(item),qsort_comparacao); /*organiza alfabeticamente as palavras*/
    for(i=1;i<n+1;i++) {
      grafo[i].posicao=i;
      grafo[i].cost=-1;
      grafo[i].posicao=i;
      grafo[i].prox=NULL;
    }


    cria_lista_adj(grafo,grafo[0].cost,n_mutacoes_max,tamanho);


    vector_grafos[tamanho-1][n_mutacoes_max]=grafo;

    n1=procura_binaria(grafo,n,*pal1);
    n2=procura_binaria(grafo,n,*pal2);
    return dijkstra(grafo,n1,n2,custo_final, tamanho_caminho_final);
  } else {
    n1=procura_binaria(vector_grafos[tamanho-1][n_mutacoes_max],vector_grafos[tamanho-1][n_mutacoes_max][0].cost,*pal1);
    n2=procura_binaria(vector_grafos[tamanho-1][n_mutacoes_max],vector_grafos[tamanho-1][n_mutacoes_max][0].cost,*pal2);

    return dijkstra(vector_grafos[tamanho-1][n_mutacoes_max],n1,n2,custo_final, tamanho_caminho_final);
  }
    return NULL;
}


/******************************************************************************
 * procura_binaria
 * 
 * 
 *****************************************************************************/
int procura_binaria(item* grafo,int tamanho, char* palavra) {
  int valor_menor = 0;
  int valor_maior = tamanho;
  int valor_medio = -1;
  int sucesso = 0;
    
  while(sucesso!=1) {
    if(valor_maior<valor_menor)
      return -1; /* a palavra nao existe*/

    valor_medio=valor_menor + (valor_maior - valor_menor) / 2;

    if(strcmp(palavra,grafo[valor_medio].pal)>0)
      valor_menor=valor_medio+1;
    else if(strcmp(palavra,grafo[valor_medio].pal)<0)
      valor_maior=valor_medio-1;
    else if(strcmp(palavra,grafo[valor_medio].pal)==0) {
      sucesso=1;
      return valor_medio;
    }
  }
  return -1;
}


/******************************************************************************
 * free_vector_grafos 
 * 
 * liberta a lista de adjacencias pertencente a cada elemeto da tabela a comecar
 * na posicao um, pois a posicao zero não contem lista. Liberta tambem memoria da
 * tabela
 * 
 *****************************************************************************/
void free_vector_grafos(item* vector_item_a_apagar,int n) {
  int i;
     
  for(i=1;i<n+1;i++) {
    free_lista(vector_item_a_apagar[i].prox);
    free(vector_item_a_apagar[i].pal);
  }
  free(vector_item_a_apagar);
  return;
}


/******************************************************************************
 * free_lista
 * 
 * funcao recursiva que se chama a si propria ate chegar ao fim da lista, e que
 * vai libertando os varios elementos da mesma
 * 
 *****************************************************************************/
void free_lista(item* elemento) {
  if(elemento==NULL)
    return;
  free_lista(elemento->prox);
  free(elemento->pal);
  free (elemento);
  return;
}


/******************************************************************************
 * recolhe_problema
 * 
 * 
 *****************************************************************************/
int recolhe_problema(FILE* f, char** pal1, char** pal2, int * n_mutacoes) {
  int a=0;
  char linha[60]="inicializacao";
  char temp1[30]="inicializacao";
  char temp2[30]="inicializacao";
  if(fgets(linha, sizeof(linha), f)) {
    a=sscanf(linha,"%s %s %d", temp1, temp2, n_mutacoes);
    if (a==3) {
      *pal1 = (char*) malloc (strlen(temp1) + 1);
      *pal2 = (char*) malloc (strlen(temp2) + 1);
      strncpy(*pal1, temp1, strlen(temp1) + 1);
      strncpy(*pal2, temp2, strlen(temp2) + 1);
      return 1; /*ainda ha problemas*/
    } else
      return 0; /*fim do ficheiro*/
  }

  return 0;
}


/******************************************************************************
 * escreve_solucao
 * 
 * escreve a solucao no ficheiro de saida para os casos em que e usado o algoritmo
 * (solucao nao imediata)
 * 
 *****************************************************************************/
void escreve_solucao(FILE* fp, item* grafo, int* solucao,int custo_final, int tamanho_caminho) {
  int i;
  
  fprintf(fp,"%s %d\n",grafo[solucao[0]].pal, custo_final);

  for (i=1; i<tamanho_caminho; i++) {
    fprintf(fp,"%s\n", grafo[solucao[i]].pal);
  }
  
  fprintf(fp,"\n");
    
  return;
}


/******************************************************************************
 * escreve_solucao2
 * 
 * escreve a solucao para os casos em que nao ha caminho entre as palavras pal1 e pal2
 *
 *****************************************************************************/
void escreve_solucao2(FILE* fp, char* palavra_inicial, char* palavra_final) {
  fprintf(fp,"%s -1\n%s\n\n",palavra_inicial, palavra_final);
}


/******************************************************************************
 * cria_nome_saida
 * 
 * recebe o nome do ficheiro que contem os problemas e aplica a extencao desejada.
 * Retorna uma apontador para o novo nome
 * 
 *****************************************************************************/
char* cria_nome_saida(char* nome_entrada) {
  char* ext1 = ".path";
  char* nome_final = (char *) calloc  (strlen(nome_entrada)+2, sizeof(char));
  strncpy(nome_final, nome_entrada, strlen(nome_entrada)-4);
  strcat(nome_final, ext1);

  return nome_final;
}


/******************************************************************************
 * escreve_solucao3
 * 
 * imprime no ficheiro de saida o resultado de as palavras pal1 e pal2 serem iguais
 * 
 *****************************************************************************/
void escreve_solucao3(FILE* fp,char* pal1,char* pal2) {
  fprintf(fp,"%s 0\n%s\n\n", pal1,pal2);
}


/******************************************************************************
 * escreve_solucao4
 * 
 * imprime no ficheiro de saida o resultado de as palavras pal1 e pal2 terem
 * tamanho um (custo imediato = 1)
 * 
 *****************************************************************************/
void escreve_solucao4(FILE* fp,char* pal1,char* pal2) {
  fprintf(fp,"%s 1\n%s\n\n", pal1,pal2);
}
