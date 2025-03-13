#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "quickext.h"

#define TAMAREA TAMMEMORIA

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


MemoriaInterna Inicializa(){
    MemoriaInterna Area;
    Area.celaux = 0;
    return Area;
}

void bubblesort(Aluno registros[TAMMEMORIA], int contReg) {
    for (int i = 0; i < contReg - 1; i++) {
        for (int j = 0; j < contReg - i - 1; j++) {
            if (registros[j].nota > registros[j + 1].nota) { // ordena por nota
                //Troca registro
                Aluno temp = registros[j];
                registros[j] = registros[j + 1];
                registros[j + 1] = temp;
            }
        }
    }
}

void InsereItem(MemoriaInterna *Area, Aluno UltLido) {
    Area->registros[Area->celaux] = UltLido;
    Area->celaux++;
    bubblesort(Area->registros, Area->celaux); // Ordena após a inserção
}

int ObterNumCelOcupadas(MemoriaInterna *area) {
    return area->celaux; // Retorna o número de células ocupadas
}

void InserirArea(MemoriaInterna *Area, Aluno *UltLido, int *NRArea) {
    InsereItem(Area, *UltLido); // Passa o valor de UltLido
    *NRArea = ObterNumCelOcupadas(Area);
}


void RetiraUltimo(MemoriaInterna *Area, Aluno *R) {
    *R = Area->registros[Area->celaux - 1];
    Area->celaux--;
}

void RetiraPrimeiro(MemoriaInterna *Area, Aluno *R) {
    *R = Area->registros[0];
    //deslocar elementos em uma posicao
    for (int i = 1; i < Area->celaux; i++) {
        Area->registros[i - 1] = Area->registros[i];
    }
    Area->celaux--;
}

void RetiraMax(MemoriaInterna *Area, Aluno *R, int *NRArea){
    RetiraUltimo(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void RetiraMin(MemoriaInterna *Area, Aluno *R, int *NRArea){
    RetiraPrimeiro(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
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


void EscreveMax(FILE **ArqLEs, Aluno R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, Aluno R, int *Ei) {
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    (*Ei)++;
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, MemoriaInterna *Area, int Esq, int Dir, int *i, int *j)  {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;;
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
            InserirArea(Area, &UltLido, &NRArea); // Insere de forma ordenada
            continue;
        } else if (Ls == Es) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        } else if (Li == Ei) {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        } else if (OndeLer) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        } else {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }
        
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

        InserirArea(Area, &UltLido, &NRArea);

        if (Ei - Esq < Dir - Es) {
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        } else {
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }
    }

    // Escreve os dados restantes na memória interna
    while (Ei <= Es) {
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
}

void ImprimirIntervalo(FILE *arq, int inicio, int fim) {
    Aluno aluno;
    fseek(arq, 0, SEEK_SET);
    for (int i = 0; i <= fim; i++) {
        fread(&aluno, sizeof(Aluno), 1, arq);
        if (i >= inicio && i <= fim) {
            printf("Matrícula: %ld | Nota: %.1f\n", aluno.matricula, aluno.nota);
        }
    }
}


void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) {
    int i, j;
    // Verifica se o intervalo é válido
    if (Dir - Esq < 1) 
        return;
    // Inicializa a memória interna
    MemoriaInterna Area = Inicializa();

    Particao(ArqLi, ArqEi, ArqLEs, &Area, Esq, Dir, &i, &j);

    fflush(*ArqLi);
    fflush(*ArqEi);
    fflush(*ArqLEs);

    // printf("\nIntervalo Esq até i:\n");
    // ImprimirIntervalo(*ArqLi, Esq, i);

    //  printf("\nIntervalo de i até j:\n");
    //  ImprimirIntervalo(*ArqLi, i+1, j-1);
    //  printf("\nIntervalo dir até j:\n");
    //  ImprimirIntervalo(*ArqLi, j, Dir);

     if (i - Esq < Dir - j) {
         QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
         QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
     } else {
         QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
         QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
     }
}



