#include <stdio.h>
#include <stdlib.h>
#include "../header/livro.h"
#include "../header/fila.h"

// Funcao auxiliar para imprimir em inOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: imprime na tela a arvore ordenada
static void imprimir_inOrder(FILE* f, int pos);

// Funcao para impressao
// Pre-condicao: nenhuma
// Pos-condicao: imprime especificas informacoes do livro
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
    if(!x)
        erro();

    fseek(f, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fread(x, sizeof(no), 1, f);

    return x;
}

// Funcao para inserir no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: insere novo no na arvore
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

    cab->total_livros++;
    escrever_no(f, &novo, pos);
    escrever_cabecalho(f, cab);
    free(cab);
}

// Funcao para remover no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void remover_no(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);
    int codigo;

    printf("Insira o codigo do livro que deseja remover: ");
    scanf("%d%*c", &codigo);

    cab->pos_raiz = remover(f, cab->pos_raiz, codigo, cab);

    cab->total_livros--;
    escrever_cabecalho(f, cab);
    free(cab);
}

// Funcao auxiliar para remover elemento
// Pre-condicao: nenhuma
// Pos-condicao: remove no da arvore e retorna novo valor
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

// Funcao para encontrar minimo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do menor valor
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

// Funcao de busca de no
// Pre-condicao: nenhuma
// Pos-condicao: retorna -1 se nao encontrado, e a posicao do no se encontrado
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

// Funcao para cadastrar livro
// Pre-condicao: nenhuma
// Pos-condicao: cadastra e insere novo livro
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

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime livro de acordo com codigo do livro
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

        printf("=================================\n");
        printf("|Codigo: %d\n", livro.codigo);
        printf("|Titulo: %s\n", livro.titulo);
        printf("|Autor: %s\n", livro.autor);
        printf("|Editora: %s\n", livro.editora);
        printf("|Edicao: %d\n", livro.edicao);
        printf("|Ano: %d\n", livro.ano);
        printf("|Exemplares: %d\n", livro.exemplares);
        printf("|Preco: R$ %d,%d\n", livro.preco[0], livro.preco[1]);
        printf("=================================\n");

        free(x);
    }
}

// Funcao para calcular total de livros
// Pre-condicao: nenhuma
// Pos-condicao: imprime total de livros cadastrados
void total_livros(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);

    printf("Total de livros cadastrados: %d\n", cab->total_livros);
    free(cab);
    system("pause");
    system("cls");
}

// Funcao para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: nenhuma
void listar_livros(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);
    imprimir_inOrder(f, cab->pos_raiz);
    free(cab);
}

// Funcao auxiliar para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: imprime na tela a arvore pre ordenada
static void imprimir_inOrder(FILE* f, int pos)
{
    if(pos == -1)
        return;

    no* x = ler_no(f, pos);

    imprimir_inOrder(f, x->esq);
    impressao(x);
    imprimir_inOrder(f, x->dir);

    free(x);
}

// Funcao para impressao
// Pre-condicao: nenhuma
// Pos-condicao: imprime especificas informacoes do livro
static void impressao(no* x)
{
    printf("=====================================\n");
    printf("|Codigo: %d\n", x->l.codigo);
    printf("|Titulo: %s\n", x->l.titulo);
    printf("|Autor: %s\n", x->l.autor);
    printf("|Exemplares: %d\n", x->l.exemplares);
    printf("_____________________________________\n");
}

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime por niveis
void imprimir_niveis(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);

    if(cab->pos_raiz == -1){
        free(cab);
        return;
    }

    Fila deq = criar_fila();
    no* x = ler_no(f, cab->pos_raiz);
    enqueue(deq, x);
    enqueue(deq, NULL);

    while(!fila_vazia(deq)){
        no* atual = dequeue(deq);

        if(atual == NULL){
            printf("\n");

            if(!fila_vazia(deq))
                enqueue(deq, NULL);
        }
        else{
            printf("%d ", atual->l.codigo);

            if(atual->esq != -1){
                int pos = atual->esq;
                no* aux = ler_no(f, pos);
                enqueue(deq, aux);
            }

            if(atual->dir != -1){
                int pos = atual->dir;
                no* aux = ler_no(f, pos);
                enqueue(deq, aux);
            }
        }
    }
    deq = liberar_fila(deq);
    free(cab);
}

// Funcao para alocar strings
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void alocar_string(Livro* livro)
{
    livro->titulo = malloc(151 * sizeof(char));
    livro->autor = malloc(201 * sizeof(char));
    livro->editora = malloc(51 * sizeof(char));

    if(!livro->titulo || !livro->autor || !livro->editora)
        erro();
}
