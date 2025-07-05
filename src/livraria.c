#include <stdio.h>
#include <stdlib.h>
#include "../header/livro.h"
#include "../header/livraria.h"
#include "../header/carregar_arquivo.h"

// Funcao para inicializar arvore binaria
// Pre-condicao: arquivo inicializado
// Pos-condicao: inicializa o cabecalho
void criar_arvore(FILE* f)
{
    cabecalho* cab = (cabecalho*) malloc(sizeof(cabecalho));
    if(!cab)
        erro();

    cab->pos_raiz = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    cab->total_livros = 0;

    escrever_cabecalho(f, cab);
    free(cab);
}

// Funcao para escrever cabecalho
// Pre-condicao: arvore criada
// Pos-condicao: escreve alteracoes no cabecalho
void escrever_cabecalho(FILE* f, cabecalho* cab)
{
    fseek(f, 0, SEEK_SET);
    fwrite(cab, sizeof(cabecalho), 1, f);
}

// Funcao para ler o cabecalho
// Pre-condicao: arvore criada
// Pos-condicao: retorna a condicao do cabecalho
cabecalho* ler_cabecalho(FILE* f)
{
    cabecalho* cab = (cabecalho*) malloc(sizeof(cabecalho));
    if(!cab)
        erro();

    fseek(f, 0, SEEK_SET);
    fread(cab, sizeof(cabecalho), 1, f);

    return cab;
}

// Funcao para menu do usuario
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void menu()
{
    FILE* arq_bin = fopen("livraria.bin", "w+b");
    criar_arvore(arq_bin);
    int opcao;
    system("color B");

    do{
        opcoes();
        printf("Insira uma opcao: ");
        scanf("%d%*c", &opcao);
        system("cls");

        switch(opcao){
        case 1:
            cadastrar_livro(arq_bin);
            break;
        case 2:
            imprimir_codigo(arq_bin);
            break;
        case 3:
            listar_livros(arq_bin);
            break;
        case 4:
            total_livros(arq_bin);
            break;
        case 5:
            remover_no(arq_bin);
            break;
        case 6:
            carregar_arquivo(arq_bin);
            break;
        case 7:
            imprimir_registros_livres(arq_bin);
            break;
        case 8:
            imprimir_niveis(arq_bin);
            break;
        case 0:
            printf("Obrigado!!\n");
            break;
        default:
            printf("Opcao invalida, tente novamente\n");
        }

    }while(opcao != 0);

    fclose(arq_bin);
}

// Funcao para opcoes do usuario
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void opcoes()
{
    printf("=============LIVRARIA=============\n");
    printf("||1 - Cadastrar livro           ||\n");
    printf("||2 - Imprimir dados do livro   ||\n");
    printf("||3 - Listar todos os livros    ||\n");
    printf("||4 - Calcular total            ||\n");
    printf("||5 - Remover livro             ||\n");
    printf("||6 - Carregar arquivo          ||\n");
    printf("||7 - Imprimir registros livres ||\n");
    printf("||8 - Imprimir por niveis       ||\n");
    printf("||0 - Sair                      ||\n");
    printf("==================================\n");
}

// Funcao para imprimir registros livres
// Pre-condicao: nenhuma
// Pos-condicao: nenhuma
void imprimir_registros_livres(FILE* f)
{
    cabecalho* cab = ler_cabecalho(f);
    int pos = cab->pos_livre;

    if(pos == -1)
        printf("Nao ha registros livres\n");

    else{
        printf("Registros livres: ");

        while(pos != -1){
            printf("Posicao: %d\n", pos);
            no* x = ler_no(f, pos);
            pos = x->dir;
            free(x);
        }
    }
    free(cab);
}

// Funcao para alertar erro
// Pre-condicao: erro encontrado
// Pos-condicao: fecha execucao
void erro()
{
    printf("Erro ao alocar memoria!!\n");
    exit(1);
}
