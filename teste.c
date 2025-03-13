#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define TAM_AREA 10
#define TRUE 1
#define FALSE 0

//estrutura do tipo de arquivo
typedef struct {
    char numIncricao[10]; 
    char nota[6]; 
    char estado[3];
    char cidade[51];
    char curso[31];
} TipoRegistro;

//estrutura do tipo area
typedef struct{
    TipoRegistro tamArea[TAM_AREA];
    int numCelOcupadas;
} TipoArea;

// inicializar area
TipoArea inicializaArea(){
    TipoArea area;
    area.numCelOcupadas = 0;
    return area;
}

//algoritmo insertion para ordenacao interna
void insertionsort(TipoRegistro area[TAM_AREA], int numRegistros) {
    for (int i = 1; i < numRegistros; i++) {
        TipoRegistro temp = area[i];
        int j = i - 1;
        
        while (j >= 0 && atof(area[j].nota) > atof(temp.nota)) { // ordena por nota
            area[j + 1] = area[j];
            j--;
        }
        area[j + 1] = temp;
    }
}

void InsereItem(TipoRegistro UltLido, TipoArea *Area) {
    Area->tamArea[Area->numCelOcupadas] = UltLido;
    Area->numCelOcupadas++;
    insertionsort(Area->tamArea, Area->numCelOcupadas);
}

int ObterNumCelOcupadas(TipoArea *area){
    return area->numCelOcupadas;
}

void RetiraUltimo(TipoArea *area, TipoRegistro * R){
    *R = area->tamArea[area->numCelOcupadas-1];
    area->numCelOcupadas--;
}

void RetiraPrimeiro(TipoArea *area, TipoRegistro *R) {
    *R = area->tamArea[0];

    //deslocar elementos em uma posicao
    for (int i = 1; i < area->numCelOcupadas; i++) {
        area->tamArea[i - 1] = area->tamArea[i];
    }
    area->numCelOcupadas--;
}

void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea){
    InsereItem(*UltLido, Area);
    *NRArea = ObterNumCelOcupadas(Area);
}

void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraUltimo(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraPrimeiro(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer) {
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = FALSE;
}

void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer) {
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    *OndeLer = TRUE;
}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei) {
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
}

void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;
    double Linf = INT_MIN, Lsup = INT_MAX;

    short OndeLer = TRUE;
    TipoRegistro UltLido, R;

    fseek(*ArqLi, (Li - 1)* sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1)* sizeof(TipoRegistro), SEEK_SET);

    *i = Esq - 1;
    *j = Dir + 1;

    //se dados nao cabem na memoria princial, ocorre a particao
    while (Ls >= Li) {
        if (NRArea < TAM_AREA - 1) {
            if (OndeLer) {
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            }
            InserirArea(&Area, &UltLido, &NRArea);
            continue;
        }

        //interromper a alternancia caso a leitura e escrita inf e sup 
        //estejam na mesma posicao
        if (Ls == Es) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        } else if (Li == Ei) {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        } else if (OndeLer) {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        } else {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }

        //verifica se item lido é menor ou maior que os limites
        if (atof(UltLido.nota) > Lsup) {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }
        if (atof(UltLido.nota) < Linf) {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        int contador_ordenacoes = 0;
        if(contador_ordenacoes % 10000 == 0){
            printf("Lendo registro: Insc: %s | Nota: %s\n\n", UltLido.numIncricao, UltLido.nota);
        }

        InserirArea(&Area, &UltLido, &NRArea);

        //remover maior ou menor registro para escrita
        if (Ei - Esq < Dir - Es) {
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = atof(R.nota);
        } else {
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = atof(R.nota);
        }
    }

    //caso nao haja mais itens para serem inserido na memoria interna
    while (Ei <= Es) {
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
}

void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) {
    int i, j;

    if (Dir - Esq < 1) 
        return;

    TipoArea Area = inicializaArea();

    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);

    fflush(*ArqLi);
    fflush(*ArqEi);
    fflush(*ArqLEs);

    if (i - Esq < Dir - j) {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}

int main() {
    FILE *arqTxt;
    FILE *arqBin;
    FILE *arqLi;
    FILE *arqEi;
    FILE *arqLEs;

    // Abrindo o arquivo de texto para leitura
    arqTxt = fopen("PROVAO.txt", "r");
    if (arqTxt == NULL) {
        printf("\nERRO: Falha ao abrir arquivo de texto\n");
        return 1;
    }

    // Criando um arquivo binário para armazenar os registros
    arqBin = fopen("PROVAO.bin", "w+b");
    if (arqBin == NULL) {
        printf("\nERRO: Falha ao criar arquivo binário\n");
        fclose(arqTxt);
        return 1;
    }

    // Lendo registros do arquivo de texto e escrevendo no arquivo binário
    TipoRegistro reg;
    while (fscanf(arqTxt, "%s %s %s %50[^\n] %30[^\n]", reg.numIncricao, reg.nota, reg.estado, reg.cidade, reg.curso) != EOF) {
        fwrite(&reg, sizeof(TipoRegistro), 1, arqBin);
    }
    fclose(arqTxt);
    fclose(arqBin);

    // Abrindo o arquivo binário para leitura e escrita
    arqLi = fopen("PROVAO.bin", "r+b");
    arqEi = fopen("PROVAO.bin", "r+b");
    arqLEs = fopen("PROVAO.bin", "r+b");

    if (arqLi == NULL || arqEi == NULL || arqLEs == NULL) {
        printf("\nERRO: Falha ao abrir arquivo binário\n");
        return 1;
    }

    // Chamando a função de ordenação
    QuickSortExterno(&arqLi, &arqEi, &arqLEs, 1, 471705);

    // Reabrindo o arquivo ordenado para exibição
    rewind(arqLi);
    arqTxt = fopen("PROVAO_ORDENADO.txt", "w");
    if (arqTxt == NULL) {
        printf("\nERRO: Falha ao criar arquivo de texto\n");
        fclose(arqLi);
        fclose(arqEi);
        fclose(arqLEs);
        return 1;
    }

    printf("Arquivo ordenado:\n");
    while (fread(&reg, sizeof(TipoRegistro), 1, arqLi)) {
        printf("Insc: %s | Nota: %s | Estado: %s | Cidade: %s | Curso: %s\n",
               reg.numIncricao, reg.nota, reg.estado, reg.cidade, reg.curso);
        fprintf(arqTxt, "%s %s %s %s %s\n",
                reg.numIncricao, reg.nota, reg.estado, reg.cidade, reg.curso);
    }

    fclose(arqLi);
    fclose(arqEi);
    fclose(arqLEs);
    fclose(arqTxt);
    return 0;
}