#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>

typedef struct Item {
    char nome[50];
    struct Item *prox;
} Item;

int menu(const char *titulo, Item *itens, bool sair);
void exibir_itens(Item *itens);

Item* inserir_item(Item *lista, const char *nome);

void esperar_enter();
void limpa_buffer();

void limpa_tela();

#endif