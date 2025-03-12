#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "intercalacao.h"
#include "heapsort.h"

void ImprimirArquivo(FILE *Arq) {
    Aluno aluno;
    fseek(Arq, 0, SEEK_SET);
    printf("=== Conteúdo do arquivo ===\n");
    while (fread(&aluno, sizeof(Aluno), 1, Arq)) {
        printf("Matrícula: %ld | Nota: %.1f\n", aluno.matricula, aluno.nota);
    }
    printf("==========================\n");
    fseek(Arq, 0, SEEK_SET);
}

int todosMarcados(Aluno registros[]) {
    for (int i = 0; i < F; i++) {
        if (registros[i].matricula != -1 && registros[i].nota < 100) {
            return 0; // Ainda há registros não marcados
        }
    }
    return 1; // Todos marcados
}


// Função que retorna o indice do menor valor, e -1 se não houver registros válidos
short menorIndice(Aluno registros[]) {
    short menorIdx = -1;

    for (int i = 0; i < F; i++) {
        if (registros[i].matricula != -1) {
            if (menorIdx == -1 || registros[i].nota < registros[menorIdx].nota) {
                menorIdx = i;
            }
        }
    }

    return menorIdx;
}

void reescreverArquivoTemp(FILE* fitasSaida[F], FILE** temp) {
    FILE* fitaSaidaFinal = fitasSaida[0];  // A fita que contém os dados ordenados
    Aluno registro;
    // Reabre o arquivo temp para sobrescrever
    fseek(*temp, 0, SEEK_SET);  // "NULL" faz com que o arquivo temp seja reaberto no modo de escrita (binária)
    
    // Lê os registros da fitaSaida[0] e escreve no arquivo temp
    while (fread(&registro, sizeof(Aluno), 1, fitaSaidaFinal) == 1) {
        fwrite(&registro, sizeof(Aluno), 1, *temp);
    }
    ImprimirArquivo(*temp);
}

// Função que desmarca todos os valores válidos do vetor
void desmarca(Aluno v[]){
    for(int i = 0; i < F; i++) {
        if (v[i].matricula > 100) v[i].nota -= 101;
    }
}

// Geração dos blocos
void geraBlocos2f(FILE* temp, Aluno mem[], FILE* fitasEntrada[F]) {
    int fitaAtual;
    Aluno prox;

    //Preenche a memoria interna com os F primeiros dados
    for (int i = 0; i < F; i++) {
        if (fread(&mem[i], sizeof(Aluno), 1, temp) != 1) {
            printf("Erro ao ler do arquivo temp. i=%d\n", i);
            exit(1);
        }
    }

    // Lê o próximo registro para comparar com o que sai
    short temProx = fread(&prox, sizeof(Aluno), 1, temp);

    // Enquanto não chegar ao final do arquivo 'temp'
    while(temProx || !todosMarcados(mem)) {
        fitaAtual = 0;
        
        // Enquanto a fita atual não chegar ao máximo de fitas de entrada, continue o loop
        while(fitaAtual < F) {
            
            // Enquanto os todos os valores da memoria interna não estiverem marcados, efetua a geração dos blocos na fita atual
            while(todosMarcados(mem) != 1) {
                // Ordena os valores na memória principal usando o HeapSort
                heapSortMarcados(mem, F);

                // Compara a nota de prox com a primeira da memoria principal
                if (temProx && prox.nota < mem[0].nota) {
                    // Registro marcado
                    prox.nota += 101;
                }
                
                // Escreve o menor registro na fita de entrada atual
                fwrite(&mem[0], sizeof(Aluno), 1, fitasEntrada[fitaAtual]);
                
                // Substitui pelo próximo registro ou um "vazio"
                if (temProx) {
                    mem[0] = prox;
                    temProx = fread(&prox, sizeof(Aluno), 1, temp);
                } else {
                    memset(&mem[0], 0, sizeof(Aluno));
                    mem[0].matricula = -1;  // Marca como menor valor possível
                }
            }

            // Quando todos os valores da memoria interna estiverem marcados, avança para a próxima fita
            fitaAtual++;
            
            // printf("\nPASSOU EM DESMARCA\n");
            // Desmarca todos os registros na memória principal antes de passar para a próxima fita
            desmarca(mem);
        }

        // Quando preenche todas as fitas com bloco, insere um registro para marcar o fim de um bloco
        for (int i = 0; i < F; i++) {
            Aluno marcador;
            memset(&marcador, 0, sizeof(Aluno));  // Preenche toda a struct com 0
            marcador.matricula = -1;  
            fwrite(&marcador, sizeof(Aluno), 1, fitasEntrada[i]);
        }
    }
    
    for (int i = 0; i < F; i++) {
        printf("\n fita_entrada_%d: \n", i);
        ImprimirArquivo(fitasEntrada[i]);
    }
}

void intercalaBlocos(FILE* fitasEntrada[F], FILE* fitasSaida[F]) {
    Aluno registros[F];
    int fitasAtivas = F;

    // Inicializa a leitura das fitas de entrada
    for (int i = 0; i < F; i++) {
        if (fread(&registros[i], sizeof(Aluno), 1, fitasEntrada[i]) != 1) {
            registros[i].matricula = -1; // Marca a fita como inativa
            fitasAtivas--;
        }
    }

    while (fitasAtivas > 0) {
        while (fitasAtivas > 0) {
            
            int menorIdx = menorIndice(registros);
            // Se não há mais registros válidos, finaliza o bloco
            if (menorIdx == -1) break;

            // Escreve o menor registro na fita de saída atual (sempre fitaSaida[0])
            fwrite(&registros[menorIdx], sizeof(Aluno), 1, fitasSaida[0]);

            // Lê o próximo registro da fita correspondente
            if (fread(&registros[menorIdx], sizeof(Aluno), 1, fitasEntrada[menorIdx]) != 1) {
                registros[menorIdx].matricula = -1; // Marca a fita como esgotada
                fitasAtivas--;
            }
        }

        // Marca o fim do bloco na fita de saída
        Aluno marcador = { .matricula = 0 };  // Final de bloco
        fwrite(&marcador, sizeof(Aluno), 1, fitasSaida[0]);

        // Se todas as fitas de saída foram preenchidas, troca os papéis
        if (fitasAtivas == 0) {
            FILE* temp[F];
            memcpy(temp, fitasEntrada, sizeof(temp));
            memcpy(fitasEntrada, fitasSaida, sizeof(temp));
            memcpy(fitasSaida, temp, sizeof(temp));

            // Reinicializa as fitas ativas para um novo ciclo
            fitasAtivas = F;
            
            // Recarrega os primeiros registros das novas fitas de entrada
            for (int i = 0; i < F; i++) {
                if (fread(&registros[i], sizeof(Aluno), 1, fitasEntrada[i]) != 1) {
                    registros[i].matricula = -1;
                    fitasAtivas--;
                }
            }
        }
    }

    for (int i = 0; i < F; i++) {
        printf("\n fita_saida_%d: \n", i);
        ImprimirArquivo(fitasSaida[i]);
    }
    
    // No final, todos os dados ordenados estão em fitasSaida[0] (ou na fita que você definir como saída)
    // Para garantir que tudo fique na fita final, basta garantir que o fluxo de gravação seja contínuo até essa fita.
}

void intercalacao2f(FILE** temp) {
    Aluno memoria[F];

    ImprimirArquivo(*temp);

    // 2f Fitas = 10 entrada / 10 saída
    FILE* fitasEntrada[F];
    FILE* fitasSaida[F];

    // Abre as fitas de entrada e saída
    for (int i = 0; i < F; i++) {
        char nomeEntrada[20], nomeSaida[20];
        sprintf(nomeEntrada, "fita_entrada_%d.bin", i);
        sprintf(nomeSaida, "fita_saida_%d.bin", i);

        fitasEntrada[i] = fopen(nomeEntrada, "wb+");
        fitasSaida[i] = fopen(nomeSaida, "wb+");

        if (!fitasEntrada[i] || !fitasSaida[i]) {
            printf("Erro ao abrir arquivos de fita!\n");
            exit(1);
        }
    }
    
    printf("tenta gerar");
    // Lê os registros das fitas de entrada
    geraBlocos2f(*temp, memoria, fitasEntrada);
    printf("gera");
    // Intercala os blocos gerados nas fitas de saída
    printf("tenta intercalar");
    intercalaBlocos(fitasEntrada, fitasSaida);
    printf("intercala");


    // Após gerado o arquivo ordenado em fitasSaida[0], passa novamente para temp
    printf("tenta reescrever");
    reescreverArquivoTemp(fitasSaida, temp);
    printf("reescreve");

    // Fecha os arquivos após o uso
    for (int i = 0; i < F; i++) {
        fclose(fitasEntrada[i]);
        fclose(fitasSaida[i]);
    }
}