#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para despesas
typedef struct despesa
{
    int id;
    char *data;
    char descricao[100];
    float valor;
} Despesa;

// Função para adicionar uma despesa
void adicionarDespesa()
{
    Despesa *novo = malloc(sizeof(Despesa));
    novo->data = malloc(100); // Alocar memória para a data

    printf("Informe o ID da despesa: ");
    scanf("%d", &novo->id);
    getchar();
    printf("Informe a data da despesa: ");
    fgets(novo->data, 100, stdin);

    novo->descricao[0] = '\0'; // Inicialize a descrição com uma string vazia
    printf("Informe a descrição da despesa: ");
    fgets(novo->descricao, 100, stdin);

    printf("Informe o valor da despesa: ");
    scanf("%f", &novo->valor);

    getchar();

    FILE *arquivo = fopen("despesas.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de despesas.\n");
        return;
    }

    fprintf(arquivo, "ID: %d\n Data: %s Descrição: %s Valor: %.2f\n\n",
            novo->id,
            novo->data,
            novo->descricao,
            novo->valor);

    fclose(arquivo);

    printf("Despesa adicionada com sucesso.\n");
}

// Função para listar todas as despesas
void listarDespesas()
{
    char *nomeArquivo = "despesas.txt";
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        printf(" %s", linha);
    }

    fclose(arquivo);
}

// função para excluir uma despesa do arquivo
void excluirDispesa()
{
    char id[10];
    char id_nome[] = "ID: ";
    printf("Qual o id da dispesa que você deseja excluir? ");
    scanf("%s", id);
    strcat(id_nome, id);

    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    char linha[200];
    int excluirProximas = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (excluirProximas > 0)
        {
            excluirProximas--;
        }
        else
        {
            if (strstr(linha, id_nome))
            {
                excluirProximas = 4;
            }
            else
            {
                fputs(linha, temp);
            }
        }
    }

    fclose(arquivo);
    fclose(temp);
    remove("despesas.txt");
    rename("temp.txt", "despesas.txt");
}

// função para modificar despesa
void atualizarDespesa()
{
    char id[10];
    char id_nome[] = "ID: ";
    printf("Qual o id da despesa que você deseja modificar? ");
    scanf("%9s", id); // Limita a entrada a 9 caracteres para evitar estouro de buffer
    strcat(id_nome, id);
    getchar(); // Consumir o caractere de nova linha

    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    char linha[200];
    int encontrado = 0;
    int pular = 4;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (strstr(linha, id_nome))
        {
                encontrado = 1;
                fputs(linha, temp); // Mantém a linha do ID

                char data[100];
                char descricao[101];
                char valor[101];

                printf("Informe a data da despesa: ");
                fgets(data, sizeof(data), stdin);
                data[strlen(data) - 1] = '\0'; // Remova o caractere de nova linha
                getchar(); // Consumir o caractere de nova linha

                printf("Informe a descrição da despesa: ");
                fgets(descricao, sizeof(descricao), stdin);
                descricao[strlen(descricao) - 1] = '\0'; // Remova o caractere de nova linha
                getchar(); // Consumir o caractere de nova linha

                printf("Informe o valor da despesa: ");
                fgets(valor, sizeof(valor), stdin);
                //valor[strlen(valor) - 1] = '\0'; // Remova o caractere de nova linha
                

                fprintf(temp, " Data: %s\n Descrição: %s\n Valor: %s\n", data, descricao, valor);
            
                if(pular > 0){
                    for (int i = 0; i < pular; ++i) {
                        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
                            
                        }
                    }
                    pular = 0;
                }
                
        }
        else
        {
            fputs(linha, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);

    if (encontrado)
    {
        if (remove("despesas.txt") != 0)
        {
            printf("Erro ao remover o arquivo original.\n");
            exit(1);
        }

        if (rename("temp.txt", "despesas.txt") != 0)
        {
            printf("Erro ao renomear o arquivo temporário.\n");
            exit(1);
        }
    }
    else
    {
        printf("Despesa com ID %s não encontrada.\n", id);
        remove("temp.txt"); // Remove o arquivo temporário, pois a modificação não foi bem-sucedida
    }
}

// Função principal
int main()
{
    while (1)
    {
        printf("\nSistema de Gerenciamento de Finanças Pessoais\n");
        printf("1. Adicionar Despesa\n");
        printf("2. Listar Despesas\n");
        printf("3. excluir Despesa\n");
        printf("4. modificar Despesa\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");

        int opcao;
        scanf("%d", &opcao);

        getchar();
        switch (opcao)
        {
        case 1:
            adicionarDespesa();
            break;
        case 2:
            listarDespesas();
            break;
        case 3:
            excluirDispesa();
            break;
        case 4:
            atualizarDespesa();
            break;
        case 5:
            printf("Saindo do programa.\n");
            exit(0);
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}