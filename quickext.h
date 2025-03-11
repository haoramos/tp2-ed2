#ifndef QUICKEXT_H
#define QUICKEXT_H

#include "structs.h"


// Funções auxiliares
void LeSup(FILE**, Aluno*, int*, Bool*);
void LeInf(FILE**, Aluno*, int*, Bool*);
void EscreveMin(FILE**, Aluno, int*);
void EscreveMax(FILE**, Aluno, int*);
void InsereItem(MemoriaInterna *Area, Aluno *NovoAluno);
void InserirArea(MemoriaInterna*, Aluno*, int*);
void RetiraMin(MemoriaInterna*, Aluno*, int*);
void RetiraMax(MemoriaInterna*, Aluno*, int*);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, MemoriaInterna Area, int Esq, int Dir, int *i, int *j);
void QuicksortExterno(FILE**, FILE**, FILE**, int, int);

#endif