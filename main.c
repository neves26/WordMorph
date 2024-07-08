#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main(int argc, char** argv) {

  FILE *fproblemas, *fsaida = NULL; /*apontadores para os ficheiros .pal e saida respetivamente*/
  char *apt, *pal1, *pal2 = NULL;
  int custo_final, n_mutacoes, i, tamanho_caminho_final, j, tamanho = -1, *sol = NULL;

  fproblemas = fopen(argv[2], "r");
  apt = cria_nome_saida(argv[2]);
  fsaida = fopen(apt, "w");

  item*** vector_grafos = (item***)malloc(28*sizeof(item*)); /*vetor para matriz de apontadores para item*/
  
  for(i=0; i<28; i++) { /*alocacao de memoria para a matriz de apontadores*/
    vector_grafos[i] = (item**)malloc(28*sizeof(item*));
    for(j=0; j<28; j++)
      vector_grafos[i][j] = NULL;
  }

  while(recolhe_problema(fproblemas, &pal1, &pal2, &n_mutacoes)) { /*enquanto houver problemas no ficheiro .pal, o ciclo while esta ativo*/
    tamanho = strlen(pal1);

    if(n_mutacoes>tamanho)  /*caso o numero de mutacoes seja maior que tamanho maximo da palavra, o numero de mutacoes fica igual tamanho das palavras iniciais*/
      n_mutacoes=tamanho;

    if(strcmp(pal1,pal2)==0) /*caso em que a palavra inicial e final sao iguais*/
      escreve_solucao3(fsaida,pal1,pal2);
    else if(calculate_hamming_dist(pal1,pal2,1,tamanho)==1) /*caso em que a palavra inicial diferem em apenas um caracter, custo minimo imediato de 1*/
      escreve_solucao4(fsaida,pal1,pal2);
    else if(tamanho==1) /*caso sejam palavras de tamanho 1*/
      escreve_solucao4(fsaida,pal1,pal2);
    else if(n_mutacoes>0) { /*casos gerais onde e necessario aplicar o algoritmo*/
      sol=resolucao_problema(&pal1, &pal2, argv[1],tamanho, n_mutacoes, vector_grafos, &custo_final, &tamanho_caminho_final); /*funcao que retorna a solucao ao problema*/

      if(sol!=NULL) { /*caso haja solucao, sera imprimida, e memoria do vetor solucao libertada*/
        escreve_solucao(fsaida,vector_grafos[tamanho-1][n_mutacoes],sol,custo_final,tamanho_caminho_final);
        free(sol);
      } else { /*caso nao haja solucao */
        escreve_solucao2(fsaida,pal1,pal2);
      }
    } else {
      exit(0);
    }
    free(pal1); /*libertacao de memoria das palavras, apontadores sao reutilizados para problemas seguintes*/
    free(pal2);
  }


  /*libertacao de memoria das listas de adjacencias nas tabelas, dos items nas tabelas e por fim dos apontadores para tabela de item alocados na matriz*/
  for(i=0; i<28; i++) {
    for(j=0; j<28; j++) {
      if(vector_grafos[i][j]!=NULL)
        free_vector_grafos(vector_grafos[i][j],vector_grafos[i][j][0].cost);
      else
        free(vector_grafos[i][j]);
    }
  }
  /*libertacao de memoria da matriz de apontadores para item*/
  for(i=0; i<28; i++)
    free(vector_grafos[i]);

  free(vector_grafos); /*libertacao de memoria do apontador para a matriz de apontadores para item*/
  fclose(fproblemas); 
  fclose(fsaida);
  free(apt);
  return 0;
}
