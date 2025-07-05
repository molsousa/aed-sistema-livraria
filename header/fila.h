#ifndef _FILA_H_INCLUDED
#define _FILA_H_INCLUDED

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
void enqueue(Fila f, int codigo);

// Funcao para defilerar elemento
// Pre-condicao: fila criada
// Pos-condicao: retorna elemento defilerado
int dequeue(Fila f);

#endif // _FILA_H_INCLUDED
