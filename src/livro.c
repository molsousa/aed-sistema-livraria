#include <stdio.h>
#include <stdlib.h>
#include "../header/livro.h"

static void impressao(no* x);

// Funcao para escrever no
// Pre-condicao: nenhuma
// Pos-condicao: escreve novo no
void escrever_no(FILE* f, no* x, int pos)
{
    fseek(f, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fwrite(x, sizeof(no), 1, f);
}

// Funcao para ler no
// Pre-condicao: nenhuma
// Pos-condicao: retorna no lido
no* ler_no(FILE* f, int pos)
{
    no* x = (no*) malloc(sizeof(no));

    fseek(f, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fread(x, sizeof(no), 1, f);

    return x;
}

void inserir_no(FILE* f, Livro l)
{
    cabecalho* cab = ler_cabecalho(f);
    no novo;
    int pos;

    novo.l = l;
    novo.esq = novo.dir = -1;

    if(cab->pos_livre != -1){
        pos = cab->pos_livre;
        no* livre = ler_no(f, pos);
        cab->pos_livre = livre->esq;
        free(livre);
    }
    else
        pos = cab->pos_topo++;

    if(cab->pos_raiz == -1)
        cab->pos_raiz = pos;

    else{
        int pos_pai = cab->pos_raiz;
        no* pai = ler_no(f, pos_pai);

        while(1){
            if(novo.l.codigo < pai->l.codigo){
                if(pai->esq == -1){
                    pai->esq = pos;
                    escrever_no(f, pai, pos_pai);
                    free(pai);
                     break;
                }
                pos_pai = pai->esq;
            }
            else if(novo.l.codigo > pai->l.codigo){
                if(pai->dir == -1){
                    pai->dir = pos;
                    escrever_no(f, pai, pos_pai);
                    free(pai);
                    break;
                }
                pos_pai = pai->dir;
            }
            free(pai);
            pai = ler_no(f, pos_pai);
        }
    }

    escrever_no(f, &novo, pos);
    escrever_cabecalho(f, cab);
    free(cab);
}

void remover_no(FILE* f, int codigo)
{
    cabecalho* cab = ler_cabecalho(f);
    cab->pos_raiz = remover(f, cab->pos_raiz, codigo, cab);
    escrever_cabecalho(f, cab);
    free(cab);
}

int minimo(FILE* f, int pos)
{
    no* x = ler_no(f, pos);

    while(x->esq != -1){
        pos = x->esq;
        free(x);
        x = ler_no(f, pos);
    }
    free(x);
    return pos;
}

int remover(FILE* f, int pos, int codigo, cabecalho* cab)
{
    if(pos == -1)
        return -1;

    no* x = ler_no(f, pos);

    if(codigo < x->l.codigo)
        x->esq = remover(f, x->esq, codigo, cab);

    else if(codigo > x->l.codigo)
        x->dir = remover(f, x->dir, codigo, cab);

    else{
        int temp;

        if(x->esq == -1){
            temp = x->dir;
            x->dir = cab->pos_livre;
            cab->pos_livre = pos;
            escrever_no(f, x, pos);
            free(x);
            return temp;
        }
        else if(x->dir == -1){
            temp = x->esq;
            x->esq = cab->pos_livre;
            cab->pos_livre = pos;
            escrever_no(f, x, pos);
            free(x);
            return temp;
        }
        else{
            int menor_pos = minimo(f, x->dir);
            no* menor = ler_no(f, menor_pos);
            x->l = menor->l;
            x->dir = remover(f, x->dir, menor->l.codigo, cab);
            escrever_no(f, x, pos);
            free(menor);
        }
    }
    escrever_no(f, x, pos);
    free(x);

    return pos;
}

int busca(FILE* f, int codigo, int pos)
{
    if(pos == -1)
        return -1;

    no* x = ler_no(f, pos);
    if(x->l.codigo < codigo){
        pos = x->dir;
        free(x);
        return busca(f, codigo, pos);
    }

    else if(x->l.codigo > codigo){
        pos = x->esq;
        free(x);
        return busca(f, codigo, pos);
    }

    else{
        free(x);
        return pos;
    }
}

void imprimir_codigo(FILE* f)
{
    int codigo;
    cabecalho* cab = ler_cabecalho(f);

    printf("Digite o codigo do livro: ");
    scanf("%d%*c", &codigo);

    codigo = busca(f, codigo, cab->pos_raiz);

    if(codigo == -1)
        printf("Livro nao encontrado!!");

    else{
        no* x = ler_no(f,codigo);
        Livro livro = x->l;

        printf("Codigo: %d\n", livro.codigo);
        printf("Titulo: %s\n", livro.titulo);
        printf("Autor: %s\n", livro.autor);
        printf("Editora: %s\n", livro.editora);
        printf("Edicao: %d\n", livro.edicao);
        printf("Ano: %d\n", livro.ano);
        printf("Exemplares: %d\n", livro.exemplares);
        printf("Preco: R$ %d,%d\n", livro.preco[0], livro.preco[1]);

        free(x);
    }
}

void cadastrar_livro(FILE* f)
{
    Livro l;
    alocar_string(&l);
    char valor[10];

    printf("Digite o codigo: ");
    scanf("%d%*c", &l.codigo);

    printf("Digite o titulo: ");
    scanf("%[^\n]%*c", l.titulo);

    printf("Digite o autor: ");
    scanf("%[^\n]%*c", l.autor);

    printf("Digite a editora: ");
    scanf("%[^\n]%*c", l.editora);

    printf("Digite a edicao: ");
    scanf("%d", &l.edicao);

    printf("Digite o ano: ");
    scanf("%d", &l.ano);

    printf("Digite o numero de exemplares: ");
    scanf("%d%*c", &l.exemplares);

    printf("Digite o preco do livro: ");
    scanf("%[^\n]%*c", valor);

    sscanf(valor, "%d,%d", &l.preco[0], &l.preco[1]);

    inserir_no(f, l);
}

// Funcao auxiliar para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: imprime na tela a arvore pre ordenada
void imprimir_preOrder(FILE* f, int pos)
{
    if(pos == -1)
        return;

    no* x = ler_no(f, pos);

    imprimir_preOrder(f, x->esq);
    impressao(x);
    imprimir_preOrder(f, x->dir);

    free(x);
}

// Funcao para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: nenhuma
void listar_livros(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);
    imprimir_preOrder(f, cab->pos_raiz);
    free(cab);
}

static void impressao(no* x)
{
    printf("=====================================\n");
    printf("Codigo: %d\n", x->l.codigo);
    printf("Titulo: %s\n", x->l.titulo);
    printf("Autor: %s\n", x->l.autor);
    printf("Exemplares: %d\n", x->l.exemplares);
    printf("_____________________________________\n");
}

void alocar_string(Livro* livro)
{
    livro->titulo = malloc(151 * sizeof(char));
    livro->autor = malloc(201 * sizeof(char));
    livro->editora = malloc(51 * sizeof(char));
}
