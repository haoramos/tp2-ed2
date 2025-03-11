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
    printf("\n%08ld %.1f %s %s %s\n", aux.matricula, aux.nota, aux.estado, aux.cidade, aux.curso);
    fwrite(&aux, sizeof(Aluno), 1, temp);
  }
}

void ImprimirArquivo(FILE *Arq) {
    Aluno aluno;
    fseek(Arq, 0, SEEK_SET);
    printf("=== Conteúdo do arquivo ===\n");
    while (fread(&aluno, sizeof(Aluno), 1, Arq)) {
        printf("Matrícula: %d | Nota: %.1f\n", aluno.matricula, aluno.nota);
    }
    printf("==========================\n");
}

int main(int argc, char const *argv[]) {
    int metodo = 3; // Método de ordenação (3 = Quicksort Externo)
    int quantidade = 20; // Número de registros a serem lidos

    FILE* temp = fopen("temp.bin", "w+b");
    if (!temp) {
        perror("Erro ao abrir o arquivo temp.bin");
        return 1;
    }

    FILE* arq = fopen("PROVAO.TXT", "r");
    if (!arq) {
        perror("Erro ao abrir o arquivo PROVAO.TXT");
        fclose(temp);
        return 1;
    }

    geraArquivo(arq, temp, quantidade);
    fseek(temp, 0, SEEK_SET);

    ImprimirArquivo(temp);

    switch (metodo) {
        case 3:
        printf("=== QuickSort Externo ===\n");
        printf("entrou\n");
        fflush(stdout);

        // Abre o mesmo arquivo três vezes para leitura e escrita
        FILE* arquivoEscritaBaixo = fopen("temp.bin", "r+b"); // Parte esquerda
        FILE* arquivoEscritaAlto = fopen("temp.bin", "r+b");  // Parte direita
        FILE* arquivoCentral = fopen("temp.bin", "r+b");      // Parte central

        if (!arquivoEscritaBaixo || !arquivoEscritaAlto || !arquivoCentral) {
            perror("Erro ao abrir os arquivos auxiliares");
            fclose(temp);
            fclose(arq);
            exit(EXIT_FAILURE);
        }

        // Chama o Quicksort Externo
        QuicksortExterno(&arquivoCentral, &arquivoEscritaBaixo, &arquivoEscritaAlto, 0, quantidade - 1);

        // Fecha os arquivos
        fclose(arquivoEscritaBaixo);
        fclose(arquivoEscritaAlto);
        fclose(arquivoCentral);
        break;

        default:
        printf("Método inválido!\n");
    }

    // Exibe os registros ordenados
    fseek(temp, 0, SEEK_SET);
    Aluno aux;
    while (fread(&aux, sizeof(Aluno), 1, temp)) {
        printf("Matrícula: %08ld | Nota: %.1f\n", aux.matricula, aux.nota);
    }

    fclose(arq);
    fclose(temp);

    return 0;
}
  