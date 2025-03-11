#ifndef STRUCTS_H
#define STRUCTS_H

#define TAMMEMORIA 10 // tam da memoria principal

#define FF 20 // qtd de fitas
#define F FF/2
#define F1 (FF-1)

// Definição do tipo enum Bool
typedef enum { FALSE, TRUE } Bool;

typedef struct  {
	long matricula;
	float nota;
	char estado[3];
	char cidade[51];
	char curso[31];
} Aluno;

typedef struct {
    Aluno registros[TAMMEMORIA];
    int tamanho;
} MemoriaInterna;

typedef struct {
    FILE *fita;
    int numRegistros;
} Fita;

#endif