# Sistema de Livraria

## Descrição

Este repositório contém um programa que simula um sistema de livraria utilizando árvore binária em arquivo binário. O sistema foi desenvolvido para a disciplina de Algoritmos e Estruturas de Dados.

## Opções para usuário

O usuário tem acesso a:
- Cadastro de livros.
- Imprimir dados do livro (in-order).
- Listar todos os livros: imprimindo código, título, autor e exemplares disponíveis.
- Verificar todos os diferentes livros cadastrados.
- Remover um determinado livro.
- Carregar arquivo de texto.
- Imprimir lista de registros livres.
- Imprimir por níveis os códigos dos livros cadastrados.
	
## Implementação

- O código foi escrito em C.
- A estrutura de dados utilizada foi a árvore de busca binária implementada em arquivo binário.
- A cada remoção de determinado nó da árvore, esse nó é marcado como livre para ser reutilizado em caso de nova inserção, é fácil entender essa lógica imaginando uma prateleira sendo liberada para um novo livro.
- A árvore binária de busca é muito mais eficiente que uma lista encadeada se estritamente binária. Menos eficiente que outras árvores de busca como as balanceadas.
	
## Como utilizar

### Carregar arquivo de texto

O arquivo a ser carregado pela função <carregar_arquivo> deve seguir a seguinte sintaxe:

	<código>;<título>;<autor>;<editora>;<edição>;<ano>;<exemplares>;<preço>

O preço deve ser escrito com vírgula, seguindo o exemplo abaixo:

	13; Algoritmos e Estruturas de Dados em Linguagem C; André Backes; GEN; 1; 2025; 20; 205,50 

Espaçamento não é o problema, a menos que a sintaxe seja seguida.

Há um exemplo de arquivo de texto contido no repositório.

### Compilação e Execução

É necessário compilar os arquivos de código fonte e executar o programa:

	gcc main.c .\src\*.c -o livraria
    .\livraria
