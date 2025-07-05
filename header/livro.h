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
typedef struct{
    Livro livro;
    int esq;
    int dir;
}no;

// Funcao para escrever no
// Pre-condicao: nenhuma
// Pos-condicao: escreve novo no
void escrever_no(FILE* arq_bin, no* x, int pos);

// Funcao para ler no
// Pre-condicao: nenhuma
// Pos-condicao: retorna no lido
no* ler_no(FILE* arq_bin, int pos);

// Funcao para inserir no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: insere novo no na arvore
void inserir_no(FILE* arq_bin, Livro livro);

// Funcao para remover no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void remover_no(FILE* arq_bin);

// Funcao auxiliar para remover elemento
// Pre-condicao: nenhuma
// Pos-condicao: remove no da arvore e retorna novo valor
int remover(FILE* arq_bin, int pos, int codigo, cabecalho* cab);

// Funcao para encontrar minimo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do menor valor
int minimo(FILE* arq_bin, int pos);

// Funcao para encontrar maximo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do maior valor
int maximo(FILE* arq_bin, int pos);

// Funcao de busca de no
// Pre-condicao: nenhuma
// Pos-condicao: retorna -1 se nao encontrado, e a posicao do no se encontrado
int busca(FILE* arq_bin, int codigo, int pos);

// Funcao para cadastrar livro
// Pre-condicao: nenhuma
// Pos-condicao: cadastra e insere novo livro
void cadastrar_livro(FILE* arq_bin);

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime livro de acordo com codigo do livro
void imprimir_codigo(FILE* arq_bin);

// Funcao para calcular total de livros
// Pre-condicao: nenhuma
// Pos-condicao: imprime total de livros cadastrados
void total_livros(FILE* arq_bin);

// Funcao para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: nenhuma
void listar_livros(FILE* arq_bin);

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime por niveis
void imprimir_niveis(FILE* arq_bin);

// Funcao para alocar strings
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void alocar_string(Livro* livro);

#endif // _LIVRO_H_INCLUDED
