#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "quickext.h"

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

  int metodo = 3;//atoi(argv[1]);
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
  rewind(temp);
  
  switch (metodo)
  {
    case 1:
      printf("=== Intercalação balanceada de vários caminhos (2f fitas) ===\n");
      break;
        
    case 2:
      printf("=== Intercalação balanceada de vários caminhos (2f fitas) ===\n");
      break;
      
      case 3:
      printf("=== QuickSort Externo ===");
      printf("\nentrou");
      fflush(stdout);
      
      FILE* arquivoEscritaBaixo = fopen("a1.bin", "wb+"); // Arquivo auxiliar inferior
      FILE* arquivoEscritaAlto = fopen("a2.bin", "wb+"); // Arquivo auxiliar superior
      
      if (!arquivoEscritaBaixo || !arquivoEscritaAlto) {
        perror("Erro ao abrir os arquivos auxiliares");
        exit(EXIT_FAILURE);
      }
      
      QuicksortExterno(&temp, &arquivoEscritaBaixo, &arquivoEscritaAlto, 0, quantidade - 1);
      
      fflush(temp);
      fclose(arquivoEscritaBaixo);
      fclose(arquivoEscritaAlto);
      break;
        
    default:
      printf("Método inválido!\n");
  }


  Aluno aux;
  while(fread(&aux, sizeof(Aluno), 1, temp)) printf("Nota: %.1f\n", aux.nota);

  fclose(arq);
  fclose(temp);
    
  return 0;
}


