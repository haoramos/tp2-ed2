#include <stdio.h>
#include <stdlib.h>

#include "quickext.h"

// Função de Partição (Quicksort Externo)
void Particao(FILE **arquivoLimiteInferior, FILE **arquivoEscritaInferior, FILE **arquivoLimiteSuperior, MemoriaInterna *espacoMemoria, int esquerda, int direita, int *indiceEsquerdo, int *indiceDireito) {
    int limiteSuperior = direita;  // Limite superior da partição
    int posicaoEscritaSuperior = direita;  // Posição de escrita no arquivo de saída superior
    int limiteInferior = esquerda;  // Limite inferior da partição
    int posicaoEscritaInferior = esquerda;  // Posição de escrita no arquivo de saída inferior
    int numeroRegistrosMemoria = 0;  // Número de registros na área de memória interna
    float limiteInferiorPivo = -99999999;  // Limite inferior do pivô
    float limiteSuperiorPivo = 99999999;   // Limite superior do pivô
    short lerDoSuperior = TRUE;    // Flag para indicar de onde ler (TRUE = superior, FALSE = inferior)
    Aluno ultimoLido, registro;

    // Posiciona os ponteiros dos arquivos no início das partes a serem lidas
    fseek(*arquivoLimiteInferior, (limiteInferior - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*arquivoEscritaInferior, (posicaoEscritaInferior - 1) * sizeof(Aluno), SEEK_SET);

    *indiceEsquerdo = esquerda - 1;  // Inicializa o índice esquerdo
    *indiceDireito = direita + 1;  // Inicializa o índice direito

    while (limiteSuperior >= limiteInferior) {
        // Se a área de memória interna ainda não está cheia, leia um registro
        if (numeroRegistrosMemoria < TAMMEMORIA - 1) {
            if (lerDoSuperior) {
                LeSup(arquivoLimiteSuperior, &ultimoLido, &limiteSuperior, &lerDoSuperior);  // Lê do arquivo superior
            } else {
                LeInf(arquivoLimiteInferior, &ultimoLido, &limiteInferior, &lerDoSuperior);  // Lê do arquivo inferior
            }
            InserirEsp(espacoMemoria, &ultimoLido, &numeroRegistrosMemoria); 
            continue;
        }

        // Decide de onde ler o próximo registro
        if (limiteSuperior == posicaoEscritaSuperior) {
            LeSup(arquivoLimiteSuperior, &ultimoLido, &limiteSuperior, &lerDoSuperior);  // Lê do arquivo superior
        } else if (limiteInferior == posicaoEscritaInferior) {
            LeInf(arquivoLimiteInferior, &ultimoLido, &limiteInferior, &lerDoSuperior);  // Lê do arquivo inferior
        } else if (lerDoSuperior) {
            LeSup(arquivoLimiteSuperior, &ultimoLido, &limiteSuperior, &lerDoSuperior);  // Lê do arquivo superior
        } else {
            LeInf(arquivoLimiteInferior, &ultimoLido, &limiteInferior, &lerDoSuperior);  // Lê do arquivo inferior
        }

        // Verifica se o registro lido deve ser escrito no arquivo de saída superior ou inferior
        if (ultimoLido.nota > limiteSuperiorPivo) {
            (*indiceDireito) = posicaoEscritaSuperior;
            EscreveMax(arquivoLimiteSuperior, ultimoLido, &posicaoEscritaSuperior);  // Escreve no arquivo superior
            continue;
        }

        if (ultimoLido.nota < limiteInferiorPivo) {
            (*indiceEsquerdo) = posicaoEscritaInferior;
            EscreveMin(arquivoEscritaInferior, ultimoLido, &posicaoEscritaInferior);  // Escreve no arquivo inferior
            continue;
        }

        // Insere o registro na área de memória interna
        InserirEsp(espacoMemoria, &ultimoLido, &numeroRegistrosMemoria);
        // Decide de onde retirar o próximo registro para escrita
        if (posicaoEscritaInferior - esquerda < direita - posicaoEscritaSuperior) {
            RetiraMin(espacoMemoria, &registro, &numeroRegistrosMemoria);  // Retira o menor registro da área
            EscreveMin(arquivoEscritaInferior, registro, &posicaoEscritaInferior);  // Escreve no arquivo inferior
            limiteInferiorPivo = registro.nota;  // Atualiza o limite inferior do pivô
        } else {
            RetiraMax(espacoMemoria, &registro, &numeroRegistrosMemoria);  // Retira o maior registro da área
            EscreveMax(arquivoLimiteSuperior, registro, &posicaoEscritaSuperior);  // Escreve no arquivo superior
            limiteSuperiorPivo = registro.nota;  // Atualiza o limite superior do pivô
        }
    }
    
    // Escreve os registros restantes na área de memória no arquivo inferior
    while (posicaoEscritaInferior <= posicaoEscritaSuperior) {
        RetiraMin(espacoMemoria, &registro, &numeroRegistrosMemoria);
        EscreveMin(arquivoEscritaInferior, registro, &posicaoEscritaInferior);
    }
}

// Função principal do Quicksort Externo
void QuicksortExterno(FILE **arquivoLimiteInferior, FILE **arquivoEscritaInferior, FILE **arquivoLimiteSuperior, int esquerda, int direita) {
    int indiceEsquerdo, indiceDireito;
    MemoriaInterna espacoMemoria;
    espacoMemoria.tamanho = 0;  // Inicializa a área de memória interna

    // Se houver mais de um registro para ordenar
    if (direita - esquerda > 1) {
        
        Particao(arquivoLimiteInferior, arquivoEscritaInferior, arquivoLimiteSuperior, &espacoMemoria, esquerda, direita, &indiceEsquerdo, &indiceDireito);
        // Ordena recursivamente as partições inferior e superior
        if (esquerda < indiceDireito) QuicksortExterno(arquivoLimiteInferior, arquivoEscritaInferior, arquivoLimiteSuperior, esquerda, indiceDireito);  // Ordena a parte inferior
        printf("saiu");
        if (indiceEsquerdo < direita) QuicksortExterno(arquivoLimiteInferior, arquivoEscritaInferior, arquivoLimiteSuperior, indiceEsquerdo, direita);  // Ordena a parte superior
    }
}

// Funções auxiliares implementadas
void LeSup(FILE **arquivoLimiteSuperior, Aluno *ultimoLido, int *limiteSuperior, short *lerDoSuperior) {
    fseek(*arquivoLimiteSuperior, (*limiteSuperior - 1) * sizeof(Aluno), SEEK_SET);
    if (fread(ultimoLido, sizeof(Aluno), 1, *arquivoLimiteSuperior) == 1) {
        (*limiteSuperior)--;
    } else {
        *lerDoSuperior = FALSE;  // Se não conseguir ler, muda para ler do inferior
    }
}

void LeInf(FILE **arquivoLimiteInferior, Aluno *ultimoLido, int *limiteInferior, short *lerDoSuperior) {
    fseek(*arquivoLimiteInferior, (*limiteInferior - 1) * sizeof(Aluno), SEEK_SET);
    if (fread(ultimoLido, sizeof(Aluno), 1, *arquivoLimiteInferior) == 1) {
        (*limiteInferior)++;
    } else {
        *lerDoSuperior = TRUE;  // Se não conseguir ler, muda para ler do superior
    }
}

void EscreveMin(FILE **arquivoEscritaInferior, Aluno registro, int *posicaoEscritaInferior) {
    fseek(*arquivoEscritaInferior, (*posicaoEscritaInferior - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&registro, sizeof(Aluno), 1, *arquivoEscritaInferior);
    (*posicaoEscritaInferior)++;  // Avança a posição de escrita no arquivo inferior
}

void EscreveMax(FILE **arquivoLimiteSuperior, Aluno registro, int *posicaoEscritaSuperior) {
    fseek(*arquivoLimiteSuperior, (*posicaoEscritaSuperior - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&registro, sizeof(Aluno), 1, *arquivoLimiteSuperior);
    (*posicaoEscritaSuperior)--;  // Retrocede a posição de escrita no arquivo superior
}

void InserirEsp(MemoriaInterna *espacoMemoria, Aluno *registro, int *numeroRegistrosMemoria) {
    espacoMemoria->registros[(*numeroRegistrosMemoria)++] = *registro;  // Insere o registro na área e incrementa o contador
}

void RetiraMin(MemoriaInterna *espacoMemoria, Aluno *registro, int *numeroRegistrosMemoria) {
    int i, indiceMinimo = 0;
    // Encontra o índice do registro com a menor nota
    for (i = 1; i < *numeroRegistrosMemoria; i++) {
        if (espacoMemoria->registros[i].nota < espacoMemoria->registros[indiceMinimo].nota) {
            indiceMinimo = i;
        }
    }
    *registro = espacoMemoria->registros[indiceMinimo];  // Retira o registro com a menor nota
    // Remove o registro da área de memória
    for (i = indiceMinimo; i < (*numeroRegistrosMemoria - 1); i++) {
        espacoMemoria->registros[i] = espacoMemoria->registros[i + 1];
    }
    (*numeroRegistrosMemoria)--;  // Decrementa o número de registros na área
}

void RetiraMax(MemoriaInterna *espacoMemoria, Aluno *registro, int *numeroRegistrosMemoria) {
    int i, indiceMaximo = 0;
    // Encontra o índice do registro com a maior nota
    for (i = 1; i < *numeroRegistrosMemoria; i++) {
        if (espacoMemoria->registros[i].nota > espacoMemoria->registros[indiceMaximo].nota) {
            indiceMaximo = i;
        }
    }
    *registro = espacoMemoria->registros[indiceMaximo];  // Retira o registro com a maior nota
    // Remove o registro da área de memória
    for (i = indiceMaximo; i < (*numeroRegistrosMemoria - 1); i++) {
        espacoMemoria->registros[i] = espacoMemoria->registros[i + 1];
    }
    (*numeroRegistrosMemoria)--;  // Decrementa o número de registros na área
}