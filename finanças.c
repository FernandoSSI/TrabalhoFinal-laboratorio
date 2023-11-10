#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ------------------------BLOCO DO CÓDIGO QUE LIDAA COM AS DESPESAS ------------------------
// Definição da estrutura para despesas
typedef struct despesa{
    int id;
    char *data;
    char descricao[100];
    float valor;
} Despesa;

// FUNÇÃO PARA ADICIONAR DEPESA NO ARQUIVO
void adicionarDespesa(){
    Despesa *novo = malloc(sizeof(Despesa));
    novo->data = malloc(100); 

    // REGISTRO DAS INFORMAÇÕES DA NOVA DESPESA
    printf("Informe o ID da despesa: ");
    scanf("%d", &novo->id);
    getchar();
    printf("Informe a data da despesa: ");
    fgets(novo->data, 100, stdin);
    novo->descricao[0] = '\0';
    printf("Informe a descrição da despesa: ");
    fgets(novo->descricao, 100, stdin);
    printf("Informe o valor da despesa: ");
    scanf("%f", &novo->valor);
    getchar();

    // ABRINDO O ARQUIVO TRATANDO ERRO, E ESCREVENDO NELE
    FILE *arquivo = fopen("despesas.txt", "a");
    if (arquivo == NULL){
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

// FUNÇÃO PARA LISTAGEM DE DESPESAS
void listarDespesas(){
    char *nomeArquivo = "despesas.txt";
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) != NULL){
        printf(" %s", linha);
    }
    fclose(arquivo);
}

// FUNÇÃOPARA EXCLUIR DESPESA DO ARQUIVO POR ID NUMÉRICO
void excluirDespesa(){
    char id[10];
    char id_nome[] = "ID: ";
    printf("Qual o id da despesa que você deseja excluir? ");
    scanf("%s", id);
    strcat(id_nome, id);

    // ABRINDO O ARQUIVO ORIGINAL
    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // ABRINDO O ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL){
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    char linha[200];
    int excluirProximas = 0;

    // PULANDO AS 4 LINHAS DE INFORMAÇÕES DA DESPESA, E ESCREVENDO O RESTO DO ARQUIVO ORIGINAL, NO TEMPORÁRIO
    while (fgets(linha, sizeof(linha), arquivo)){
        if (excluirProximas > 0){
            excluirProximas--;
        } else{
            if (strstr(linha, id_nome)){
                excluirProximas = 4;
            } else {
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
void atualizarDespesa() {

    // PEDINDO O ID PARA MODIFICAÇÃO DA DESPESA REFERENTE AO MESMO, E CONCATENAÇÃO DE TEXTO PARA PADRONIZAÇÃO DE LINHAS
    char id[10];
    char id_nome[] = "ID: ";
    printf("Qual o id da despesa que você deseja modificar? ");
    scanf("%9s", id); 
    strcat(id_nome, id);
    getchar(); 

    // ABRINDO ARQUIVO ORIGINAL
    FILE *arquivo = fopen("despesas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    //ABRINDO ARQUIVO TEMPORÁRIO
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL){
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }


    char linha[200];  // armazenar a linha do arquivo
    int encontrado = 0; // verificação para saber se a linha com id indicado foi encontrada
    int pular = 4;  // números de linha a se pular, referentes as informações anteriores que agora serão sobrepostas

    while (fgets(linha, sizeof(linha), arquivo)) {

        // VERIFICAÇÃO PARA SABER SE A LINHA ATUAL CONTÉM O ID DA DESPESA QUE A GENTE DESEJA MODIFICAR
        if (strstr(linha, id_nome)){
                encontrado = 1;
                fputs(linha, temp); 

                char data[100];
                char descricao[101];
                char valor[101];

                printf("Informe a data da despesa: ");
                fgets(data, sizeof(data), stdin);
                data[strlen(data) - 1] = '\0'; // Remova o caractere de nova linha
                getchar(); // Consumir o caractere de nova linha

                printf("Informe a descrição da despesa: ");
                fgets(descricao, sizeof(descricao), stdin);
                descricao[strlen(descricao) - 1] = '\0'; 
                getchar(); 
                printf("Informe o valor da despesa: ");
                fgets(valor, sizeof(valor), stdin);
                
                // ESCREVENDO AS NOVAS INFORMAÇÕES NAS LINHAS POSTERIORES AO ID
                fprintf(temp, " Data: %s\n Descrição: %s\n Valor: %s\n", data, descricao, valor);
            
                // IGNORANDO AS LINHAS DO ARQUIVO ORIGINAL COM AS INFORMAÇÕES ANTIGAS    
                if(pular > 0){
                    for (int i = 0; i < pular; ++i) {
                        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
                            
                        }
                    }
                    pular = 0;
                }
                
        } else {
            fputs(linha, temp); // escrevendo o restante das linhas que não nos interessam tratar
        }
    }
    fclose(arquivo);
    fclose(temp);

    // VERIFICAÇÃO E TRATAMENTO DE ERRO
    if (encontrado) {
        if (remove("despesas.txt") != 0) {
            printf("Erro ao remover o arquivo original.\n");
            exit(1);
        }

        if (rename("temp.txt", "despesas.txt") != 0) {
            printf("Erro ao renomear o arquivo temporário.\n");
            exit(1);
        }
    } else {
        printf("Despesa com ID %s não encontrada.\n", id);
        remove("temp.txt"); // Remove o arquivo temporário, pois a modificação não foi bem-sucedida
    }
}

// ---------------------------------------------------------------------------------------------------



// ------------------------BLOCO DO CÓDIGO QUE LIDAA COM OS GANHOS ------------------------



// ---------------------------------------------------------------------------------------------------
// Função principal
int main() {
    while (1) {

        // HUB DE ESCOLHA 
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
            excluirDespesa();
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