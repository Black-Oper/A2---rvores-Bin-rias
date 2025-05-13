#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

struct No {
    char nome[50];
    int id;
    char email[50];
    int alt;
    struct No *esq;
    struct No *dir;
};

typedef struct No No;

int altura_no(No *no) {
    return (no == NULL) ? -1 : no->alt;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int fator_balanceamento_no(No *no) {
    return altura_no(no->esq) - altura_no(no->dir);
}

No *rotacao_ll(No *raiz) {
    No *filho = raiz->esq;
    raiz->esq = filho->dir;
    filho->dir = raiz;
    raiz->alt = maior(altura_no(raiz->esq), altura_no(raiz->dir)) + 1;
    filho->alt = maior(altura_no(filho->esq), altura_no(filho->dir)) + 1;
    return filho;
}

No *rotacao_rr(No *raiz) {
    No *filho = raiz->dir;
    raiz->dir = filho->esq;
    filho->esq = raiz;
    raiz->alt = maior(altura_no(raiz->esq), altura_no(raiz->dir)) + 1;
    filho->alt = maior(altura_no(filho->esq), altura_no(filho->dir)) + 1;
    return filho;
}

No *rotacao_lr(No *raiz) {
    raiz->esq = rotacao_rr(raiz->esq);
    return rotacao_ll(raiz);
}

No *rotacao_rl(No *raiz) {
    raiz->dir = rotacao_ll(raiz->dir);
    return rotacao_rr(raiz);
}

No *cria_usuario(const char *nome, int id, const char *email) {
    No *novo = (No*)malloc(sizeof(No));
    if (!novo) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(novo->nome, nome);
    novo->id = id;
    strcpy(novo->email, email);
    novo->alt = 0;
    novo->esq = novo->dir = NULL;
    return novo;
}

No *insere_AVLTree(No *raiz, const char *nome, int id, const char *email) {
    if (raiz == NULL)
        return cria_usuario(nome, id, email);

    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0)
        raiz->esq = insere_AVLTree(raiz->esq, nome, id, email);
    else if (cmp > 0)
        raiz->dir = insere_AVLTree(raiz->dir, nome, id, email);
    else
        return raiz;

    raiz->alt = maior(altura_no(raiz->esq), altura_no(raiz->dir)) + 1;
    int fb = fator_balanceamento_no(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacao_ll(raiz);
    if (fb < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacao_rr(raiz);
    if (fb > 1 && strcmp(nome, raiz->esq->nome) > 0) {
        raiz->esq = rotacao_rr(raiz->esq);
        return rotacao_ll(raiz);
    }
    if (fb < -1 && strcmp(nome, raiz->dir->nome) < 0) {
        raiz->dir = rotacao_ll(raiz->dir);
        return rotacao_rr(raiz);
    }
    return raiz;
}

No *menorValor(No *no) {
    No *atual = no;
    while (atual->esq)
        atual = atual->esq;
    return atual;
}

No *remover(No *raiz, const char *nome) {
    if (raiz == NULL)
        return raiz;

    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (cmp > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            No *temp = menorValor(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = remover(raiz->dir, temp->nome);
        }
    }
    if (raiz == NULL)
        return raiz;

    raiz->alt = maior(altura_no(raiz->esq), altura_no(raiz->dir)) + 1;
    int fb = fator_balanceamento_no(raiz);

    if (fb > 1 && fator_balanceamento_no(raiz->esq) >= 0)
        return rotacao_ll(raiz);
    if (fb > 1 && fator_balanceamento_no(raiz->esq) < 0) {
        raiz->esq = rotacao_rr(raiz->esq);
        return rotacao_ll(raiz);
    }
    if (fb < -1 && fator_balanceamento_no(raiz->dir) <= 0)
        return rotacao_rr(raiz);
    if (fb < -1 && fator_balanceamento_no(raiz->dir) > 0) {
        raiz->dir = rotacao_ll(raiz->dir);
        return rotacao_rr(raiz);
    }
    return raiz;
}

No *buscar(No *raiz, const char *nome) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;
    if (strcmp(nome, raiz->nome) < 0)
        return buscar(raiz->esq, nome);
    else
        return buscar(raiz->dir, nome);
}

void inOrder(No *raiz) {
    if (raiz) {
        inOrder(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        inOrder(raiz->dir);
    }
}

void freeTree(No *raiz) {
    if (raiz) {
        freeTree(raiz->esq);
        freeTree(raiz->dir);
        free(raiz);
    }
}

int main(void) {
    No *raiz = NULL;

    int op = -1;
    const char *titulo = "Gerenciamento de Usuarios - AVL Tree";

    Item *itens = NULL;
    itens = inserir_item(itens, "Cadastrar Usuario");
    itens = inserir_item(itens, "Remover Usuario");
    itens = inserir_item(itens, "Buscar Usuario");
    itens = inserir_item(itens, "Listar Usuarios (ordem alfabetica)");

    while (op != 0) {
        limpa_tela();
        op = menu(titulo, itens, true);

        switch (op) {
            case 1: {
                limpa_tela();
                char nome_input[50];
                int id_input;
                char email_input[50];

                printf("Digite o nome: ");
                scanf(" %s", nome_input);
                limpa_buffer();

                printf("Digite o ID: ");
                scanf("%d", &id_input);
                limpa_buffer();

                printf("Digite o email: ");
                scanf(" %s", email_input);

                raiz = insere_AVLTree(raiz, nome_input, id_input, email_input);
                printf("\nUsuario cadastrado com sucesso!\n");
                esperar_enter();
                break;
            }
            case 2: {
                limpa_tela();
                char nome_input[50];
                printf("Digite o nome do usuario a remover: ");
                scanf(" %s", nome_input);

                raiz = remover(raiz, nome_input);
                printf("\nUsuario removido (se existia)!\n");
                esperar_enter();
                break;
            }
            case 3: {
                limpa_tela();
                char nome_input[50];
                printf("Digite o nome do usuario a buscar: ");
                scanf(" %s", nome_input);

                No *res = buscar(raiz, nome_input);
                if (res) {
                    printf("\nUsuario encontrado:\n");
                    printf("Nome: %s | ID: %d | Email: %s\n", res->nome, res->id, res->email);
                } else {
                    printf("\nUsuario nao encontrado.\n");
                }
                esperar_enter();
                break;
            }
            case 4:
                limpa_tela();
                printf("Lista de usuarios (ordem alfabetica):\n\n");
                inOrder(raiz);
                esperar_enter();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                esperar_enter();
                break;
        }
    }

    freeTree(raiz);
    return 0;
}
