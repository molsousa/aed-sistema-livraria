#ifndef _LIVRO_H_INCLUDED
#define _LIVRO_H_INCLUDED
#include "livraria.h"

#include <stdio.h>
#define TITULO 151
#define AUTOR 201
#define EDITORA 51

// Estrutura para livro
typedef struct{
    int codigo;
    char* titulo;
    char* autor;
    char* editora;
    int edicao;
    int ano;
    int exemplares;
    int preco[2];
}Livro;

// Estrutura para no
typedef struct _no{
    Livro l;
    int esq;
    int dir;
}no;

// Funcao para escrever no
// Pre-condicao: nenhuma
// Pos-condicao: escreve novo no
void escrever_no(FILE* f, no* x, int pos);

// Funcao para ler no
// Pre-condicao: nenhuma
// Pos-condicao: retorna no lido
no* ler_no(FILE* f, int pos);

// Funcao para inserir no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: insere novo no na arvore
void inserir_no(FILE* f, Livro l);

// Funcao para remover no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: remove no encontrado na arvore
void remover_no(FILE* f, int codigo);

// Funcao para encontrar minimo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do menor valor
int minimo(FILE* f, int pos);

int remover(FILE* f, int pos, int codigo, cabecalho* cab);

// Funcao de busca de no
// Pre-condicao: nenhuma
// Pos-condicao: retorna 0 se nao encontrado, valor do no se encontrado
int busca(FILE* f, int codigo, int pos);

void cadastrar_livro(FILE* f);

void listar_livros(FILE* f);

void imprimir_codigo(FILE* f);

void alocar_string(Livro* livro);

#endif // _LIVRO_H_INCLUDED
