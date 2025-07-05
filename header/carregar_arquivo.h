#ifndef _CARREGAR_ARQUIVO_H_INCLUDED
#define _CARREGAR_ARQUIVO_H_INCLUDED
#define TAM 2000

// Funcao para carregar arquivo de texto
// Pre-condicao: nenhuma
// Pos-condicao: carrega arvore binaria com registros
void carregar_arquivo(FILE* arq_bin);

// Funcao para cortar espacos
// Pre-condicao: nenhuma
// Pos-condicao: retira os espacos de uma string
void trim(char* str);

// Funcao para retirar espacos
// Pre-condicao: nenhuma
// Pos-condicao: retorna posicao em memoria sem espacos
char* retirar_espaco(char* str);

// Funcao para cortar ponto-virgula
// Pre-condicao: nenhuma
// Pos-condicao: retorna uma string com corte no ponto-virgula
char* cortar_ponto(char* str);

#endif // _CARREGAR_ARQUIVO_H_INCLUDED
