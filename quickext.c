#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "quickext.h"

#define TAMAREA TAMMEMORIA

void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) {
    int i, j;
    MemoriaInterna Area;
    Area.tamanho = 0;

    if (Dir - Esq < 1) return;

    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);

    if (i - Esq < Dir - j) {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, MemoriaInterna Area, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;
    float Linf = -999999999.0, Lsup = 999999999.0;
    Bool OndeLer = TRUE;
    Aluno UltLido, R;

    fseek(*ArqLi, (Li - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(Aluno), SEEK_SET);

    *i = Esq - 1; *j = Dir + 1;

    while (Ls >= Li) {
        if (NRArea < TAMMEMORIA - 1) {
            if (OndeLer) {
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            }
            InserirArea(&Area, &UltLido, &NRArea); // Insere de forma ordenada
            continue;
        }

        if (Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei)
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        else if (OndeLer)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);

        if (UltLido.nota > Lsup) {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if (UltLido.nota < Linf) {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        InserirArea(&Area, &UltLido, &NRArea);

        if (Ei - Esq < Dir - Es) {
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        } else {
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }
    }

    while (NRArea > 0) {
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
}



void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, Bool *OndeLer) {
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(UltLido, sizeof(Aluno), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = FALSE;
}

void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, Bool *OndeLer) {
    fread(UltLido, sizeof(Aluno), 1, *ArqLi);
    (*Li)++;
    *OndeLer = TRUE;
}
void InsereItem(MemoriaInterna *Area, Aluno *NovoAluno) {
    // Adiciona o novo aluno ao final da área
    Area->registros[Area->tamanho] = *NovoAluno;
    Area->tamanho++;

    // Ordena a área usando Bubble Sort
    for (int i = Area->tamanho - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (Area->registros[j].nota > Area->registros[j + 1].nota) {
                // Troca os alunos
                Aluno temp = Area->registros[j];
                Area->registros[j] = Area->registros[j + 1];
                Area->registros[j + 1] = temp;
            }
        }
    }
}
void InserirArea(MemoriaInterna *Area, Aluno *UltLido, int *NRArea) {
    InsereItem(Area, UltLido); // Insere o novo aluno de forma ordenada
    (*NRArea)++; // Incrementa o número de registros na área
}

void EscreveMax(FILE **ArqLEs, Aluno R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, Aluno R, int *Ei) {
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    (*Ei)++;
}

void RetiraMax(MemoriaInterna *Area, Aluno *R, int *NRArea) {
    *R = Area->registros[Area->tamanho - 1];
    Area->tamanho--;
    (*NRArea)--;
}

void RetiraMin(MemoriaInterna *Area, Aluno *R, int *NRArea) {
    *R = Area->registros[0];
    for (int i = 1; i < Area->tamanho; i++) {
        Area->registros[i - 1] = Area->registros[i];
    }
    Area->tamanho--;
    (*NRArea)--;
}