#include <stdio.h>
#include "heapsort.h"

void trocaReg(Aluno v[], int a, int b){
  Aluno x = v[a];
  v[a] = v[b];
  v[b] = x;
}

void trocaInt(int v[], int a, int b){
  int x = v[a];
  v[a] = v[b];
  v[b] = x;
}

void trocaShort(short v[], int a, int b){
  short x = v[a];
  v[a] = v[b];
  v[b] = x;
}

void heapRefazMarcados(Aluno v[], short m[], int i, int n){
  int menor = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  if (esq < n && ehMenorMarcado(v, m, esq, dir))
    menor = esq;

  if (dir < n && ehMenorMarcado(v, m, dir, esq))
    menor = dir;

  if (menor != i) {
    trocaReg(v, i, menor);
    heapRefazMarcados(v, m, menor, n);
  }  
}

void heapConstroiMarcados(Aluno v[], short m[], int n){
  for (int i = (n / 2) - 1; i >= 0; i--) {
    heapRefazMarcados(v, m, i, n);
}
}

void heapSortMarcados(Aluno v[], short m[], int n){
  heapConstroiMarcados(v, m, n);
  for (int i = n - 1; i > 0; i--) {
      Aluno temp = v[0];
      v[0] = v[i];
      v[i] = temp;
      heapRefazMarcados(v, m, 0, i);
  }
}