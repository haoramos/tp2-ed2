#ifndef QUICKEXT_H
#define QUICKEXT_H

#include "structs.h"

MemoriaInterna Inicializa(); // Inicializa a memória interna
void bubblesort(Aluno registros[TAMMEMORIA], int contReg); // Ordena os registros
void InsereItem(MemoriaInterna *Area, Aluno UltLido); // Insere um item na área
int ObterNumCelOcupadas(MemoriaInterna *area); // Retorna o número de células ocupadas
void InserirArea(MemoriaInterna *Area, Aluno *UltLido, int *NRArea); // Insere um item na área
void RetiraUltimo(MemoriaInterna *Area, Aluno *R); // Remove o último item da área
void RetiraPrimeiro(MemoriaInterna *Area, Aluno *R); // Remove o primeiro item da área
void RetiraMax(MemoriaInterna *Area, Aluno *R, int *NRArea); // Remove o item com maior nota
void RetiraMin(MemoriaInterna *Area, Aluno *R, int *NRArea); // Remove o item com menor nota
void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, Bool *OndeLer); // Lê do arquivo superior
void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, Bool *OndeLer); // Lê do arquivo inferior
void EscreveMax(FILE **ArqLEs, Aluno R, int *Es); // Escreve no arquivo superior
void EscreveMin(FILE **ArqEi, Aluno R, int *Ei); // Escreve no arquivo inferior
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, MemoriaInterna *Area, int Esq, int Dir, int *i, int *j); // Partição do QuickSort
void ImprimirIntervalo(FILE *arq, int inicio, int fim); // Imprime um intervalo de registros
void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir); // QuickSort externo

#endif