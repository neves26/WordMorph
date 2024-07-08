#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "funcoes.h"

/******************************************************************************
 * calculate_hamming_dist
 *
 * Funcao que recebe duas strings, o numero de mutacoes maximo e o tamanho da palavra
 * e calcula o numero de caracteres diferentes (= hamming distance).
 *
 *****************************************************************************/
int calculate_hamming_dist(char* str1, char* str2,int n_mutacoes_max,int tamanho_palavra) {
  int i, dist = 0;

  for(i=0; i<tamanho_palavra; i++) {
    if(str1[i]!=str2[i]) {
      dist++;
      if(dist>n_mutacoes_max)
        return INT_MAX;
    }
  }
  return dist;
}

/******************************************************************************
 * cria_lista_adj
 * 
 * Adiciona um novo item a cada lista de adjacencia de cada palavra na tabela.
 * Usa a funcao de calculo da hamming distance para saber se as palavraas sao adjacentes.
 * 
 *****************************************************************************/
void cria_lista_adj(item* tabela, int num_pal_tabela, int n_mutacoes_max,int tamanho_palavra) {
  int i, j, p, n_mutacoes = 0;

  for(i = 1; i < num_pal_tabela + 1; i++)
    for(j = i + 1; j < num_pal_tabela + 1; j++) {
      n_mutacoes = calculate_hamming_dist(tabela[i].pal,tabela[j].pal,n_mutacoes_max,tamanho_palavra);

      if(n_mutacoes <= n_mutacoes_max) {
        p++;
        new_item(&tabela[i],j, n_mutacoes, tamanho_palavra);
        new_item(&tabela[j],i, n_mutacoes, tamanho_palavra);
      }
    }
}

/******************************************************************************
 * new_item
 * 
 * Aloca memoria para um novo item, inicializa com os valores do item a inserir
 * e adiciona o item ao fim da lista.
 * 
 *****************************************************************************/
void new_item(item* topo, int posicao, int n_mutacoes, int tamanho_palavra) {
  item* temp = (item*) malloc(sizeof(item));
  temp->pal=NULL;

  temp->cost = (n_mutacoes * n_mutacoes);
  temp->posicao = posicao;
  temp->prox = NULL;

  item* aux = topo;

  if(aux->prox == NULL)
    aux->prox = temp;

  else {
    while(aux->prox != NULL)
      aux = aux->prox;
    aux->prox = temp;

  }
}