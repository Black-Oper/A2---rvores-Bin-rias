#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

// Função para inserir um item no menu
Item* inserir_item(Item *lista, const char *nome) {
    
    Item *novo = (Item *)malloc(sizeof(Item));
    if (!novo) {
        fprintf(stderr, "Erro de alocação de memória!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(novo->nome, nome);
    novo->prox = NULL;

    if (lista == NULL) {
        return novo;
    }

    Item *aux = lista;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;

    return lista;
}

// Função para aguardar a confirmação do usuário
void esperar_enter() {
    printf("\nPressione Enter para continuar...\n");
    limpa_buffer();
    getchar();
}

// Função para limpar o buffer do teclado
void limpa_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para limpar a tela
void limpa_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}