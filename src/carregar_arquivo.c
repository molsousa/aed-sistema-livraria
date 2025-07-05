#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header/livro.h"
#include "../header/carregar_arquivo.h"

void carregar_arquivo(FILE* arq_bin)
{
    char input[TAM/4];
    char leitura[TAM];
    char* base;
    FILE* arq_texto;

    printf("Digite o nome do arquivo: ");
    scanf("%[^\n]%*c", input);
    arq_texto = fopen(input, "r");

    while(fgets(leitura, TAM, arq_texto) != NULL){
        register int i = 0;
        Livro livro;
        alocar_string(&livro);
        base = leitura;
        base = retirar_espaco(base);

        sscanf(base, "%d", &livro.codigo);
        base++;
        base = retirar_espaco(base);

        // printf("%d\n", livro.codigo); // [DEBUG]
        base = cortar_ponto(base);
        sscanf(base, "%[^;];%[^;];%[^;]", livro.titulo, livro.autor, livro.editora);
        trim(livro.titulo);
        trim(livro.autor);
        trim(livro.editora);

        // printf("%s\n%s\n%s\n", livro.titulo, livro.autor, livro.editora); // [DEBUG]

        while(i != 4){
            if(*base < '0' || *base > '9')
                base++;
            else
                i++;
        }
        sscanf(base, "%d", &livro.edicao);
        base = cortar_ponto(base);
        sscanf(base, "%d", &livro.ano);
        base = cortar_ponto(base);
        sscanf(base, "%d", &livro.exemplares);
        base = cortar_ponto(base);
        // printf("%d - %d - %d\n", livro.edicao, livro.ano, livro.exemplares); // [DEBUG]

        // printf("%s\n", base); // [DEBUG]

        sscanf(base, "%d,%d", &livro.preco[0], &livro.preco[1]);
        // printf("%d,%02d\n", livro.preco[0], livro.preco[1]); // [DEBUG]

        inserir_no(arq_bin, livro);
    }
    fclose(arq_texto);
}

void trim(char* str)
{
    int inicio = 0;
    int fim = strlen(str) - 1;

    while(isspace(str[inicio]))
        inicio++;

    while(fim > inicio && isspace(str[fim]))
        fim--;

    if(inicio > 0 || fim < (strlen(str) -1)){
        memmove(str, str + inicio, fim - inicio + 1);
        str[fim - inicio + 1] = '\0';
    }
}

char* retirar_espaco(char* str)
{
    while(isspace(*str))
        str++;

    return str;
}

char* cortar_ponto(char* str)
{
    register int i = 0;

    while(str[i] != ';')
        str++;
    str++;

    return str;
}
