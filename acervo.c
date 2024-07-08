#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "funcoes.h"

/******************************************************************************
 * dijkstra
 *
 *  Argumentos:
 *              item* grafo - É a nossa tabela com items, quando chega a esta função já tem a lista de adjacências
 *              int palavra_inicial - Posição no grafo da palavra inicial do problema
 *              int palavra_final - Posição no grafoda segunda palavra do problema
 *              int* custo final - É um inteiro que é passado por referência para retornar o custo total do caminho entre as duas palavra
 *                                 É retornado até ao main onde é enviado para uma das funções de escrever solução
 *              int* tamanho_caminho_final - É um inteiro que é passado por referência para retornar o numero de palavras do caminho, ou seja desde a palavra inicial até à palavra final
 *                                          É retornado até ao main onde é enviado para uma das funções de escrever solução
 *
 *
 *  O Dijkstra é a a função que obtem o menor caminhos entre entre dois vértices no grafo, "palavra_inicial" e palavra_final.
 *  O dijkstra usa como base um acervo que nós chamamos de heap do tipo de estrutura acervo*.
 *  A prioridade no acervo é dada pelo custo de cada elemento. O custo é a distancia do vértice à palavra inicial.
 *  O custo é guardado no vector de inteiros "dist" da estrutura caminho, na funcao usamos caminho* c1.
 *  O heap guarda todas as palavras do grafo inicialmente com custo infinito (INT_MAX) excepto a palavra inicial que é guardada com custo 0.
 *
 *  CICLO:
 *  A função retira a palavra com menor custo do acervo, pois essa palavra corresponde ao menor caminho possivel, e vai actualizar os custos das palavras que lhe forem adjacentes no grafo.
 *  Assim, como temos uma lista de adjacências, quando retiramos a palavra com menor custo vamos percorrer a lista de adjacencias dessa palavra.
 *  Para cada adjacente se o custo entre a palavra removida e ele for menor que o guardado no acervo, esse custo é actualizado.
 *  Sempre que se altera o custo de um elemento do acervo fazemos chamamos a funcao "organiza_no" para reordenar o acervo.
 *  Guardamos também para cada adjacente a posiçao da palavra removida para sabermos qual o elemento anterior do caminho. Isto é guardado no "elemento_anterior" de "c1" da estrutura caminho correspondente ao adjacente.
 *
 * Depois de remover a palavra com menor custo do acervo vamos verificar se essa palavra é a palavra final (encontrámos solução).
 * Ou se essa palavra tem custo INT_MAX (nao há soluçao, porque ficamos sem mais caminhos possiveis).
 *
 * Retorna:
 *          solucao - Vector de inteiros com as posições no grafo dos elementos do caminho desde a palavra inicial até à final. SE ENCONTRAR A PALAVRA FINAL.
 *          NULL - SE NAO ENCONTRAR A PALAVRA FINAL
 *
 *
 *****************************************************************************/
int* dijkstra( item* grafo, int palavra_inicial, int palavra_final,int* custo_final,int* tamanho_caminho_final) {
  int n_vertices = 1+grafo[0].cost;

caminho* c1 = (caminho*) malloc(n_vertices*sizeof(caminho));

  int i;

  acervo* heap = cria_acervo(n_vertices);

  for ( i = 0; i < n_vertices; ++i) {
    c1[i].dist = INT_MAX;
    c1[i].elemento_anterior = -1;

    heap->vector_acervo[i] = novo_no_acervo(i, c1[i].dist);     /*O vector_acervo guarda os nós (no_acervo) que têm a informação do grafo, a sua posição (vértice) e a sua distancia*/
    heap->posicao_acervo[i] = i;                                /*Posicao dos elementos no acervo*/
  }


  heap->posicao_acervo[palavra_inicial]   = palavra_inicial;
  c1[palavra_inicial].dist = 0;
  c1[palavra_inicial].elemento_anterior=-1;
  organiza_no(heap, palavra_inicial, c1[palavra_inicial].dist);


  heap->n_vertices_actual = n_vertices;



  while (!verifica_acervo(heap)) {      /*Enquanto houver elementos no acervo*/

    no_acervo* node = remove_minimo(heap);


    int palavra_removida = node->vertice;


    if(c1[palavra_removida].dist==INT_MAX)  /*Se a palavra removida tiver custo infinito, ou seja nao encontrámos caminho para a palavra final*/
    {
        int j;
        for(j=0;j<n_vertices;j++)
        {
            free((heap->vector_acervo[j]));
        }

        free(heap->vector_acervo);
        free(heap->posicao_acervo);
        free(heap);
        free(c1);
        return NULL;
    }
    if (palavra_removida == palavra_final)  /*Se a palavra removida for a palavra final, cria o vector solução onde guarda os elementos do caminho, e retorna-o*/
    {

        *custo_final=c1[palavra_removida].dist;





        int inc=0;
        int s=palavra_removida;
        while(c1[s].elemento_anterior!=-1)
        {
            inc++;
            s=c1[s].elemento_anterior;
        }
        inc++;
        *tamanho_caminho_final=inc;
        int* solucao = (int*)malloc(inc*sizeof(int));
        solucao[inc-1]=palavra_removida;

        s=palavra_removida;
        inc=inc-2;
        while(c1[s].elemento_anterior!=-1)
        {

            solucao[inc]=c1[s].elemento_anterior;
            inc--;
            s=c1[s].elemento_anterior;
        }


        int j;
        for(j=0;j<n_vertices;j++)
        {
            free((heap->vector_acervo[j]));
        }

        free(heap->vector_acervo);
        free(heap->posicao_acervo);
        free(heap);
        free(c1);
        return (solucao);

    }

    item* aux = grafo[palavra_removida].prox;
    while (aux != NULL) {                           /*Percorre lista de adjacências*/
      int auxp = aux->posicao;

      /*Se o adjacente ainda pertencer ao acervo, a sua distancia nao for infinita e o seu novo custo for menor que o guardado*/
      if (pertence_acervo(heap, auxp) && c1[palavra_removida].dist != INT_MAX && aux->cost + c1[palavra_removida].dist < c1[auxp].dist)
        {
        c1[auxp].dist = c1[palavra_removida].dist + aux->cost;  /*Actualiza o custo*/
        c1[auxp].elemento_anterior=palavra_removida;            /*Actualiza o elemento anterior*/

        organiza_no(heap, auxp, c1[auxp].dist);                 /*Reordenaçao do acervo*/
        }
      aux = aux->prox;
    }
  }

    /*Se o acervo tiver ficado vazio*/
    int j;
    for(j=0;j<n_vertices;j++)
    {
        free((heap->vector_acervo[j]));
    }
    free(heap->vector_acervo);
    free(heap->posicao_acervo);
    free(heap);
    free(c1);
    return NULL;
}


/******************************************************************************
 * novo_no_acervo
 *
 *  É chamada no inicio do dijkstra para preencher o vector_acervo com os nós.
 *
 *  Aloca memória para um novo nó (no_acervo).
 *  Guarda o vértice, que é a posição da palavra no grafo e a sua distancia.
 *
 *  Retorna o nó que alocou.
 *
 *****************************************************************************/
no_acervo* novo_no_acervo(int v, int d) {
  no_acervo* noAcervo = ( no_acervo*) malloc(sizeof(no_acervo));
  noAcervo->vertice = v;
  noAcervo->distancia = d;
  return noAcervo;
}

/******************************************************************************
 * cria_acervo
 *
 *  É chamada no inicio do dijkstra para criar o acervo.
 *
 *  Aloca memória para o acervo  e para os seus vectores.
 *
 *  Retorna o apontador para acervo que alocou.
 *
 *****************************************************************************/
acervo* cria_acervo(int tamanho_maximo)
{
  acervo* temp = (acervo*)malloc(sizeof(acervo));
  temp->posicao_acervo = (int *)malloc(tamanho_maximo * sizeof(int));
  temp-> n_vertices_actual = 0;
  temp->tamanho_maximo = tamanho_maximo;
  temp->vector_acervo = (no_acervo**)malloc(tamanho_maximo*sizeof(no_acervo*));
  return temp;
}

/******************************************************************************
 * troca_no_acervo
 *
 *  Troca dois nós do acervo
 *
 *  É chamada nas funções de ordenação do acervo, ordena_acervo e organiza_no.
 *
 *****************************************************************************/
void troca_no_acervo(no_acervo** a, no_acervo** b) {
  no_acervo* t = *a;
  *a = *b;
  *b = t;
}

/******************************************************************************
 * ordena_acervo
 *
 *  É o nosso heapify.
 *  Recebe o acervo e o primeiro elemento.
 *  Assume que as duas sub-árvores são acervos e vai descendo o primeiro elemento para a direita ou para a esquerda, até a condição de acervo estar satisfeita.
 *
 *
 *****************************************************************************/
void ordena_acervo(acervo* acervo, int indice) {
  int menor, esq, dir;
  menor = indice;
  esq = 2 * indice + 1;
  dir = 2 * indice + 2;

  if(esq < acervo->n_vertices_actual && acervo->vector_acervo[esq]->distancia < acervo->vector_acervo[menor]->distancia)
    menor = esq;

  if(dir < acervo->n_vertices_actual && acervo->vector_acervo[dir]->distancia < acervo->vector_acervo[menor]->distancia)
    menor = dir;

  if(menor != indice) {
    no_acervo *menorNo = acervo->vector_acervo[menor];
    no_acervo *indiceNo = acervo->vector_acervo[indice];

    acervo->posicao_acervo[menorNo->vertice] = indice;
    acervo->posicao_acervo[indiceNo->vertice] = menor;

    troca_no_acervo(&acervo->vector_acervo[menor], &acervo->vector_acervo[indice]);

    ordena_acervo(acervo, menor);
  }
}

/******************************************************************************
 * verifica_acervo
 *
 *  Verifica se ainda há elementos no acervo, ou seja se o numero de vértices que estamos a utilizar no acervo é maior que zero.
 *
 *****************************************************************************/
int verifica_acervo(acervo* acervo) {
  return acervo->n_vertices_actual == 0;
}

/******************************************************************************
 * remove_minimo
 *
 *  Se o acervo estiver vazio retorna NULL.
 *  Senão troca o primeiro elemento do acervo com o último e decrementa o numero de vertices a utilizar no acervo
 *  Ordena o acervo, chamando a função ordena_acervo
 *
 *****************************************************************************/
no_acervo* remove_minimo(acervo* acervo) {
  if (verifica_acervo(acervo))
    return NULL;

  no_acervo* menor = acervo->vector_acervo[0];

  no_acervo* no_final = acervo->vector_acervo[acervo->n_vertices_actual-1];
  acervo->vector_acervo[0] = no_final;
  acervo->vector_acervo[acervo->n_vertices_actual-1] = menor;

  acervo->posicao_acervo[menor->vertice] = acervo->n_vertices_actual-1;
  acervo->posicao_acervo[no_final->vertice] = 0;

  acervo->n_vertices_actual--;
  ordena_acervo(acervo, 0);

  return menor;
}

/******************************************************************************
 * organiza_no
 *
 *  Enquanto a condição de acervo nao estiver satisfeita vai trocar o nó com o seu pai, no acervo.
 *
 *****************************************************************************/
void organiza_no(acervo* acervo, int v, int d) {

  int i = acervo->posicao_acervo[v];

  acervo->vector_acervo[i]->distancia = d;

  while (i && acervo->vector_acervo[i]->distancia < acervo->vector_acervo[(i - 1) / 2]->distancia) {

    acervo->posicao_acervo[acervo->vector_acervo[i]->vertice] = (i-1)/2;
    acervo->posicao_acervo[acervo->vector_acervo[(i-1)/2]->vertice] = i;
    troca_no_acervo(&acervo->vector_acervo[i],  &acervo->vector_acervo[(i - 1) / 2]);

    i = (i - 1) / 2;
  }
}

/******************************************************************************
 * pertence_acervo
 *
 *  Compara a posiçao dos vértice recebido, com o tamanho que o acervo tem actualmente.
 *  Se a sua posição for maior que o numero de vértices actual, significa que nao pertence ao acervo.
 *
 *  Retorna 'true' se o vértice pertencer ao acervo.
 *  Retorna 'false' se o vértice não pertencer ao acervo.
 *
 *****************************************************************************/
bool pertence_acervo(acervo *acervo, int v) {
  if (acervo->posicao_acervo[v] < acervo->n_vertices_actual)
    return true;
  return false;
}
