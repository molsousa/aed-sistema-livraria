#include <stdio.h>
#include <stdlib.h>
#include "../header/livro.h"
#include "../header/fila.h"

// Funcao auxiliar para imprimir em inOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: imprime na tela a arvore ordenada
static void imprimir_inOrder(FILE* arq_bin, int pos);

// Funcao para impressao
// Pre-condicao: nenhuma
// Pos-condicao: imprime especificas informacoes do livro
static void impressao(no* x);

// Funcao para escrever no
// Pre-condicao: nenhuma
// Pos-condicao: escreve novo no
void escrever_no(FILE* arq_bin, no* x, int pos)
{
    fseek(arq_bin, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fwrite(x, sizeof(no), 1, arq_bin);
}

// Funcao para ler no
// Pre-condicao: nenhuma
// Pos-condicao: retorna no lido
no* ler_no(FILE* arq_bin, int pos)
{
    no* x = (no*) malloc(sizeof(no));
    if(!x)
        erro();

    fseek(arq_bin, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fread(x, sizeof(no), 1, arq_bin);

    return x;
}

// Funcao para inserir no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: insere novo no na arvore
void inserir_no(FILE* arq_bin, Livro livro)
{
    cabecalho* cab = ler_cabecalho(arq_bin);
    no novo;
    int pos;

    novo.livro = livro;
    novo.esq = novo.dir = -1;

    if(cab->pos_livre != -1){ // reaproveita no livre
        pos = cab->pos_livre;
        no* livre = ler_no(arq_bin, pos);
        cab->pos_livre = livre->esq;
        free(livre);
    }
    else
        pos = cab->pos_topo++;

    if(cab->pos_raiz == -1)
        cab->pos_raiz = pos;

    else{
        int pos_pai = cab->pos_raiz;
        no* pai = ler_no(arq_bin, pos_pai);

        while(1){ // evita recursao
            if(novo.livro.codigo < pai->livro.codigo){
                if(pai->esq == -1){
                    pai->esq = pos;
                    escrever_no(arq_bin, pai, pos_pai);
                    free(pai);
                     break;
                }
                pos_pai = pai->esq;
            }
            else if(novo.livro.codigo > pai->livro.codigo){
                if(pai->dir == -1){
                    pai->dir = pos;
                    escrever_no(arq_bin, pai, pos_pai);
                    free(pai);
                    break;
                }
                pos_pai = pai->dir;
            }
            free(pai);
            pai = ler_no(arq_bin, pos_pai);
        }
    }

    cab->total_livros++;
    escrever_no(arq_bin, &novo, pos);
    escrever_cabecalho(arq_bin, cab);
    free(cab);
}

// Funcao para remover no na arvore
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void remover_no(FILE* arq_bin)
{
    cabecalho* cab = ler_cabecalho(arq_bin);
    int codigo;

    printf("Insira o codigo do livro que deseja remover: ");
    scanf("%d%*c", &codigo);

    if((busca(arq_bin, codigo, cab->pos_raiz)) == -1)
        printf("Livro nao existe\n");

    cab->pos_raiz = remover(arq_bin, cab->pos_raiz, codigo, cab);

    cab->total_livros--;
    escrever_cabecalho(arq_bin, cab);
    free(cab);
}

// Funcao auxiliar para remover elemento
// Pre-condicao: nenhuma
// Pos-condicao: remove no da arvore e retorna novo valor
int remover(FILE* arq_bin, int pos, int codigo, cabecalho* cab)
{
    if(pos == -1)
        return -1;

    no* x = ler_no(arq_bin, pos);

    if(codigo < x->livro.codigo){
        x->esq = remover(arq_bin, x->esq, codigo, cab);
        escrever_no(arq_bin, x, pos);
    }

    else if(codigo > x->livro.codigo){
        x->dir = remover(arq_bin, x->dir, codigo, cab);
        escrever_no(arq_bin, x, pos);
    }

    else{
        if(x->esq == -1 && x->dir == -1){ // no folha (caso base)
            if(pos == cab->pos_raiz)
                cab->pos_raiz = -1;

            x->esq = x->dir = cab->pos_livre;
            cab->pos_livre = pos;
            escrever_cabecalho(arq_bin, cab);
            escrever_no(arq_bin, x, pos);
            free(x);
            return -1;
        }
        else{
            if(x->esq == -1){ // no da esquerda eh nulo
                int pos_min = minimo(arq_bin, x->dir);
                no* min = ler_no(arq_bin, pos_min);
                x->livro = min->livro;
                x->dir = remover(arq_bin, x->dir, min->livro.codigo, cab);
                free(min);
            }
            else{ // no da direita eh nulo ou dois filhos
                int pos_max = maximo(arq_bin, x->esq);
                no* max = ler_no(arq_bin, pos_max);
                x->livro = max->livro;
                x->esq = remover(arq_bin, x->esq, max->livro.codigo, cab);
                free(max);
            }
        }
    }
    escrever_no(arq_bin, x, pos);
    free(x);

    return pos;
}

// Funcao para encontrar minimo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do menor valor
int minimo(FILE* arq_bin, int pos)
{
    no* x = ler_no(arq_bin, pos);

    while(x->esq != -1){
        pos = x->esq;
        free(x);
        x = ler_no(arq_bin, pos);
    }
    free(x);
    return pos;
}

// Funcao para encontrar maximo
// Pre-condicao: nenhuma
// Pos-condicao: retorna a posicao do maior valor
int maximo(FILE* arq_bin, int pos)
{
    no* x = ler_no(arq_bin, pos);

    while(x->dir != -1){
        pos = x->dir;
        free(x);
        x = ler_no(arq_bin, pos);
    }
    free(x);
    return pos;
}

// Funcao de busca de no
// Pre-condicao: nenhuma
// Pos-condicao: retorna -1 se nao encontrado, e a posicao do no se encontrado
int busca(FILE* arq_bin, int codigo, int pos)
{
    if(pos == -1)
        return -1;

    no* x = ler_no(arq_bin, pos);
    if(x->livro.codigo < codigo){
        pos = x->dir;
        free(x);
        return busca(arq_bin, codigo, pos);
    }

    else if(x->livro.codigo > codigo){
        pos = x->esq;
        free(x);
        return busca(arq_bin, codigo, pos);
    }

    else{
        free(x);
        return pos;
    }
}

// Funcao para cadastrar livro
// Pre-condicao: nenhuma
// Pos-condicao: cadastra e insere novo livro
void cadastrar_livro(FILE* arq_bin)
{
    Livro livro;
    alocar_string(&livro);
    char valor[10];

    printf("Digite o codigo: ");
    scanf("%d%*c", &livro.codigo);

    printf("Digite o titulo: ");
    scanf("%[^\n]%*c", livro.titulo);

    printf("Digite o autor: ");
    scanf("%[^\n]%*c", livro.autor);

    printf("Digite a editora: ");
    scanf("%[^\n]%*c", livro.editora);

    printf("Digite a edicao: ");
    scanf("%d", &livro.edicao);

    printf("Digite o ano: ");
    scanf("%d", &livro.ano);

    printf("Digite o numero de exemplares: ");
    scanf("%d%*c", &livro.exemplares);

    printf("Digite o preco do livro: ");
    scanf("%[^\n]%*c", valor);
    sscanf(valor, "%d,%d", &livro.preco[0], &livro.preco[1]);

    inserir_no(arq_bin, livro);

    system("cls");
}

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime livro de acordo com codigo do livro
void imprimir_codigo(FILE* arq_bin)
{
    int codigo;
    cabecalho* cab = ler_cabecalho(arq_bin);

    printf("Digite o codigo do livro: ");
    scanf("%d%*c", &codigo);

    codigo = busca(arq_bin, codigo, cab->pos_raiz);

    if(codigo == -1)
        printf("Livro nao encontrado!!");

    else{
        no* x = ler_no(arq_bin, codigo);
        Livro livro = x->livro;

        printf("|______________LIVRO______________|\n");
        printf("|=================================|\n");
        printf("|Codigo: %d\n", livro.codigo);
        printf("|Titulo: %s\n", livro.titulo);
        printf("|Autor: %s\n", livro.autor);
        printf("|Editora: %s\n", livro.editora);
        printf("|Edicao: %d\n", livro.edicao);
        printf("|Ano: %d\n", livro.ano);
        printf("|Exemplares: %d\n", livro.exemplares);
        printf("|Preco: R$ %d,%02d\n", livro.preco[0], livro.preco[1]);
        printf("|=================================|\n");

        free(x);
    }
    system("pause");
    system("cls");
}

// Funcao para calcular total de livros
// Pre-condicao: nenhuma
// Pos-condicao: imprime total de livros cadastrados
void total_livros(FILE* arq_bin)
{
    cabecalho* cab = ler_cabecalho(arq_bin);

    printf("Total de livros cadastrados: %d\n", cab->total_livros);
    free(cab);
    system("pause");
    system("cls");
}

// Funcao para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: nenhuma
void listar_livros(FILE* arq_bin)
{
    cabecalho* cab = ler_cabecalho(arq_bin);
    if(cab->pos_raiz == -1){
        printf("Sem livros adicionados!!\n");
        return;
    }

    printf("|__________LISTA_DE_LIVROS__________|\n");
    imprimir_inOrder(arq_bin, cab->pos_raiz);
    free(cab);
    system("pause");
    system("cls");
}

// Funcao auxiliar para imprimir em preOrder
// Pre-condicao: arquivo inicializado
// Pos-condicao: imprime na tela a arvore pre ordenada
static void imprimir_inOrder(FILE* arq_bin, int pos)
{
    if(pos == -1)
        return;

    no* x = ler_no(arq_bin, pos);

    imprimir_inOrder(arq_bin, x->esq);
    impressao(x);
    imprimir_inOrder(arq_bin, x->dir);

    free(x);
}

// Funcao para impressao
// Pre-condicao: nenhuma
// Pos-condicao: imprime especificas informacoes do livro
static void impressao(no* x)
{
    printf("|===================================|\n");
    printf("|Codigo: %d\n", x->livro.codigo);
    printf("|Titulo: %s\n", x->livro.titulo);
    printf("|Autor: %s\n", x->livro.autor);
    printf("|Exemplares: %d\n", x->livro.exemplares);
    printf("|===================================|\n");
}

// Funcao para imprimir
// Pre-condicao: nenhuma
// Pos-condicao: imprime por niveis
void imprimir_niveis(FILE* arq_bin)
{
    cabecalho* cab = ler_cabecalho(arq_bin);

    if(cab->pos_raiz == -1){
        printf("Sem livros cadastrados!!\n");
        system("pause");
        system("cls");
        free(cab);
        return;
    }

    Fila deq = criar_fila();
    no* x = ler_no(arq_bin, cab->pos_raiz);
	no* aux[1000];
	int i = 0;

    enqueue(deq, x);
    enqueue(deq, NULL);
	aux[i++] = x;

    printf("IMPRESSAO-POR-NIVEIS\n");

    while(!fila_vazia(deq)){
        no* atual = dequeue(deq);

        if(atual == NULL){
            printf("\n");

            if(!fila_vazia(deq))
                enqueue(deq, NULL);
        }
        else{
            printf("%d ", atual->livro.codigo);

            if(atual->esq != -1){
                x = ler_no(arq_bin, atual->esq);
                enqueue(deq, x);
				aux[i++] = x;
            }

            if(atual->dir != -1){
                x = ler_no(arq_bin, atual->dir);
                enqueue(deq, x);
				aux[i++] = x;
            }
        }
    }
	i--;
	while(i >= 0)
		free(aux[i--]);

    deq = liberar_fila(deq);
    free(cab);
    system("pause");
    system("cls");
}

// Funcao para alocar strings
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void alocar_string(Livro* livro)
{
    livro->titulo = (char*) calloc(151, sizeof(char));
    livro->autor = (char*) calloc(201, sizeof(char));
    livro->editora = (char*) calloc(51, sizeof(char));

    if(!livro->titulo || !livro->autor || !livro->editora)
        erro();
}
