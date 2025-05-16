#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

typedef enum { RED, BLACK } Color;

typedef struct {
    int code;
    char name[50];
    int quantity;
    float price;
} Product;

typedef struct Node {
    Product prod;
    Color color;
    struct Node *left, *right, *parent;
} Node;

/* Cria um novo nó Rubro-Negro com o produto dado, cor RED e ponteiros nulos */
Node* createNode(Product p) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->prod = p;
    n->color = RED;
    n->left = n->right = n->parent = NULL;
    return n;
}

/* Realiza rotação à esquerda em torno de x, retornando a nova raiz */
Node* rotateLeft(Node* root, Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left)  {
        x->parent->left  = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return root;
}

/* Realiza rotação à direita em torno de y, retornando a nova raiz */
Node* rotateRight(Node* root, Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (!y->parent) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left  = x;
    }

    x->right = y;
    y->parent = x;
    return root;
}

/* Insere n em uma BST simples com base no código do produto */
Node* bstInsert(Node* root, Node* n) {
    if (!root) {
        return n;
    }
    if (n->prod.code < root->prod.code) {
        root->left  = bstInsert(root->left, n);
        root->left->parent = root;
    }
    else if (n->prod.code > root->prod.code) {
        root->right = bstInsert(root->right, n);
        root->right->parent = root;
    }
    return root;
}

/* Reequilibra a árvore após inserção, corrigindo cores e rotações */
Node* fixInsert(Node* root, Node* z) {
    Node *uncle;
    while (z != root && z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            uncle = z->parent->parent->right;
            if (uncle && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color     = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    root = rotateLeft(root, z);
                }
                z->parent->color          = BLACK;
                z->parent->parent->color  = RED;
                root = rotateRight(root, z->parent->parent);
            }
        } else {
            uncle = z->parent->parent->left;
            if (uncle && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color     = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    root = rotateRight(root, z);
                }
                z->parent->color          = BLACK;
                z->parent->parent->color  = RED;
                root = rotateLeft(root, z->parent->parent);
            }
        }
    }
    if(root)
        root->color = BLACK;
    return root;
}

/* Insere um produto criando o nó, inserindo em BST e ajustando cores */
Node* insert(Node* root, Product p) {
    Node* n = createNode(p);
    root = bstInsert(root, n);
    root = fixInsert(root, n);
    return root;
}

/* Percorre a árvore em ordem e imprime cada produto formatado */
void inorderPrint(Node* root) {
    if (!root) {
        return;
    }

    inorderPrint(root->left);

    printf("Código: %3d | %-20s | Qtde: %3d | Preço: %8.2f | Cor: %c\n",
           root->prod.code,
           root->prod.name,
           root->prod.quantity,
           root->prod.price,
           (root->color == RED ? 'R' : 'B'));

    inorderPrint(root->right);
}

/* Busca recursivamente um nó pelo código do produto */
Node* search(Node* root, int code) {
    if (!root || root->prod.code == code) {
        return root;
    }
    if (code < root->prod.code) {
        return search(root->left, code);
    } else {
        return search(root->right, code);
    }
}

/* Substitui o subárvore em u pela subárvore em v, ajustando pais */
void transplant(Node** root, Node* u, Node* v) {
    if (!u->parent){
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left  = v;
    }
    else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

/* Retorna o nó de menor chave na subárvore de x (extremo à esquerda) */
Node* treeMinimum(Node* x) {
    while (x->left) {
        x = x->left;
    }
    return x;
}

/* Reequilibra a árvore após remoção, corrigindo cores e rotações */
Node* fixRemove(Node* root, Node* x) {
    Node *w;
    while (x != root && (!x || x->color == BLACK)) {
        if (x && x->parent && x == x->parent->left) {
            w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                root = rotateLeft(root, x->parent);
                w = x->parent->right;
            }
            if (w && (!w->left  || w->left->color  == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                if(w) w->color = RED;
                if(x) x = x->parent;
            } else {
                if (w && (!w->right || w->right->color == BLACK)) {
                    if (w->left) w->left->color = BLACK;
                    if(w) w->color = RED;
                    root = rotateRight(root, w);
                    w = x->parent->right;
                }
                if(w){
                    w->color          = x->parent->color;
                    x->parent->color  = BLACK;
                    if (w->right) {
                        w->right->color = BLACK;
                    }
                    root = rotateLeft(root, x->parent);
                }
                x = root;
            }
        } else if (x && x->parent) {
            w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                root = rotateRight(root, x->parent);
                w = x->parent->left;
            }
            if (w && (!w->left  || w->left->color  == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                if(w) w->color = RED;
                if(x) x = x->parent;
            } else {
                if (w && (!w->left || w->left->color == BLACK)) {
                    if (w->right) {
                        w->right->color = BLACK;
                    }
                    if(w) w->color = RED;
                    root = rotateLeft(root, w);
                    w = x->parent->left;
                }
                if(w){
                    w->color = x->parent->color;
                    x->parent->color = BLACK;

                    if (w->left) {
                        w->left->color = BLACK;
                    }
                    root = rotateRight(root, x->parent);
                }
                x = root;
            }
        } else {
            break;
        }
    }
    if (x) {
        x->color = BLACK;
    }
    return root;
}

/* Remove o nó com dado código e reequilibra se necessário */
Node* removeNode(Node* root, int code) {
    Node *z = search(root, code);
    if (!z) return root;
    Node *y = z;
    Node *x = NULL;
    Color yOrigColor = y->color;

    if (!z->left) {
        x = z->right;
        transplant(&root, z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(&root, z, z->left);
    } else {
        y = treeMinimum(z->right);
        yOrigColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(&root, y, y->right);
            y->right = z->right;
            if(y->right) y->right->parent = y;
        }
        transplant(&root, z, y);
        y->left = z->left;
        if(y->left) y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (yOrigColor == BLACK) {
        root = fixRemove(root, x);
    }
    return root;
}

/* Função principal: exibe menu e processa operações de inventário */
int main() {
    Node* root = NULL;
    int op = -1;

    const char *titulo = "Sistema de Inventário - Rubro-Negra";

    void *itens = NULL;
    itens = inserir_item(itens, "Cadastrar produto");
    itens = inserir_item(itens, "Remover produto");
    itens = inserir_item(itens, "Buscar produto");
    itens = inserir_item(itens, "Listar todos os produtos");

    while(op != 0) {
        limpa_tela();
        op = menu(titulo, itens, 1);

        switch (op)
        {
        case 1: {
            Product p;
            printf("Código: ");
            scanf("%d", &p.code);
            limpa_buffer();
            printf("Nome: ");
            scanf(" %49[^\n]", p.name);
            limpa_buffer();
            printf("Quantidade: ");
            scanf("%d", &p.quantity);
            limpa_buffer();
            printf("Preço unitário: ");
            scanf("%f", &p.price);
            limpa_buffer();
            root = insert(root, p);
            printf("Produto cadastrado!\n");
            esperar_enter();
            break;
        }

        case 2: {
            int code;
            printf("Código do produto a remover: ");
            scanf("%d", &code);
            limpa_buffer();
            root = removeNode(root, code);
            printf("Operação de remoção realizada.\n");
            esperar_enter();
            break;
        }

        case 3: {
            int code;
            printf("Código do produto a buscar: ");
            scanf("%d", &code);
            limpa_buffer();
            Node* f = search(root, code);
            if (f) {
                printf("Encontrado: %d | %-20s | Qtde: %d | Preço: %.2f | Cor: %c\n",
                       f->prod.code, f->prod.name, f->prod.quantity, f->prod.price,
                       (f->color==RED?'R':'B'));
            }
            else {
                printf("Produto não encontrado.\n");
            }
            esperar_enter();
            break;
        }

        case 4:
            printf("\n--- Lista de Produtos (in-order) ---\n");
            inorderPrint(root);
            printf("------------------------------------\n");
            esperar_enter();
            break;
        default:
            if (op != 0) {
                printf("Opção inválida!");
                esperar_enter();
            }
            break;
        }
    }

    return 0;
}