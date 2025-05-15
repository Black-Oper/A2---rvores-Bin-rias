#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcoes.h"

// Função para exibir um menu básico
int menu(const char *titulo, Item *itens, bool sair) {
    system("clear");

    char concluido[100];
    snprintf(concluido, sizeof(concluido), "MENU : %s", titulo);

    printf("%s\n", concluido);

    char separador[100];
    int tam = strlen(concluido);
    for (int i = 0; i < tam; i++) {
        separador[i] = '=';
    }
    separador[tam] = '\0';

    printf("%s\n", separador);

    exibir_itens(itens);

    if (sair) {
        printf("[ 0 ] - Sair\n");
    } else {
        printf("[ 0 ] - Voltar\n");
    }

    printf("%s\n", separador);

    printf("Escolha uma opção: ");
    int opcao;
    scanf("%d", &opcao);

    return opcao;
}

// Função para exibir itens do menu
void exibir_itens(Item *itens) {
    Item *atual = itens;
    int i = 1;

    while (atual != NULL) {
        printf("[ %d ] - %s\n", i, atual->nome);
        atual = atual->prox;
        i++;
    }
}