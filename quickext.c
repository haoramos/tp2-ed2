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

    Particao(ArqLi, ArqEi, ArqLEs, &Area, Esq, Dir, &i, &j);

    if (i - Esq < Dir - j) {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, MemoriaInterna *Area, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;
    int Linf = INT_MIN, Lsup = INT_MAX;
    Bool OndeLer = TRUE;
    Aluno UltLido, R;

    printf("=== Particao ===\n");
    printf("Esq: %d, Dir: %d\n", Esq, Dir);

    while (Ls >= Li) {
        if (NRArea < TAMAREA - 1) {
            if (OndeLer) {
                printf("Lendo superior: Ls = %d\n", Ls);
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                printf("Lendo inferior: Li = %d\n", Li);
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            }
            printf("Inserindo na área: Matrícula = %d | Nota %.1f\n", UltLido.matricula, UltLido.nota);
            InserirArea(Area, &UltLido, &NRArea);
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

        if (UltLido.matricula > Lsup) {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if (UltLido.matricula < Linf) {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        InserirArea(Area, &UltLido, &NRArea);

        if (Ei - Esq < Dir - Es) {
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.matricula;
        } else {
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.matricula;
        }
    }

    while (Ei <= Es) {
        RetiraMin(Area, &R, &NRArea);
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

void InserirArea(MemoriaInterna *Area, Aluno *UltLido, int *NRArea) {
    int i = Area->tamanho - 1;
    while (i >= 0 && Area->registros[i].matricula > UltLido->matricula) {
        Area->registros[i + 1] = Area->registros[i];
        i--;
    }
    Area->registros[i + 1] = *UltLido;
    Area->tamanho++;
    (*NRArea)++;
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