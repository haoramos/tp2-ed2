#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

// Funções auxiliares
void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, short *OndeLer);
void EscreveMin(FILE **ArqEi, Aluno R, int *Ei);
void EscreveMax(FILE **ArqLEs, Aluno R, int *Es);
void InserirEsp(TipoEsp *Esp, Aluno *Reg, int *NREsp);
void RetiraMin(TipoEsp *Esp, Aluno *R, int *NREsp);
void RetiraMax(TipoEsp *Esp, Aluno *R, int *NREsp);

// Função de Partição (Quicksort Externo)
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoEsp *Esp, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir;  // Limite superior da partição
    int Es = Dir;  // Posição de escrita no arquivo de saída superior
    int Li = Esq;  // Limite inferior da partição
    int Ei = Esq;  // Posição de escrita no arquivo de saída inferior
    int NREsp = 0;  // Número de registros na área de memória interna
    float Linf = -99999999;  // Limite inferior do pivô
    float Lsup = 99999999;   // Limite superior do pivô
    short OndeLer = TRUE;    // Flag para indicar de onde ler (TRUE = superior, FALSE = inferior)
    Aluno UltLido, R;

    // Posiciona os ponteiros dos arquivos no início das partes a serem lidas
    fseek(*ArqLi, (Li - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(Aluno), SEEK_SET);

    *i = Esq - 1;  // Inicializa o índice esquerdo
    *j = Dir + 1;  // Inicializa o índice direito

    while (Ls >= Li) {
        // Se a área de memória interna ainda não está cheia, leia um registro
        if (NREsp < TAMMEMORIA - 1) {
            if (OndeLer) {
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);  // Lê do arquivo superior
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);  // Lê do arquivo inferior
            }
            InserirEsp(Esp, &UltLido, &NREsp);  // Insere o registro na área de memória
            continue;
        }

        // Decide de onde ler o próximo registro
        if (Ls == Es) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);  // Lê do arquivo superior
        } else if (Li == Ei) {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);  // Lê do arquivo inferior
        } else if (OndeLer) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);  // Lê do arquivo superior
        } else {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);  // Lê do arquivo inferior
        }

        // Verifica se o registro lido deve ser escrito no arquivo de saída superior ou inferior
        if (UltLido.nota > Lsup) {
            (*j) = Es;
            EscreveMax(ArqLEs, UltLido, &Es);  // Escreve no arquivo superior
            continue;
        }

        if (UltLido.nota < Linf) {
            (*i) = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);  // Escreve no arquivo inferior
            continue;
        }

        // Insere o registro na área de memória interna
        InserirEsp(Esp, &UltLido, &NREsp);

        // Decide de onde retirar o próximo registro para escrita
        if (Ei - Esq < Dir - Es) {
            RetiraMin(Esp, &R, &NREsp);  // Retira o menor registro da área
            EscreveMin(ArqEi, R, &Ei);  // Escreve no arquivo inferior
            Linf = R.nota;               // Atualiza o limite inferior
        } else {
            RetiraMax(Esp, &R, &NREsp);  // Retira o maior registro da área
            EscreveMax(ArqLEs, R, &Es);  // Escreve no arquivo superior
            Lsup = R.nota;               // Atualiza o limite superior
        }
    }

    // Escreve os registros restantes na área de memória no arquivo inferior
    while (Ei <= Es) {
        RetiraMin(Esp, &R, &NREsp);
        EscreveMin(ArqEi, R, &Ei);
    }
}

// Função principal do Quicksort Externo
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) {
    int i, j;
    TipoEsp Esp;
    Esp.numRegistros = 0;  // Inicializa a área de memória interna

    if (Dir - Esq > 1) {  // Se houver mais de um registro para ordenar
        Particao(ArqLi, ArqEi, ArqLEs, &Esp, Esq, Dir, &i, &j);

        if (Esq < j) QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, j);  // Ordena a parte inferior
        if (i < Dir) QuicksortExterno(ArqLi, ArqEi, ArqLEs, i, Dir);  // Ordena a parte superior
    }
}

// Funções auxiliares implementadas
void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, short *OndeLer) {
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    if (fread(UltLido, sizeof(Aluno), 1, *ArqLEs) == 1) {
        (*Ls)--;
    } else {
        *OndeLer = FALSE;
    }
}

void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, short *OndeLer) {
    fseek(*ArqLi, (*Li - 1) * sizeof(Aluno), SEEK_SET);
    if (fread(UltLido, sizeof(Aluno), 1, *ArqLi) == 1) {
        (*Li)++;
    } else {
        *OndeLer = TRUE;
    }
}

void EscreveMin(FILE **ArqEi, Aluno R, int *Ei) {
    fseek(*ArqEi, (*Ei - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    (*Ei)++;
}

void EscreveMax(FILE **ArqLEs, Aluno R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    (*Es)--;
}

void InserirEsp(TipoEsp *Esp, Aluno *Reg, int *NREsp) {
    Esp->registros[(*NREsp)++] = *Reg;  // Insere o registro na área e incrementa o contador
}

void RetiraMin(TipoEsp *Esp, Aluno *R, int *NREsp) {
    int i, minIndex = 0;
    for (i = 1; i < *NREsp; i++) {
        if (Esp->registros[i].nota < Esp->registros[minIndex].nota) {
            minIndex = i;
        }
    }
    *R = Esp->registros[minIndex];
    for (i = minIndex; i < (*NREsp - 1); i++) {
        Esp->registros[i] = Esp->registros[i + 1];
    }
    (*NREsp)--;
}

void RetiraMax(TipoEsp *Esp, Aluno *R, int *NREsp) {
    int i, maxIndex = 0;
    for (i = 1; i < *NREsp; i++) {
        if (Esp->registros[i].nota > Esp->registros[maxIndex].nota) {
            maxIndex = i;
        }
    }
    *R = Esp->registros[maxIndex];
    for (i = maxIndex; i < (*NREsp - 1); i++) {
        Esp->registros[i] = Esp->registros[i + 1];
    }
    (*NREsp)--;
}