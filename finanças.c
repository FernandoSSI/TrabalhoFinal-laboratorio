#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------BLOCO DO CÓDIGO QUE LIDAA COM AS DESPESAS ------------------------
// DEFINIÇÃO DOS DADOS DE DESPESA
typedef struct despesa
{
    int id;
    char *data;
    char descricao[100];
    float valor;
} Despesa;

// FUNÇÃO PARA ADICIONAR DEPESA NO ARQUIVO
void adicionarDespesa()
{
    Despesa *novo = malloc(sizeof(Despesa));
    novo->data = malloc(100);

    // REGISTRO DAS INFORMAÇÕES DA NOVA DESPESA
    printf("Informe o ID da despesa: ");
    scanf("%d", &novo->id);
    getchar();
    printf("Informe a data da despesa: ");
    fgets(novo->data, 100, stdin);
    novo->descricao[0] = '\0';
    printf("Informe a descricao da despesa: ");
    fgets(novo->descricao, 100, stdin);
    printf("Informe o valor da despesa: ");
    scanf("%f", &novo->valor);
    getchar();

    // ABRINDO O ARQUIVO TRATANDO ERRO, E ESCREVENDO NELE
    FILE *arquivo = fopen("despesas.txt", "at");
    if (arquivo == NULL)
    {
        // Se o arquivo não existe, tenta criar um novo
        arquivo = fopen("despesas.txt", "w");
        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo despesas.\n");
            return;
        }
        fclose(arquivo);
        // Agora tenta abrir novamente para escrever
        arquivo = fopen("despesas.txt", "a");
    }

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo despesas.\n");
        return;
    }

    // registrando as informações no arquivo txt
    fprintf(arquivo, "ID: %d\n Data: %s Descrição: %s Valor: %.2f\n\n",
            novo->id,
            novo->data,
            novo->descricao,
            novo->valor);
    fclose(arquivo);
    printf("Despesa adicionada com sucesso.\n");
}

// FUNÇÃO PARA LISTAGEM DE DESPESAS
void listarDespesas()
{
    //abrindo o arquivo e tratando erro
    char *nomeArquivo = "despesas.txt";
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    // imprimindo linha por linha do arquivo
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        printf(" %s", linha);
    }
    fclose(arquivo);
}

// FUNÇÃOPARA EXCLUIR DESPESA DO ARQUIVO POR ID NUMÉRICO
void excluirDespesa()
{
    char id[10];
    char id_nome[15] = "ID: ";
    printf("Qual o id da despesa que voce deseja excluir? ");
    scanf("%s", id);
    strcat(id_nome, id);

    // ABRINDO O ARQUIVO ORIGINAL
    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // ABRINDO O ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    char linha[200];
    int excluirProximas = 0;

    // PULANDO AS 4 LINHAS DE INFORMAÇÕES DA DESPESA, E ESCREVENDO O RESTO DO ARQUIVO ORIGINAL, NO TEMPORÁRIO
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

    // EXCLUINDO O ARQUIVO ORIGINAL, E RENOMEANDO O TEMPORÁRIO COM O NOME DO ORIGINAL EXCLUIDO
    fclose(arquivo);
    fclose(temp);
    remove("despesas.txt");
    rename("temp.txt", "despesas.txt");
}

// FUNÇÃO PARA MODIFICAR INFORMAÇÕES DE DESPESA INDICANDO-A O ID DA DESPESA
void atualizarDespesa()
{

    // PEDINDO O ID PARA MODIFICAÇÃO DA DESPESA REFERENTE AO MESMO, E CONCATENAÇÃO DE TEXTO PARA PADRONIZAÇÃO DE LINHAS
    char id[10];
    char id_nome[15] = "ID: ";
    printf("Qual o id da despesa que voce deseja modificar? ");
    scanf("%9s", id);
    strcat(id_nome, id);
    getchar();

    // ABRINDO ARQUIVO ORIGINAL
    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // ABRINDO ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    char linha[200];    // armazenar a linha do arquivo
    int encontrado = 0; // verificação para saber se a linha com id indicado foi encontrada
    int pular = 3;      // números de linha a se pular, referentes as informações anteriores que agora serão sobrepostas

    while (fgets(linha, sizeof(linha), arquivo))
    {

        // VERIFICAÇÃO PARA SABER SE A LINHA ATUAL CONTÉM O ID DA DESPESA QUE A GENTE DESEJA MODIFICAR
        if (strstr(linha, id_nome))
        {
            encontrado = 1;
            fputs(linha, temp);

            char data[100];
            char descricao[101];
            char valor[101];

            // coletando as novas informações do item
            printf("Informe a data da despesa: ");
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = '\0';

            printf("Informe a descricao da despesa: ");
            fgets(descricao, sizeof(descricao), stdin);
            descricao[strcspn(descricao, "\n")] = '\0';

            printf("Informe o valor da despesa: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = '\0';

            // ESCREVENDO AS NOVAS INFORMAÇÕES NAS LINHAS POSTERIORES AO ID
            fprintf(temp, " Data: %s\n Descrição: %s\n Valor: %s\n", data, descricao, valor);

            // IGNORANDO AS LINHAS DO ARQUIVO ORIGINAL COM AS INFORMAÇÕES ANTIGAS
            if (pular > 0)
            {
                for (int i = 0; i < pular; ++i)
                {
                    if (fgets(linha, sizeof(linha), arquivo) == NULL)
                    {
                    }
                }
                pular = 0;
            }
        }
        else
        {
            fputs(linha, temp); // escrevendo o restante das linhas que não nos interessam tratar
        }
    }
    fclose(arquivo);
    fclose(temp);

    // VERIFICAÇÃO E TRATAMENTO DE ERRO
    if (encontrado)
    {
        if (remove("despesas.txt") != 0)
        {
            printf("Erro ao remover o arquivo original.\n");
            exit(1);
        }

        if (rename("temp.txt", "despesas.txt") != 0)
        {
            printf("Erro ao renomear o arquivo temporario.\n");
            exit(1);
        }
    }
    else
    {
        printf("Despesa com ID %s nao encontrada.\n", id);
        remove("temp.txt"); // Remove o arquivo temporário, pois a modificação não foi bem-sucedida
    }
}

// ------------------------BLOCO DO CÓDIGO QUE LIDAA COM OS GANHOS ------------------------

// ESTRUTURA QUE DEFINE OS DADOS DE UM GANHO
typedef struct ganho
{
    int id;
    char *data;
    char descricao[100];
    float valor;
} Ganho;

// FUNÇÃO PARA ADICIONAR DEPESA NO ARQUIVO
void adicionarGanho()
{
    Ganho *novo = malloc(sizeof(Ganho));
    novo->data = malloc(100);

    // REGISTRO DAS INFORMAÇÕES DA NOVA GANHO
    printf("Informe o ID do ganho: ");
    scanf("%d", &novo->id);
    getchar();
    printf("Informe a data do ganho: ");
    fgets(novo->data, 100, stdin);
    novo->descricao[0] = '\0';
    printf("Informe a descricaoo do ganho: ");
    fgets(novo->descricao, 100, stdin);
    printf("Informe o valor do ganho: ");
    scanf("%f", &novo->valor);
    getchar();

    // ABRINDO O ARQUIVO TRATANDO ERRO, E ESCREVENDO NELE
    FILE *arquivo = fopen("ganhos.txt", "at");
    if (arquivo == NULL)
    {
        // Se o arquivo não existe, tenta criar um novo
        arquivo = fopen("ganhos.txt", "w");
        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo ganhos.\n");
            return;
        }
        fclose(arquivo);
        // Agora tenta abrir novamente para escrever
        arquivo = fopen("ganhos.txt", "a");
    }

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo ganhos.\n");
        return;
    }

    fprintf(arquivo, "ID: %d\n Data: %s Descrição: %s Valor: %.2f\n\n",
            novo->id,
            novo->data,
            novo->descricao,
            novo->valor);
    fclose(arquivo);
    printf("Ganho adicionada com sucesso.\n");
}

// FUNÇÃO PARA LISTAGEM DE GANHOS
void listarGanhos()
{
    //abrindo arquivo e tratando erro
    char *nomeArquivo = "ganhos.txt";
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    //imprimindo linha por linha do arquivo
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        printf(" %s", linha);
    }
    fclose(arquivo);
}

// FUNÇÃOPARA EXCLUIR ganho DO ARQUIVO POR ID NUMÉRICO
void excluirGanho()
{
    char id[10];
    char id_nome[15] = "ID: ";
    printf("Qual o id da ganho que você deseja excluir? ");
    scanf("%s", id);
    strcat(id_nome, id);

    // ABRINDO O ARQUIVO ORIGINAL
    FILE *arquivo = fopen("ganhos.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // ABRINDO O ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    char linha[200];
    int excluirProximas = 0;

    // PULANDO AS 4 LINHAS DE INFORMAÇÕES DO GANHO, E ESCREVENDO O RESTO DO ARQUIVO ORIGINAL, NO TEMPORÁRIO
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

    // EXCLUINDO O ARQUIVO ORIGINAL, E RENOMEANDO O TEMPORÁRIO COM O NOME DO ORIGINAL EXCLUIDO
    fclose(arquivo);
    fclose(temp);
    remove("ganhos.txt");
    rename("temp.txt", "ganhos.txt");
}

// FUNÇÃO PARA MODIFICAR INFORMAÇÕES DO GANHO INDICANDO-A O ID DO GANHO
void atualizarGanho()
{

    // PEDINDO O ID PARA MODIFICAÇÃO DO GANHO REFERENTE AO MESMO, E CONCATENAÇÃO DE TEXTO PARA PADRONIZAÇÃO DE LINHAS
    char id[10];
    char id_nome[15] = "ID: ";
    printf("Qual o id da ganho que voce deseja modificar? ");
    scanf("%9s", id);
    strcat(id_nome, id);
    getchar();

    // ABRINDO ARQUIVO ORIGINAL
    FILE *arquivo = fopen("ganhos.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // ABRINDO ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    char linha[200];    // armazenar a linha do arquivo
    int encontrado = 0; // verificação para saber se a linha com id indicado foi encontrada
    int pular = 3;      // números de linha a se pular, referentes as informações anteriores que agora serão sobrepostas

    while (fgets(linha, sizeof(linha), arquivo))
    {

        // VERIFICAÇÃO PARA SABER SE A LINHA ATUAL CONTÉM O ID DO GANHO QUE A GENTE DESEJA MODIFICAR
        if (strstr(linha, id_nome))
        {
            encontrado = 1;
            fputs(linha, temp);

            char data[100];
            char descricao[101];
            char valor[101];

            //coletando as novas informações do item
            printf("Informe a data do ganho: ");
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = '\0'; // Remova todos os caracteres de nova linha

            printf("Informe a descricao do ganho: ");
            fgets(descricao, sizeof(descricao), stdin);
            descricao[strcspn(descricao, "\n")] = '\0';

            printf("Informe o valor do ganho: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = '\0';

            // ESCREVENDO AS NOVAS INFORMAÇÕES NAS LINHAS POSTERIORES AO ID
            fprintf(temp, " Data: %s\n Descrição: %s\n Valor: %s\n", data, descricao, valor);

            // IGNORANDO AS LINHAS DO ARQUIVO ORIGINAL COM AS INFORMAÇÕES ANTIGAS
            if (pular > 0)
            {
                for (int i = 0; i < pular; ++i)
                {
                    if (fgets(linha, sizeof(linha), arquivo) == NULL)
                    {
                    }
                }
                pular = 0;
            }
        }
        else
        {
            fputs(linha, temp); // escrevendo o restante das linhas que não nos interessam tratar
        }
    }
    fclose(arquivo);
    fclose(temp);

    // VERIFICAÇÃO E TRATAMENTO DE ERRO
    if (encontrado)
    {
        if (remove("ganhos.txt") != 0)
        {
            printf("Erro ao remover o arquivo original.\n");
            return;
        }

        if (rename("temp.txt", "ganhos.txt") != 0)
        {
            printf("Erro ao renomear o arquivo temporário.\n");
            return;
        }
    }
    else
    {
        printf("Ganho com ID %s não encontrada.\n", id);
        remove("temp.txt"); // Remove o arquivo temporário, pois a modificação não foi bem-sucedida
    }
}

// ------------------------ BLOCO DO CÓDIGO QUE LIDAA COM O SALDO ------------------------

// FUNÇÃO PARA ADICIONAR SALDO - (POR SEGURANÇA O SALDO SÓ PODE SER MODIFICADO AO MODIFICAR AS DESPESAS OU OS GANHOS)
void adicionarSaldo()
{

    char valorstr[] = "valor";

    //abrindo arquivo de ganhos no modo leitura
    FILE *ganhos = fopen("ganhos.txt", "r");
    if (ganhos == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //abrindo arquivo de despesas no modo leitura
    FILE *despesas = fopen("despesas.txt", "r");
    if (despesas == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //abrindo arquivo de saldo no modo escrita
    FILE *saldo = fopen("saldo.txt", "w");
    if (saldo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    double soma_despesas = 0.0;
    double soma_ganhos = 0.0;
    double soma_saldo = 0.0;

    char linha[100];

    //armazenando o valor contido em cada linha do arquivo de despesas que contém a string "Valor:"
    while (fgets(linha, sizeof(linha), despesas) != NULL)
    {
        if (strstr(linha, "Valor: ") != NULL)
        {
            double valor;
            sscanf(linha, " Valor: %lf", &valor);
            soma_despesas += valor;
        }
    }

    //armazenando o valor contido em cada linha do arquivo de ganhos que contém a string "Valor:"
    while (fgets(linha, sizeof(linha), ganhos) != NULL)
    {
        if (strstr(linha, "Valor: ") != NULL)
        {
            double valor;
            sscanf(linha, " Valor: %lf", &valor);
            soma_ganhos += valor;
        }
    }

    soma_saldo = soma_ganhos - soma_despesas;

    fprintf(saldo, "O TOTAL DE SEUS GANHOS É: %.2f\n", soma_ganhos);
    fprintf(saldo, "O TOTAL DE SUAS DEPESAS É: %.2f\n", soma_despesas);
    fprintf(saldo, "SEU SALDO TOTAL É: %.2f\n", soma_saldo);

    fclose(despesas);
    fclose(ganhos);
    fclose(saldo);
}

// FUNÇÃO PARA EXIBIR SALDO TOTAL
void exibirSaldo()
{

    char *nomeArquivo = "saldo.txt";
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

// FUNÇÃO PRINCIPAL
int main()
{
    printf("\n");
    printf("   __||__\n");
    printf("  // ||  \\\\\n");
    printf(" ||  ||\n");
    printf(" ||  ||\n");
    printf("  \\\\ ||\n");
    printf("   \\\\||__\n");
    printf("     ||  \\\\ \n");
    printf("     ||   \\\\ \n");
    printf("     ||   ||\n");
    printf("     ||   ||\n");
    printf(" \\\\__||__//");
    printf(" I S T E M A   D E   G E S T A O   F I N A N C E I R A\n");
    printf("     ||\n");
    printf(" \n");

    printf(" Como podemos te ajudar hoje? \n");
    while (1)
    {

        // HUB DE ESCOLHA
        printf("\n");
        printf(" +----------------------------------------------------+\n");
        printf(" | 1. Adicionar Despesa                               |\n");
        printf(" | 2. Listar Despesas                                 |\n");
        printf(" | 3. excluir Despesa                                 |\n");
        printf(" | 4. modificar Despesa                               |\n");
        printf(" | 5. Adicionar Ganho                                 |\n");
        printf(" | 6. Listar Ganhos                                   |\n");
        printf(" | 7. Excluir Ganho                                   |\n");
        printf(" | 8. Modificar Ganho                                 |\n");
        printf(" | 9. Exibir saldo                                    |\n");
        printf(" | 10. Sair                                           |\n");
        printf(" +----------------------------------------------------+\n");
        printf(" Escolha uma opcao: ");

        int opcao;
        scanf("%d", &opcao);

        getchar();

        // switch e case de opções, com clear de terminal em cada operação
        switch (opcao)
        {
        case 1:
            system("clear");
            adicionarDespesa();
            adicionarSaldo();
            break;
        case 2:
            system("clear");
            listarDespesas();
            adicionarSaldo();
            break;
        case 3:
            system("clear");
            excluirDespesa();
            adicionarSaldo();
            break;
        case 4:
            system("clear");
            atualizarDespesa();
            adicionarSaldo();
            break;
        case 5:
            system("clear");
            adicionarGanho();
            adicionarSaldo();
            break;
        case 6:
            system("clear");
            listarGanhos();
            adicionarSaldo();
            break;
        case 7:
            system("clear");
            excluirGanho();
            adicionarSaldo();
            break;
        case 8:
            system("clear");
            atualizarGanho();
            adicionarSaldo();
            break;
        case 9:
            system("clear");
            exibirSaldo();
            break;
        case 10:
            system("clear");
            printf("Saindo do programa.\n");
            adicionar_saldo();
            exit(0);
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}