#ifndef STRUCTS_H
#define STRUCTS_H

#define TAMMEMORIA 10// tam da memoria principal

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
	int celaux;
} MemoriaInterna;

//Variaveis para o relatório
// long long numLeituras = 0;
// long long numEscritas = 0;
// long long numComparacoes = 0;
// clock_t tempoInicio, tempoFim;



#endif