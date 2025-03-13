#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structs.h"
#include "quickext.h"

// Função para gerar o arquivo temporário com os dados
void geraArquivo(FILE* arq, FILE* temp, int quantidade) {
    Aluno aux;
    for (int i = 0; i < quantidade; i++) {
        fscanf(arq, "%ld %f ", &aux.matricula, &aux.nota);
        fgets(aux.estado, 3, arq);
        fseek(arq, 1, 1);
        fgets(aux.cidade, 50, arq);
        fseek(arq, 2, 1);
        fgets(aux.curso, 30, arq);
        fwrite(&aux, sizeof(Aluno), 1, temp);
    }
}

// Função para imprimir o conteúdo do arquivo
void ImprimirArquivo(FILE* Arq) {
    Aluno aluno;
    fseek(Arq, 0, SEEK_SET);
    printf("=== Conteúdo do arquivo ===\n");
    while (fread(&aluno, sizeof(Aluno), 1, Arq)) {
        printf("Matrícula: %ld | Nota: %.1f\n", aluno.matricula, aluno.nota);
    }
    printf("==========================\n");
}

// Função para salvar os dados ordenados em um arquivo
void salvarEmArquivo(FILE* temp, const char* nomeArquivo) {
    FILE* saida = fopen(nomeArquivo, "w");
    if (!saida) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }

    Aluno aux;
    fseek(temp, 0, SEEK_SET);
    while (fread(&aux, sizeof(Aluno), 1, temp)) {
        fprintf(saida, "%08ld %.1f %s %s %s\n", aux.matricula, aux.nota, aux.estado, aux.cidade, aux.curso);
    }

    fclose(saida);
}

int main() {
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

    // Gera o arquivo temporário com os dados lidos
    geraArquivo(arq, temp, quantidade);
    fseek(temp, 0, SEEK_SET);

    // Solicita o tipo de ordenação ao usuário
    char tipoOrdenacao[20];
    printf("Digite a ordenação (ascendente, descendente, aleatorio): ");
    scanf("%s", tipoOrdenacao);

    // Lógica de ordenação e exibição
    if (strcmp(tipoOrdenacao, "ascendente") == 0) {
        printf("Ordenando de forma ascendente...\n");

        // Realiza a ordenação ascendente
        FILE* arquivoEscritaBaixo = fopen("temp.bin", "r+b"); // Parte esquerda
        FILE* arquivoEscritaAlto = fopen("temp.bin", "r+b");  // Parte direita
        FILE* arquivoCentral = fopen("temp.bin", "r+b");      // Parte central

        if (!arquivoEscritaBaixo || !arquivoEscritaAlto || !arquivoCentral) {
            perror("Erro ao abrir os arquivos auxiliares");
            fclose(temp);
            fclose(arq);
            exit(EXIT_FAILURE);
        }

        // Chama o Quicksort Externo (ordenação ascendente)
        QuickSortExterno(&arquivoCentral, &arquivoEscritaBaixo, &arquivoEscritaAlto, 1, quantidade);

        fclose(arquivoEscritaBaixo);
        fclose(arquivoEscritaAlto);
        fclose(arquivoCentral);

        // Salva no arquivo
        salvarEmArquivo(temp, "provaoascendente.txt");
    }
    else if (strcmp(tipoOrdenacao, "descendente") == 0) {
        printf("Ordenando de forma descendente...\n");

        // Realiza a ordenação descendente
        FILE* arquivoEscritaBaixo = fopen("temp.bin", "r+b"); // Parte esquerda
        FILE* arquivoEscritaAlto = fopen("temp.bin", "r+b");  // Parte direita
        FILE* arquivoCentral = fopen("temp.bin", "r+b");      // Parte central

        if (!arquivoEscritaBaixo || !arquivoEscritaAlto || !arquivoCentral) {
            perror("Erro ao abrir os arquivos auxiliares");
            fclose(temp);
            fclose(arq);
            exit(EXIT_FAILURE);
        }

        // Chama o Quicksort Externo (ordenação descendente)
        QuickSortExterno(&arquivoCentral, &arquivoEscritaBaixo, &arquivoEscritaAlto, 1, quantidade);

        fclose(arquivoEscritaBaixo);
        fclose(arquivoEscritaAlto);
        fclose(arquivoCentral);

        // Salva no arquivo
        salvarEmArquivo(temp, "provaodescendente.txt");
    }
    else if (strcmp(tipoOrdenacao, "aleatorio") == 0) {
        printf("Os dados estão em ordem aleatória...\n");

        // Apenas salva no arquivo aleatório sem ordenação
        salvarEmArquivo(temp, "provaoaleatorio.txt");
    } else {
        printf("Opção inválida. Tente novamente.\n");
        fclose(arq);
        fclose(temp);
        return 1;
    }

    // Exibe os registros ordenados
    fseek(temp, 0, SEEK_SET);
    Aluno aux;
    while (fread(&aux, sizeof(Aluno), 1, temp)) {
        printf("Matrícula: %08ld | Nota: %.1f\n", aux.matricula, aux.nota);
    }

    // Fecha os arquivos abertos
    fclose(arq);
    fclose(temp);

    return 0;
}
