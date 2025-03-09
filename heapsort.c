#include <stdio.h>
#include "heapsort.h"

void trocaReg(registro v[], int a, int b){
  registro x = v[a];
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

int ehMenorMarcado(registro v[], short m[], int a, int b){
  if (m[a] != 0) return 0;
  if (m[b] != 0) return 1;
  return v[b].nota < v[a].nota;  
}

void heapRefazMarcados(registro v[], short m[], int i, int n){
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

void heapConstroiMarcados(registro v[], short m[], int n){
  for (int i = (n / 2) - 1; i >= 0; i--) {
    heapRefazMarcados(v, m, i, n);
}
}

void heapSortMarcados(registro v[], short m[], int n){
  heapConstroiMarcados(v, m, n);
  for (int i = n - 1; i > 0; i--) {
      registro temp = v[0];
      v[0] = v[i];
      v[i] = temp;
      heapRefazMarcados(v, m, 0, i);
  }
}