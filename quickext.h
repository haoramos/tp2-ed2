#ifndef QUICKEXT_H
#define QUICKEXT_H

#include "structs.h"

// Funções auxiliares
void LeSup(FILE**, Aluno*, int*, short*);
void LeInf(FILE**, Aluno*, int*, short*);
void EscreveMin(FILE**, Aluno, int*);
void EscreveMax(FILE**, Aluno, int*);
void InserirEsp(MemoriaInterna*, Aluno*, int*);
void RetiraMin(MemoriaInterna*, Aluno*, int*);
void RetiraMax(MemoriaInterna*, Aluno*, int*);
void Particao(FILE**, FILE**, FILE**, MemoriaInterna*, int, int, int*, int*);
void QuicksortExterno(FILE**, FILE**, FILE**, int, int);

#endif