#ifndef STRUCTS_H
#define STRUCTS_H

#define TAMMEMORIA 10 // tam da memoria principal

#define FF 20 // qtd de fitas
#define F FF/2
#define F1 (FF-1)

enum Bool {
	FALSE,
	TRUE,
};

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

typedef struct {
    Aluno registros[TAMMEMORIA];  // Vetor de registros com o tamanho máximo da memória interna
    int numRegistros;  // Número atual de registros na área
} TipoEsp;

#endif