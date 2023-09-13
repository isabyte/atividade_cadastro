#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pessoa {
    int codigo;
    char nome[50];
    char telefone[16];
    int idade;
    float altura;
} Pessoa;

typedef struct No {
    Pessoa pessoa;
    struct No* prox;
} No;

typedef struct Lista {
    No* inicio;
} Lista;

// inicializa a lista (inicio = null)
void inicializa(Lista* lista) {
    lista->inicio = NULL;
}

// padroniza strings lidas do teclado
void padroniza_string(char string[], int comprimento_string) {
    if (comprimento_string > 0) {
        // se o ultimo caractere for \n, elimina-o e o substitui por \0
        if (string[comprimento_string - 1] == '\n') {
            string[comprimento_string - 1] = '\0';
        } else {
            // se o \n nao estiver presente, o nome excedeu o tamanho da string
            // limpa os caracteres nao lidos do buffer de entrada
            char c;
            do {
                c = fgetc(stdin);
            } while (c != '\n' && c != EOF);
        }
    }
}

// cadastro de pessoas
Pessoa cadastro() {
    Pessoa p;

    puts("* CADASTRO *\n");
    puts("Digite o codigo: ");
    scanf("%d", &p.codigo);
    getchar();

    puts("Digite o nome: ");
    p.nome[0] = '\0'; // garante que a string esteja vazia antes de ler
    fgets(p.nome, sizeof(p.nome), stdin);
    int comp = strlen(p.nome);
    padroniza_string(p.nome, comp);

    puts("Digite o numero de telefone:");
    p.telefone[0] = '\0'; // garante que a string esteja vazia antes de ler
    fgets(p.telefone, sizeof(p.telefone), stdin);
    comp = strlen(p.telefone);
    padroniza_string(p.telefone, comp);

    puts("Digite a idade: ");
    scanf("%d", &p.idade);
    getchar();

    puts("Digite a altura: ");
    scanf("%f", &p.altura);
    getchar();

    return p;
}

// adiciona pessoas ao início da lista
/*
void adicionar_pessoa_inicio(Lista* lista) {
    Pessoa p = cadastro();

    No* novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        puts("Erro ao alocar memoria. O programa sera encerrado\n");
        exit(1);
    }

    novo->pessoa = p; //(*novo).pessoa = p
    novo->prox = lista->inicio; // o próximo elemento do novo nó aponta para oq antes era o início
    lista->inicio = novo; // o início agora aponta para o novo nó
}
 */

// adiciona pessoas ao final da lista
void adicionar_pessoa_final(Lista* lista) {
    Pessoa p = cadastro();

    No* novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        puts("Erro ao alocar memoria. O programa sera encerrado\n");
        exit(1);
    }

    // atribui ao novo nó a pessoa cadastrada
    novo->pessoa = p;
    // atribui ao endereço do próximo no o valor nulo
    // pois será colocado no final da lista
    novo->prox = NULL;

    // se a lista estiver vazia
    if(lista->inicio == NULL) {
        lista->inicio = novo; // o novo no sera seu primeiro elemento
    } else { // se a lista não estiver vazia
        No* atual = lista->inicio; // cria-se um no auxiliar que recebe o inicio da lista

        while(atual->prox != NULL) { // enquanto o proximo elemento nao for nulo
            atual = atual->prox; // percorre a lista, pois nao chegou ao seu final
        }
        // sai do while quando o atual estiver no ultimo no
        atual->prox = novo; // atribui o endereço novo no ao proximo do ultimo no
    }
}

// imprime a lista
void imprime_lista(Lista* lista) {
    No* atual = lista->inicio; // o elemento atual recebe o inicio da lista

    // se a lista estiver vazia, seu inicio é nulo
    if(atual == NULL) {
        printf("A lista esta vazia\n");
        return;
    }

    // enquanto a lista não for vazia, imprime seus componentes
    while(atual != NULL){
        printf("Codigo: %d\n", atual->pessoa.codigo);
        printf("Nome: %s\n", atual->pessoa.nome);
        printf("Telefone: %s\n", atual->pessoa.telefone);
        printf("Idade: %d\n", atual->pessoa.idade);
        printf("Altura: %.2f\n", atual->pessoa.altura);
        printf("\n");

        // atualiza o valor de atual para o proximo elemento da lista
        atual = atual->prox;
    }

}

// remove pessoas da lista pelo codigo
void remove_pessoa(Lista* lista) {
    imprime_lista(lista);

    if(lista->inicio == NULL) {
        return;
    }

    int codigo;
    printf("Digite o codigo da pessoa a ser removida: ");
    scanf("%d", &codigo);
    getchar();

    No* atual = lista->inicio; // no auxiliar que indica o no atual da lista
    No* remover = NULL; // no a ser removido

    // se o primeiro elemento da lista for null, quer dizer que ela esta vazia
    if(atual == NULL) {
        printf("A lista esta vazia\n");
        return; // sai da funcao
    }

    // se o no a ser removido estiver no inicio da lista
    if(atual->pessoa.codigo == codigo) {
        remover = atual; // atribui o no atual ao no a ser removido
        lista->inicio = remover->prox; // atribui ao inicio da lista o ponteiro para onde o proximo do no a ser removido apontava
    } else { // se o no a ser removido nao estiver no inicio da lista
        // enquanto existir um proximo elemento e esse proximo elemento for diferente do codigo a ser removido
        // o no atual vai percorrendo a lista
        while(atual->prox != NULL && atual->prox->pessoa.codigo != codigo) {
            atual = atual->prox;
        }
        // se o proximo no existir e for igual ao codigo a seer removido
        if(atual->prox != NULL && atual->prox->pessoa.codigo == codigo) {
            remover = atual->prox; // atribui o proximo no ao no a ser removido
            atual->prox = remover->prox; // atribui ao ponteiro proximo do no atual o endereço do no que era proximo ao no a ser removido
        } else {
            printf("ERRO! Codigo nao encontrado\n");
        }
    }

    if(remover != NULL) {
        printf("Cadastro removido com sucesso!\n");
        free(remover);
    }
}

// percorre a lista buscando se um codigo existe
// se existir, retorna o no correspondente
// se nao existir, retorna null
No* procura_cod(Lista* lista, int codigo) {
    No* atual = lista->inicio; // no auxiliar que vai percorrer a lista
    No* achou = NULL; // achou comeca com nulo

    // se a lista estiver vazia sai da funcao
    if(atual == NULL) {
        puts("A lista esta vazia\n");
        return 0;
    }

    // se o codigo estiver no inicio da lista
    if(atual->pessoa.codigo == codigo) {
        achou = atual; // achou recebe o endereco do no
        return achou;
    }
    // se o codigo nao estiver no inicio da lista
    // enquanto existir um proximo elemento e esse proximo elemento for diferente do codigo procurado
    // o no atual vai percorrendo a lista
    while(atual->prox != NULL && atual->prox->pessoa.codigo != codigo) {
        atual = atual->prox;
    }
    // se o proximo no existir e seu codigo for igual ao codigo procurado
    if(atual->prox != NULL && atual->prox->pessoa.codigo == codigo) {
        achou = atual->prox; // achou recebe o endereco do no
    }

    return achou;

}

// imprime um no
void imprime_no(No* no) {
    if(no == NULL) {
        puts("Pessoa nao encontrada\n");
        return;
    }

    printf("Codigo: %d\n", no->pessoa.codigo);
    printf("Nome: %s\n", no->pessoa.nome);
    printf("Telefone: %s\n", no->pessoa.telefone);
    printf("Idade: %d\n", no->pessoa.idade);
    printf("Altura: %.2f\n", no->pessoa.altura);
    printf("\n");

}

// busca uma pessoa pelo seu codigo
// se achar, imprime o nome e retorna seu no
// se nao achar, retorna null
No* busca_pessoa(Lista* lista) {
    // sai da funcao se a lista estiver vazia
    if(lista->inicio == NULL) {
        puts("Nao ha pessoas cadastradas\n");
        return NULL;
    }

    int codigo;
    printf("Digite o codigo da pessoa: ");
    scanf("%d", &codigo);
    getchar();

    No* pessoa = procura_cod(lista, codigo);

    if(pessoa != NULL) {
        printf("\nPessoa encontrada: %s\n", pessoa->pessoa.nome);
        return pessoa;
    } else {
        puts("\nPessoa nao encontrada\n");
        return NULL;
    }
}

// imprime os dados de uma pessoa especifica
void imprime_pessoa(Lista* lista) {
    No* pessoa = busca_pessoa(lista);

    if(pessoa != NULL) {
        puts("\n* Pessoa encontrada *\n");
        imprime_no(pessoa);
    }
}

int main() {
    int loop_menu = 1;
    int opc;

    Lista* lista = (Lista*) malloc(sizeof(Lista));

    if (lista == NULL)
    {
        puts("ERRO! Nao foi possivel alocar memoria\n");
        exit(1);
    }

    inicializa(lista);

    while(loop_menu) {
        puts("\nSelecione a opcao desejada:\n");
        printf("1- Adicionar pessoa\n"
        "2- Buscar pessoa pelo codigo\n"
        "3- Remover pessoa a partir do codigo\n"
        "4- Imprimir os dados de uma pessoa especifica\n"
        "5- Imprimir todas as pessoas\n"
        "0- Sair\n");
        scanf("%d", &opc);
        getchar();
        switch (opc) {
            case 1:
                puts("Voce selecionou: Adicionar pessoa\n");
                adicionar_pessoa_final(lista);
                break;
            case 2:
                puts("Voce selecionou: Buscar pessoa pelo codigo\n");
                busca_pessoa(lista);
                break;
            case 3:
                puts("Voce selecionou: Remover pessoa a partir do codigo\n");
                remove_pessoa(lista);
                break;
            case 4:
                puts("Voce selecionou: Imprimir os dados de uma pessoa especifica\n");
                imprime_pessoa(lista);
                break;
            case 5:
                puts("Voce selecionou: Imprimir todas as pessoas\n");
                imprime_lista(lista);
                break;
            case 0:
                puts("Voce selecionou: Sair\nAte logo! :)\n");
                loop_menu = 0;
                break;
            default:
                puts("ERRO! Opcao invalida, tente novamente\n");
        }
    }

    return 0;
}
