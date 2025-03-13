#include <stdio.h>
#include "heapsort.h" // Supondo que F está definido em heapsort.h

// Função para trocar dois registros no vetor
void trocaReg(Aluno v[], int a, int b) {
  Aluno x = v[a];
  v[a] = v[b];
  v[b] = x;
}

// Função para comparar dois elementos, tratando os marcados (nota >= 101) como sempre maiores
short ehMenorMarcados(Aluno v[], int a, int b) {
  // Se qualquer um for final de arquivo, retorna falso
  if (v[a].matricula == -1 || v[b].matricula == -1) return 0;
  // Se ambos estiverem marcados ou ambos não estiverem marcados, ordena pela nota crescente
  return v[a].nota > v[b].nota;
}

// Função para refazer o heap a partir de um nó específico
void heapRefazMarcados(Aluno v[], int i, int n) {
  int menor = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  if (esq < n && ehMenorMarcados(v, esq, menor))
      menor = esq;

  if (dir < n && ehMenorMarcados(v, dir, menor))
      menor = dir;

  if (menor != i) {
      trocaReg(v, i, menor);
      heapRefazMarcados(v, menor, n);
  }
}

// Função para construir o heap
void heapConstroiMarcados(Aluno v[], int n) {
  for (int i = (n / 2) -1; i >= 0; i--) {
      heapRefazMarcados(v, i, n - 1);
  }
}

// Função principal do HeapSort
void heapSortMarcados(Aluno v[], int n) {
  heapConstroiMarcados(v, n); // Constrói o heap inicial
  while(n > 1) {
    trocaReg(v, n - 1, 0); // Troca o menor com o último
    heapRefazMarcados(v, 0, n - 1); // Restaura o heap
    n--;
  }
}