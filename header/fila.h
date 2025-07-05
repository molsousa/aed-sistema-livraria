#ifndef _FILA_H_INCLUDED
#define _FILA_H_INCLUDED
#include "livro.h"

// Definicao de tipo para fila
typedef struct fila* Fila;

// Funcao para criar fila
// Pre-condicao: nenhuma
// Pos-condicao: cria nova fila
Fila criar_fila();

// Funcao para verificar se uma fila eh vazia
// Pre-condicao: fila criada
// Pos-condicao: retorna 1 se vazia
int fila_vazia(Fila f);

// Funcao para liberar fila
// Pre-condicao: nenhuma
// Pos-condicao: retorna nulo apos liberar elementos
void* liberar_fila(Fila f);

// Funcao para enfilerar elemento
// Pre-condicao: fila criada
// Pos-condicao: nenhuma
void enqueue(Fila f, no* x);

// Funcao para defilerar elemento
// Pre-condicao: fila criada
// Pos-condicao: retorna elemento defilerado
no* dequeue(Fila f);

#endif // _FILA_H_INCLUDED
