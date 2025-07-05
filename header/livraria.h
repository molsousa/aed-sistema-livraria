#ifndef _LIVRARIA_H_INCLUDED
#define _LIVRARIA_H_INCLUDED

#include <stdio.h>

// Estrutura para cabecalho
typedef struct{
    int pos_raiz;
    int pos_livre;
    int pos_topo;
    int total_livros;
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

// Funcao para menu do usuario
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void menu();

// Funcao para opcoes do usuario
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void opcoes();

// Funcao para imprimir registros livres
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void imprimir_registros_livres(FILE* f);

// Funcao para alertar erro
// Pre-condicao: erro encontrado
// Pos-condicao: fecha execucao
void erro();

#endif // _LIVRARIA_H_INCLUDED
