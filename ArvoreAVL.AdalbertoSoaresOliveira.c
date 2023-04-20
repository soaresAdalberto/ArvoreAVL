#include <stdio.h>
#include <stdlib.h>

// Define a estrutura de um nó da árvore
typedef struct node {
    int chave;
    struct node *esq;
    struct node *dir;
    int altura;
} Node;

// Função auxiliar para criar um novo nó da árvore
Node *criaNo(int chave) {
    Node *novoNo = (Node*) malloc(sizeof(Node));
    novoNo->chave = chave;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->altura = 1;
    return novoNo;
}

// Função auxiliar para calcular a altura de um nó
int altura(Node *no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

// Função auxiliar para calcular o fator de balanceamento de um nó
int fatorBalanceamento(Node *no) {
    if (no == NULL) {
        return 0;
    }
    return altura(no->esq) - altura(no->dir);
}

// Função auxiliar para atualizar a altura de um nó
void atualizaAltura(Node *no) {
    int alturaEsq = altura(no->esq);
    int alturaDir = altura(no->dir);
    no->altura = (alturaEsq > alturaDir ? alturaEsq : alturaDir) + 1;
}

// Função auxiliar para realizar uma rotação simples à esquerda em um nó
Node *rotacaoEsquerda(Node *no) {
    Node *noDir = no->dir;
    Node *noDirEsq = noDir->esq;
    noDir->esq = no;
    no->dir = noDirEsq;
    atualizaAltura(no);
    atualizaAltura(noDir);
    return noDir;
}

// Função auxiliar para realizar uma rotação simples à direita em um nó
Node *rotacaoDireita(Node *no) {
    Node *noEsq = no->esq;
    Node *noEsqDir = noEsq->dir;
    noEsq->dir = no;
    no->esq = noEsqDir;
    atualizaAltura(no);
    atualizaAltura(noEsq);
    return noEsq;
}

// Função auxiliar para realizar uma rotação dupla à esquerda em um nó
Node *rotacaoDuplaEsquerda(Node *no) {
    no->dir = rotacaoDireita(no->dir);
    return rotacaoEsquerda(no);
}

// Função auxiliar para realizar uma rotação dupla à direita em um nó
Node *rotacaoDuplaDireita(Node *no) {
    no->esq = rotacaoEsquerda(no->esq);
    return rotacaoDireita(no);
}

// Função para inserir um novo nó em uma árvore AVL
Node *insere(Node *raiz, int chave) {
    // Caso base: a árvore está vazia, então criamos um novo nó com a chave
    if (raiz == NULL) {
        return criaNo(chave);
    }

    // Caso contrário, percorremos a árvore até encontrar o lugar correto para inserir o novo nó
if (chave < raiz->chave) {
    raiz->esq = insere(raiz->esq, chave);
} else if (chave > raiz->chave) {
    raiz->dir = insere(raiz->dir, chave);
} else {
    // Se a chave já existe na árvore, não fazemos nada
    return raiz;
}

// Atualiza a altura do nó
atualizaAltura(raiz);

// Calcula o fator de balanceamento do nó
int fb = fatorBalanceamento(raiz);

// Verifica se o nó está desbalanceado e realiza as rotações necessárias
if (fb > 1 && chave < raiz->esq->chave) {
    return rotacaoDireita(raiz);
} else if (fb > 1 && chave > raiz->esq->chave) {
    return rotacaoDuplaDireita(raiz);
} else if (fb < -1 && chave > raiz->dir->chave) {
    return rotacaoEsquerda(raiz);
} else if (fb < -1 && chave < raiz->dir->chave) {
    return rotacaoDuplaEsquerda(raiz);
}

// Retorna a raiz da árvore atualizada
return raiz;

}

// Função para remover um nó de uma árvore AVL
Node *removeNode(Node *raiz, int chave) {
// Caso base: a árvore está vazia, então não há nada para remover
if (raiz == NULL) {
return raiz;
}

// Caso contrário, percorremos a árvore até encontrar o nó com a chave
if (chave < raiz->chave) {
    raiz->esq = removeNode(raiz->esq, chave);
} else if (chave > raiz->chave) {
    raiz->dir = removeNode(raiz->dir, chave);
} else {
    // Caso 1: o nó a ser removido não tem filho ou tem apenas um filho
    if (raiz->esq == NULL || raiz->dir == NULL) {
        Node *temp = raiz->esq ? raiz->esq : raiz->dir;
        // Caso 1.1: o nó não tem filho
        if (temp == NULL) {
            temp = raiz;
            raiz = NULL;
        } else {
            // Caso 1.2: o nó tem apenas um filho
            *raiz = *temp;
        }
        free(temp);
    } else {
        // Caso 2: o nó a ser removido tem dois filhos
        Node *temp = raiz->dir;
        while (temp->esq != NULL) {
            temp = temp->esq;
        }
        raiz->chave = temp->chave;
        raiz->dir = removeNode(raiz->dir, temp->chave);
    }
}

// Se a árvore ficou vazia após a remoção, não precisamos balancear
if (raiz == NULL) {
    return raiz;
}

// Atualiza a altura do nó
atualizaAltura(raiz);

// Calcula o fator de balanceamento do nó
int fb = fatorBalanceamento(raiz);

// Verifica se o nó está desbalanceado e realiza as rotações necessárias
if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0) {
    return rotacaoDireita(raiz);
} else if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
    return rotacaoDuplaDireita(raiz);
} else if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0) {
    return rotacaoEsquerda(raiz);
} else if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
    return rotacaoDuplaEsquerda(raiz);
}

// Retorna a raiz da árvore atualizada
return raiz;
}

// Função para verificar se uma árvore é AVL balanceada
int verificaAVL(Node *raiz) {
    
// Caso base: a árvore está vazia, então é AVL balanceada
if (raiz == NULL) {
return 1;
}

// Verifica se o fator de balanceamento é válido e se as subárvores são AVL balanceadas
int fb = fatorBalanceamento(raiz);
if (fb > 1 || fb < -1) {
    return 0;
} else {
    return verificaAVL(raiz->esq) && verificaAVL(raiz->dir);
}

}

// Exemplo de uso da árvore AVL
int main() {
Node *raiz = NULL;

// Insere alguns nós na árvore
raiz = insere(raiz, 10);
raiz = insere(raiz, 20);
raiz = insere(raiz, 30);
raiz = insere(raiz, 40);
raiz = insere(raiz, 50);
raiz = insere(raiz, 25);
raiz = insere(raiz, 55);
raiz = insere(raiz, 15);
raiz = insere(raiz, 5);
raiz = insere(raiz, 60);

// Remove um nó da árvore
raiz = removeNode(raiz, 30);
raiz = removeNode(raiz, 20);
raiz = removeNode(raiz, 15);
raiz = removeNode(raiz, 40);
raiz = removeNode(raiz, 60);

// Verifica se a árvore é AVL balanceada
if (verificaAVL(raiz)) {
    printf("A árvore é AVL balanceada.\n");
} else {
    printf("A árvore não é AVL balanceada.\n");
}

return 0;
}