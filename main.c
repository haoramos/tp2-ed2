#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void geraArquivo(FILE* arq, FILE* temp, int quantidade){
  Aluno aux;
  for (int i = 0; i < quantidade; i++){
    fscanf(arq, "%ld %f ", &aux.matricula, &aux.nota);
    fgets(aux.estado, 3, arq);
    fseek(arq, 1, 1);
    fgets(aux.cidade, 50, arq);
    fseek(arq, 2, 1);
    fgets(aux.curso, 30, arq);
    // fseek(arq, 1, 1);
    printf("\n%08ld %.1f %s %s %s\n", aux.matricula, aux.nota, aux.estado, aux.cidade, aux.curso);
    fwrite(&aux, sizeof(Aluno), 1, temp);
  }
}


int main(int argc, char const *argv[])
{
  // if(argc < 4) {
  //   printf("Usage: %s <input_file> <output_file> <num_threads>\n", argv[0]);
  //   return 1;
  // }

  // int metodo = atoi(argv[1]);
  int quantidade = 10;
  // int situacao = atoi(argv[3]);
  // int imprimir = (argc > 4 && strcmp(argv[4], "-P") == 0);

  FILE* temp = fopen("temp.bin", "w+b");
  FILE* arq = fopen("PROVAO.TXT", "r");
  if (!arq)
  {
    printf("Falha ao abrir o arquivo\n");
    return 1;
  }
  
  geraArquivo(arq, temp, quantidade);

  FILE* fitasEntrada[19];
  FILE* fitasSaida;
  fclose(arq);
  fclose(temp);
  
  return 0;
}


