#ifndef _LIVRARIA_H_INCLUDED
#define _LIVRARIA_H_INCLUDED

#include <stdio.h>

// Estrutura para cabecalho
typedef struct{
    int pos_raiz;
    int pos_livre;
    int pos_topo;
}cabecalho;

// Funcao para inicializar arvore binaria
// Pre-condicao: arquivo inicializado
// Pos-condicao: inicializa o cabecalho
void criar_arvore(FILE* f);

// Funcao para escrever cabecalho
// Pre-condicao: arvore criada
// Pos-condicao: escreve alteracoes no cabecalho
void escrever_cabecalho(FILE* f, cabecalho* cab);

// Funcao para ler o cabecalho
// Pre-condicao: arvore criada
// Pos-condicao: retorna a condicao do cabecalho
cabecalho* ler_cabecalho(FILE* f);

#endif // _LIVRARIA_H_INCLUDED
