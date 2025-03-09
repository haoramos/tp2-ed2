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

void heapRefazMarcados(registro v[], short m[], int i, int n){
  int menor = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  if (esq < n && v[esq].nota < v[menor].nota && m[esq] == 0)
      menor = esq;

  if (dir < n && v[dir].nota < v[menor].nota)
      menor = dir;

  if (menor != i) {
      trocaReg(v, i, menor);
      heaprefaz(v, menor, n);
  }  
}

void heapConstroiMarcados(registro v[], short m[], int n){
  
}

void heapSortMarcados(registro v[], short m[], int n){

}