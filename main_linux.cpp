//--------------------------------------
/**INFORMAÇÕES BÁSICAS**/
//--------------------------------------
/*
Nome do projeto: Drive-thru Mickey & Donald

Descrição:
Projeto para avaliação de desempenho na discipĺina de "Lógica de programação 1"
do curso "Tecnólogo de Análise e Desenvolvimento de Sistemas" no Instituto Federal
de São Paulo - campus São Paulo

Desenvolvedores:
Felippe Marques da Silva de Almeida SP3056686
Leandro Paiva Higa                  SP3052648
*/

//--------------------------------------
/**ORGANIZAÇÃO DE DADOS**/
//--------------------------------------

/// DEFINIÇÕES:
/*
> lançamento: registro de uma escolha de um produto e sua quantidade
              pelo cliente.
> pedido: conjunto de lançamentos realizados pelo cliente.
> menu: lista de produtos disponíveis para o cliente.
*/

/// DADOS:
/*
Há 3 tabelas onde são gravados/lidos dados:
-- menu_produtos: onde estão indicados os produtos disponíveis.
-- lancamentos_pedidos: onde estão gravados todos os lancamentos
  já confirmados para preparação.
-- pagamentos_pedidos: onde estão gravados todos os pagamentos de
  pedidos realizados.

Os dados são acompanhados por '/t' quando há algum dado na linha e
por '\n' quando a linha termina.

Os dados do tipo 'string' devem conter '_' em vez de ' '.
*/

//--------------------------------------
/**BIBLIOTECAS UTILIZADAS**/
//--------------------------------------

#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
//#include <conio.h> // WINDOWS - para utlizar 'getch'
#include <termios.h> // LINUX - para utilizar 'getch'
#include <unistd.h> // LINUX - para utilizar 'getch'

//--------------------------------------
/**CONSTANTES DO PROGRAMA**/
//--------------------------------------

const int t_NomeProd = 15;
const int t_N_Cartao = 17;
const int t_NomeCliente = 21;

//t = tamanho da string

const int max_menu = 100;
const int max_lancamentos = 100;
const int min_Qtd = 1;
const int max_Qtd = 100;

//max = máximo
//min = mínimo

const char menu_dat[] = {"MENU.DAT"};
const char lancamentos_dat[] = {"LANCAMENTOS.DAT"};
const char pagamentos_dat[] = {"PAGAMENTOS.DAT"};
const char N_pedido_dat[] = {"N_PEDIDO.DAT"};

//--------------------------------------
/**ESTRUTURAS UTILIZADAS**/
//--------------------------------------

struct produto // estrutura para formar a lista de produtos disponíveis 'menu'
{
    int CodProd; // código do produto (de 10 a 99)
    char NomeProd[t_NomeProd]; // nome do produto (máx caracteres: 14 / 15 = '\0') não pode haver espaços
    float CustoProd; // custo do produto (de 0.01 a 100.00)
};

struct lancamento // estrutura para formar os lançamentos do pedido 'l_pedido'
{
    int N_pedido; // nº do pedido (de 1 a 100000)
    int CodProd; // código do produto (de 10 a 99)
    char NomeProd[t_NomeProd]; // nome do produto (máx caracteres: 14 / 15 = '\0') não pode haver espaços
    float CustoProd; // custo do produto (de 0.01 a 100.00)
    int Qtd; // quantidade do produto (de 1 a 100)
    float SubTotal; // CustoProd * Qtd (de 0.01 a 10,000.00)
};

struct pagamento // estrutura para formar o pagamento do pedido 'p_pedido'
{
    int N_pedido; // nº do pedido (de 1 a 100000)
    float Total; // soma do SubTotal dos lançamentos (de 0.01 a 100,000.00)
    char FormaPgto; // forma de pagamento ('1'=dinheiro '2'=cheque '3'=débito '4'=crédito)
    char N_cartao[t_N_Cartao]; // nº do cartão (máx caracteres: 16 / 17 = '\0') para dinheiro e cheque '000...' não pode haver espaços
    char NomeCliente[t_NomeCliente]; // nome do cliente para identificação na entrega (máx caracteres: 20 / 21 = '\0') não pode haver espaços
};

//--------------------------------------
/**FORMAÇÃO DAS VARIÁVEIS GLOBAIS**/
//--------------------------------------

struct produto menu[max_menu] = {}; // variável de estrutura que guardará os produtos disponíveis [limite máx. 100]

struct lancamento l_pedido[max_lancamentos] = {}; // variável de estrutura que guardará os lançamentos registrados no pedido [limite máx. 100]

struct pagamento p_pedido = {}; // variável de estrutura que guardará o pagamento do pedido realizado

int indice_l = 0; // variável inteira para guardará a próxima linha de 'l_pedido' a ser escrita

int n_pedido_atual; // variável que guardará o nº do pedido que está sendo montado

//l = lançamentos
//p = pagamento

//--------------------------------------
/**FUNÇÃO PRINCIPAL**/
//--------------------------------------

int main (void)
{
    // funções utilizadas
    void download_menu(void);
    void novo_n_pedido(void);
    void tela_inicial(void);

    download_menu(); // carrega na variável 'menu' a lista de produtos disponíveis

    novo_n_pedido(); // gera um novo nº de pedido

    tela_inicial(); // inicia o sistema de pedidos

    return 0;
}

//--------------------------------------
/**MÓDULO VENDAS**/
//--------------------------------------

void tela_inicial (void)
/*Objetivo da função
Apresentar as opções de manipulação de lançamentos do pedido e confirmação de pedido para o cliente*/
{
    // funções utilizadas
    void mostrar_pedido(void);
    int getch(void); // LINUX
    void cancelar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    char opcao; // criação da variável que guardará a opção digitada pelo cliente

    do
    {
        system("clear || cls"); // limpa a tela para sensação de uma nova página

        printf("\n>>>TELA INICIAL\n"); // título da página

        mostrar_pedido(); // apresentará na tela todas as informações do pedido até o momento

        /*INTERAÇÃO COM O USUÁRIO*/
        // apresentará as opções do usuário na tela
        printf("\nOpcoes:\n");
        printf("(0) - Cancelar pedido\n");
        printf("(1) - Registrar lancamentos\n");
        printf("(2) - Remover lancamentos\n");
        printf("(3) - Confirmar pedido\n");

        opcao = getch(); // guarda o caractere digitado pelo usuário

        // direcionamento do usuário de acordo com a opção escolhida do usuário
        switch(opcao)
        {
            case '0': // cancelar pedido
                cancelar_pedido(); // direciona o usuário para a seção de cancelamento do pedido
                break;
            case '1': // registrar lançamentos
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nEntrando em REGISTRAR lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                reg_lancamento(); // direciona o usuário para a seção de registro de lançamentos no pedido
                break;
            case '2': // remover lançamentos
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nEntrando em REMOVER lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                rm_lancamento(); // direciona o usuário para a seção de remoção de lançamentos no pedido
                break;
            case '3': // confirmar pedido
                confirmar_pedido(); // direciona o usuário para a seção de confirmação do pedido
                break;
            case 27: // fechar o sistema (informação oculta para o cliente)
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nFechando sistema...\n");
                break;
                /*Como a função 'tela_inicial' foi iniciada em 'main' ao terminar esse ciclo a execução do
                programa volta para a função 'main' e finaliza a execução pois a função 'tela_inicial' era
                o último comando do programa.*/
            default: // caso o usuário digite uma opção diferente
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3' && opcao != 27);
    // enquanto o usuário não digitar uma opção válida ele ficará realizando o ciclo da aprensentação da página da 'tela_inicial'
}

//--------------------------------------
/**SUB-MÓDULOS DE VENDAS**/
//--------------------------------------

void cancelar_pedido (void)
/*Objetivo da função
Realizar o cancelamento do pedido montado*/
{
    // funções utilizadas
    int getch(void);
    void limpar_l_pedido(void);
    void limpar_p_pedido(void);
    void novo_n_pedido(void);
    void tela_inicial(void);

    char conf_cancel; // variável que guardará a decisão de confirmação do cancelamento do pedido

    if (indice_l == 0) // caso não haja lançamentos no pedido
    {
        system("clear || cls"); // limpa a tela para uma sensação de pop-up
        printf("\nNao ha lancamentos. Portanto, nao e possivel CANCELAR o pedido.\n");
    }
    else // caso haja lançamentos no pedido
    {
        do
        {
            system("clear || cls"); // limpa a tela para uma sensação de pop-up

            /*INTERAÇÃO COM O USUÁRIO*/
            // apresentará as opções do usuário na tela
            printf("\n>>>ATENCAO!!!\n");
            printf("\nTem certeza que deseja CANCELAR o pedido?\n");
            printf("(0) - nao\n");
            printf("(1) - sim\n");

            conf_cancel = getch(); // guarda o caractere digitado pelo usuário

            // direcionamento do usuário de acordo com a opção escolhida do usuário
            switch (conf_cancel)
            {
                case '0': // não cancelar pedido
                    system("clear || cls"); // limpa a tela para uma sensação de pop-up
                    printf("\nPedido NAO CANCELADO.\n");
                    break;
                case '1': // cancelar pedido
                    limpar_l_pedido();
                    limpar_p_pedido();
                    novo_n_pedido ();
                    system("clear || cls"); // limpa a tela para uma sensação de pop-up
                    printf("\nPedido CANCELADO.\n");
                    break;
                default:
                    break;

            }
        } while (conf_cancel != '0' && conf_cancel != '1');
        // enquanto o usuário não digitar uma opção válida ele ficará realizando o ciclo da aprensentação do pop-up de confirmação do cancelamento do pedido
    }

    printf("\nPressione qualquer tecla para voltar...");
    getch(); // pausa para leitura da mensagem pelo usuário
    tela_inicial(); // direciona o usuário para a tela inicial
}

void reg_lancamento (void)
/*Objetivo da função
Realizar o registro de lançamentos no pedido*/
{
    // funções utilizadas
    void mostrar_menu(void);
    void mostrar_pedido(void);
    void limpa_buffer(void);
    void limpar_lancamento(int indice_lancamento, bool subir);
    int getch(void);
    void tela_inicial(void);
    int busca_CodProd(int CodProd);
    void mostrar_lancamento(int indice_lancamento);
    void calcula_total(void);

    int indice_CodProd; // criação da variável que guardará o índice do CodProd que está sendo registrado
    char conf_lancamento; // variável que guardará a decisão de confirmação do registro do lançamento no pedido

    system("clear || cls"); // limpa a tela para sensação de uma nova página

    printf("\n>>>REGISTRO DE LANCAMENTOS\n"); // título da página

    mostrar_menu(); // apresentar na tela os produtos disponíveis com o CodProd ao lado

    mostrar_pedido(); // apresentar na tela todas as informações do pedido até o momento

    /*INTERAÇÃO COM O USUÁRIO*/
    printf("\nInsira o codigo do produto desejado (-1 para voltar): ");
    scanf("%i", &l_pedido[indice_l].CodProd); // grava o valor digitado pelo usuário na variável CodProd
    limpa_buffer(); // limpa o ENTER digitado

    if (l_pedido[indice_l].CodProd < 0) // caso o usuário tenha digitado -1 para voltar a tela inicial
    {
        limpar_lancamento(indice_l, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
        system("clear || cls"); // limpa a tela para uma sensação de pop-up
        printf("\nVoltando para a TELA INICIAL...\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch(); // pausa para leitura da mensagem pelo usuário
        tela_inicial(); // direciona o usuário para a tela inicial
    }
    else // caso o usuário tenha digitado algum valor de CodProd
    {
        indice_CodProd = busca_CodProd(l_pedido[indice_l].CodProd); // realiza uma busca do índice que o CodProd digitado se encontra no 'menu'

        if (indice_CodProd == -1) // caso o CodProd digitado não tenha sido encontrado no 'menu'
        {
            limpar_lancamento(indice_l, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
            system("clear || cls"); // limpa a tela para uma sensação de pop-up
            printf("\nCodigo de produto digitado INVALIDO.\n");
            printf("\nPor favor, digite um codigo valido.\n");
            getch(); // pausa para leitura da mensagem pelo usuário
            reg_lancamento(); // retorna ao início do registro de lançamento
        }
        else // caso o CodProd digitado tenha sido encontrado no 'menu'
        {
            l_pedido[indice_l].N_pedido = n_pedido_atual; // guarda o nº do pedido na linha do lançamento

            for (int i = 0; i < t_NomeProd; ++i) // passa por todos os caracteres do NomeProd
                l_pedido[indice_l].NomeProd[i] = menu[indice_CodProd].NomeProd[i]; // guarda cada caractere do NomeProd do item escolhido para o NomeProd do lançamento
            l_pedido[indice_l].NomeProd[t_NomeProd] = '\0'; // torna o último caractere de NomeProd '\0' (vazio)

            l_pedido[indice_l].CustoProd = menu[indice_CodProd].CustoProd; // guarda o CustoProd do item escolhido em CustoProd do lançamento

            /*INTERAÇÃO COM O USUÁRIO*/
            printf("\nInsira a quantidade desejada (-1 para voltar): ");
            scanf("%i", &l_pedido[indice_l].Qtd); // grava o valor digitado pelo usuário na variável Qtd
            limpa_buffer(); // limpa o ENTER digitado

            if (l_pedido[indice_l].Qtd < 0) // caso o usuário tenha digitado -1 para voltar ao registro de lançamento
            {
                limpar_lancamento(indice_l, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                system("clear || cls"); // pausa para leitura da mensagem pelo usuário
                reg_lancamento(); // retorna ao início do registro de lançamento
            }
            else if (l_pedido[indice_l].Qtd < min_Qtd || l_pedido[indice_l].Qtd > max_Qtd) // caso o usuário tenha digitado um valor inválido de Qtd
            {
                limpar_lancamento(indice_l, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nLimite minimo por lancamento  = %i.", min_Qtd);
                printf("\nLimite maximo por lancamento  = %i.\n", max_Qtd);
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                reg_lancamento(); // retorna ao início do registro de lançamento
            }
            else // caso o usuário tenha digitado um valor válido para Qtd
            {
                l_pedido[indice_l].SubTotal = l_pedido[indice_l].CustoProd * l_pedido[indice_l].Qtd; // guarda o SubTotal do lançamento

                do
                {
                    system("clear || cls"); // limpa a tela para uma sensação de pop-up
                    mostrar_lancamento(indice_l); // apresenta o lançamento que o cliente acabou de escrever

                    /*INTERAÇÃO COM O USUÁRIO*/
                    // apresentará as opções do usuário na tela
                    printf("\nConfirmar lancamento?\n");
                    printf("0 - nao\n");
                    printf("1 - sim\n");

                    conf_lancamento = getch(); // guarda o caractere digitado pelo usuário
                } while (conf_lancamento != '0' && conf_lancamento != '1');
                // enquanto o usuário não digitar uma opção válida ele ficará realizando o ciclo de apresentação pop-up de confirmação do lançamento

                // direcionamento do usuário de acordo com a opção escolhida do usuário
                switch(conf_lancamento)
                {
                    case '0': // lançamento cancelado
                        limpar_lancamento(indice_l, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                        system("clear || cls"); // limpa a tela para uma sensação de pop-up
                        printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); // pausa para leitura da mensagem pelo usuário
                        reg_lancamento(); // retorna ao início do registro de lançamento
                        break;
                    case '1': // lançamento confirmado
                        ++indice_l; // vai para o próximo índice de um lançamento novo
                        calcula_total(); // calcula o novo Total do pedido
                        system("clear || cls"); // limpa a tela para uma sensação de pop-up
                        printf("\nLancamento REGISTRADO!\n");
                        printf("\nPressione qualquer tecla para continuar...");
                        getch(); // pausa para leitura da mensagem pelo usuário
                        reg_lancamento(); // retorna ao início do registro de lançamento
                        break;
                }
            }
        }
    }
}

void rm_lancamento (void) // em construcao
/*Objetivo da função*/
{
    printf("\nVoce escolheu REMOVER lançamento.\n");
    /*
    Este seria o sub-módulo 'rm_lancamento' que tem no mapa lógico.
    Creio que deva ser montado com base nele.
    */
}

void confirmar_pedido (void) // revisar
/*Objetivo da função*/
{
    int getch(void); // LINUX
    void tela_inicial(void);
    void cobranca(void);

    system("clear || cls");

    if(indice_l == 0)
    {
        printf("\nNao ha itens lancados no pedido.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        tela_inicial();
    }
    else
    {
        printf("Entrando em FORMAS DE PAGAMENTO...\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        system("clear || cls");
        cobranca();
    }
}

//--------------------------------------
/**MÓDULO COBRANÇA**/
//--------------------------------------

void cobranca (void) // revisar
{
    void mostrar_pedido(void);
    int getch(void); //LINUX
    void tela_inicial(void);
    void pgto_dinheiro(void);
    void pgto_cheque(void);
    void pgto_debito(void);
    void pgto_credito(void);

    char opcao;

    do
    {
        printf("\n>>>FORMA DE PAGAMENTO\n");

        mostrar_pedido();

        printf("\nOpcoes:\n");
        printf("(0) - Lancar itens/Cancelar pedido\n");
        printf("(1) - Dinheiro\n");
        printf("(2) - Cheque\n");
        printf("(3) - Cartao de Debito\n");
        printf("(4) - Cartao de Credito\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando para a TELA INICIAL...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                tela_inicial();
                break;
            case '1':
                system("clear || cls");
                printf("\nEntrando em pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                pgto_dinheiro();
                break;
            case '2':
                system("clear || cls");
                printf("\nEntrando em pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                pgto_cheque();
                break;
            case '3':
                system("clear || cls");
                printf("\nEntrando em pagamento em DEBITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                pgto_debito();
                break;
            case '4':
                system("clear || cls");
                printf("\nEntrando em pagamento em CREDITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                pgto_credito();
                break;
            default:
                system("clear || cls");
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3' && opcao != '4');

}

//--------------------------------------
/**SUB-MÓDULOS DE COBRANÇA**/
//--------------------------------------

void pgto_dinheiro (void) // revisar
{
    void limpa_buffer(void);
    int getch(void);
    void cobranca(void);
    void get_string (char linha[]);
    void upload_pedido(void);
    void tela_inicial(void);
    void finalizar_pedido(void);

    float valor_recebido, troco;
    char conf_pedido;

    printf("\n>>>PAGAMENTO EM DINHEIRO\n");
    printf("\nValor TOTAL da compra: %.2f\n", p_pedido.Total);
    printf("\nInsira o dinheiro (digite -1 para voltar): ");
    scanf("%f", &valor_recebido);
    limpa_buffer();

    if (valor_recebido < 0)
    {
        system("clear || cls");
        valor_recebido = '\0';
        printf("Voltando para FORMAS DE PAGAMENTO...\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        cobranca ();
    }
    else if (valor_recebido < p_pedido.Total)
    {
        system("clear || cls");
        printf("\nValor insuficiente.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        pgto_dinheiro();
    }
    else
    {
        troco = valor_recebido - p_pedido.Total;

        do
        {
            system("clear || cls");
            printf("\n>>>PAGAMENTO EM DINHEIRO\n");
            printf("\nValor entregue: %.2f\n", valor_recebido);
            printf("\nValor do troco a ser recebido: %.2f\n", troco);
            printf("\nConfirmar? (0 - voltar / 1 - sim) ");
            scanf("%c", &conf_pedido);
            limpa_buffer();
        } while (conf_pedido != '0' && conf_pedido != '1');

        switch(conf_pedido)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando para pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                troco = '\0';
                valor_recebido = '\0';
                conf_pedido = '\0';
                pgto_dinheiro();
                break;
            case '1':
                system("clear || cls");
                p_pedido.N_pedido = n_pedido_atual;
                p_pedido.FormaPgto = '1';
                for (int i = 0; i < 16; ++i)
                    p_pedido.N_cartao[i] = '0';
                p_pedido.N_cartao[16] = '\0';
                finalizar_pedido();
                break;
        }
    }

}

void pgto_cheque (void) // revisar
{
    void limpa_buffer(void);
    int getch(void);
    void cobranca(void);
    void get_string (char linha[]);
    void upload_pedido(void);
    void tela_inicial(void);
    void finalizar_pedido(void);

    float valor_recebido;
    char conf_pedido;

    system("clear || cls");
    printf("\n>>>PAGAMENTO EM CHEQUE\n");
    printf("\nValor TOTAL da compra: %.2f\n", p_pedido.Total);
    printf("\nInsira o dinheiro (digite -1 para voltar): ");
    scanf("%f", &valor_recebido);
    limpa_buffer();

    if (valor_recebido < 0)
    {
        system("clear || cls");
        valor_recebido = '\0';
        printf("Voltando para FORMAS DE PAGAMENTO...\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        cobranca ();
    }
    else if (valor_recebido != p_pedido.Total)
    {
        system("clear || cls");
        printf("\nValor nao correspondente.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        pgto_cheque();
    }
    else
    {

        do
        {
            printf("\nConfirmar? (0 - voltar / 1 - sim) ");
            scanf("%c", &conf_pedido);
            limpa_buffer();
        } while (conf_pedido != '0' && conf_pedido != '1');

        switch(conf_pedido)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                conf_pedido = '\0';
                pgto_cheque();
                break;
            case '1':
                system("clear || cls");
                p_pedido.N_pedido = n_pedido_atual;
                p_pedido.FormaPgto = '2';
                for (int i = 0; i < 16; ++i)
                    p_pedido.N_cartao[i] = '0';
                p_pedido.N_cartao[16] = '\0';
                finalizar_pedido();
                break;
        }
    }
}

void pgto_debito (void) // falta fazer
{
    printf("\nVocê escolheu DEBITO.\n");
}

void pgto_credito (void) // falta fazer
{
    printf("\nVocê escolheu CREDITO.\n");
}

//--------------------------------------
/**FUNÇÕES AUXILIARES**/
//--------------------------------------

///Funções de limpeza

void limpar_menu (void)
/*Objetivo da função
Limpar qualquer "sujeira" da variável 'menu'*/
{
    for (int i = 0; i < max_menu; ++i) // passa por todas as linhas da variável 'menu'
    {
        menu[i].CodProd = '\0'; // preenche '\0' (vazio) em CodProd
        for (int ii = 0; ii < t_NomeProd; ++ii) // passa por todos os caracteres de 'NomeProd'
            menu[i].NomeProd[ii] = '\0'; // preenche '\0' (vazio) em todos os caracteres de 'NomeProd'
        menu[i].CustoProd = '\0'; // preenche '\0' (vazio) em CustoProd
    }
}

void limpar_l_pedido (void)
/*Objetivo da função
Limpar todos os lançamentos de 'l_pedido'*/
{
    void limpar_lancamento(int indice_lancamento, bool subir);

    for (int i = 0; i < indice_l; ++i)
        limpar_lancamento(i, false);

    indice_l = 0;
}

void limpar_p_pedido (void)
/*Objetivo da função
Limpar 'p_pedido'*/
{
    p_pedido.N_pedido = '\0'; // preenche '\0' (vazio) em N_pedido

    p_pedido.Total = '\0'; // preenche '\0' (vazio) em Total

    p_pedido.FormaPgto = '\0'; // preenche '\0' (vazio) em FormaPgto

    for (int i = 0; i < t_N_Cartao; ++i) // passa por todos os caracteres de N_Cartao
        p_pedido.N_cartao[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de N_Cartao

    for (int i = 0; i < t_NomeCliente; ++i) // passa por todos os caracteres de NomeCliente
        p_pedido.NomeCliente[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de NomeCliente
}

void limpar_lancamento (int indice_lancamento, bool subir)
/*Objetivo da função
- Limpar um lançamento específico do pedido;
- Chama a função 'subir_lancamento' se o segundo argumento for 'true'
  (usado quando se quer limpar um lançamento específico apenas;*/
{
    // funções utilizadas
    void subir_lancamento(int indice_ref);
    void calcula_total(void);

    l_pedido[indice_lancamento].N_pedido = '\0'; // preenche '\0' (vazio) em N_pedido

    l_pedido[indice_lancamento].CodProd = '\0'; // preenche '\0' (vazio) em CodProd

    for (int i = 0; i < t_NomeProd; ++i) // passa por todos os caracteres de NomeProd
        l_pedido[indice_lancamento].NomeProd[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de NomeProd

    l_pedido[indice_lancamento].CustoProd = '\0'; // preenche '\0' (vazio) em CustoProd

    l_pedido[indice_lancamento].Qtd = '\0'; // preenche '\0' (vazio) em Qtd

    l_pedido[indice_lancamento].SubTotal = '\0'; // preenche '\0' (vazio) em SubTotal

    if (subir) // caso se queira subir os lançamentos abaixo
    {
        while (l_pedido[indice_lancamento + 1].CodProd != '\0') // o ciclo roda até que a próxima linha seja '\0' (vazio)
        {
            subir_lancamento(indice_lancamento + 1); // sobe o próximo lancamento para a o 'indice_lancamento'
            ++indice_lancamento; // vai para o próximo índice de lançamento
        }

        indice_l = indice_lancamento; // atualiza o índice do próximo lançamento a realizar

        calcula_total(); // calcula o 'Total' do pedido atual
    }
}

void subir_lancamento (int indice_ref)
/*Objetivo da função
Copia os dados do índice referente para um índice anterior e apaga os dados do índice referente*/
{
    //funções utilizadas
    void limpar_lancamento(int indice_lancamento, bool subir);

    int indice_novo = indice_ref - 1; // cria uma variável que guardará o índice anterior (onde que ser gravar os dados do índice referente)

    l_pedido[indice_novo].N_pedido = l_pedido[indice_ref].N_pedido; // copia o N_pedido do índice referente para o índice anterior

    l_pedido[indice_novo].CodProd = l_pedido[indice_ref].CodProd; // copia o CodProd do índice referente para o índice anterior

    for (int i = 0; i < 15; ++i) // passa por todos os caracteres de NomeProd
        l_pedido[indice_novo].NomeProd[i] = l_pedido[indice_ref].NomeProd[i]; // copia os caracteres do NomeProd do índice referente para o índice anterior

    l_pedido[indice_novo].CustoProd = l_pedido[indice_ref].CustoProd; // copia o CustoProd do índice referente para o índice anterior

    l_pedido[indice_novo].Qtd = l_pedido[indice_ref].Qtd; // copia o Qtd do índice referente para o índice anterior

    l_pedido[indice_novo].SubTotal = l_pedido[indice_ref].SubTotal; // copia o SubTotal do índice referente para o índice anterior

    limpar_lancamento(indice_ref, false); // limpa os dados do índice referente que foram copiados para o índice anterior
}

///Funções de impressão

void mostrar_pedido (void)
/*Objetivo da função
Imprimir na tela as informações até o momento do pedido*/
{
    //funções utilizadas
    void imprime_n_pedido(void);
    void imprime_cabecalho_pedido(void);
    void imprime_lancamento(int indice_ref);
    void imprime_rodape_pedido(void);

    if(indice_l == 0) // se 'indice_l' for igual a zero quer dizer que ainda não houveram lançamentos no pedido
    {
        printf("\nPedido No %i", n_pedido_atual); // informações apresentadas caso não haja lançamentos no pedido
        printf("\nNao ha lancamentos no pedido.\n");
    }
    else // apresentação das informações do pedido caso haja lançamentos
    {
        printf("\n==================SEU PEDIDO=================="); // título do quadro

        imprime_n_pedido(); // imprime o nº do pedido

        imprime_cabecalho_pedido(); // imprime o cabeçalho de lançamentos

        for(int i = 0; i < indice_l; ++i) // ciclo que fará a impressão de lançamento por lançamento a partir da variável 'l_pedido'
            imprime_lancamento(i); // imprime um lançamento

        imprime_rodape_pedido(); // imprime o total do pedido

        printf("==============================================\n"); // rodapé do quadro
    }
}

void imprime_n_pedido (void)
{
    printf("\n                              Pedido No ");
    // dependendo do valor de 'n_pedido_atual' é necessário uma quantidade de espaços para alinhar ao quadro
    if (n_pedido_atual < 10)
        printf("     %i\n", n_pedido_atual);
    else if (n_pedido_atual < 100)
        printf("    %i\n" , n_pedido_atual);
    else if (n_pedido_atual < 1000)
        printf("   %i\n"  , n_pedido_atual);
    else if (n_pedido_atual < 10000)
        printf("  %i\n"   , n_pedido_atual);
    else if (n_pedido_atual < 100000)
        printf(" %i\n"    , n_pedido_atual);
    else
        printf("%i\n"     , n_pedido_atual); // valor máximo do 'n_pedido_atual' é de 100000
}

void imprime_cabecalho_pedido (void)
{
    printf("\nCodProd Nome do Produto   Custo  Qtd  SubTotal\n"); // cabeçalho da tabela
}

void imprime_lancamento (int indice_ref)
{
    int i = indice_ref; // cria uma variável que guardará o índica do lançamento que se deseja imprimir

    // CodProd
    printf("%i      ",   l_pedido[i].CodProd); // imprime CodProd (como tem sempre dois caracteres não é necessário quantidade de espações diferentes

    // Nome do Produto
    for (int ii = 0; ii < t_NomeProd; ++ii) // ciclo que passa por todos os caracteres de NomeProd
    {
        if (l_pedido[i].NomeProd[ii] != '\0') // se o caractere de NomeProd for diferente de vazio ele imprime na tela
            printf("%c", l_pedido[i].NomeProd[ii]);
        else // se for vazio ele imprime um espaço
            printf(" ");
    }
    printf(" "); // um espaço para separação da próxima informação

    // Custo
    // dependendo do valor de CustoProd é necessário uma quantidade de espaços para alinhar ao quadro
    if (l_pedido[i].CustoProd < 10)
        printf("   %.2f ", l_pedido[i].CustoProd);
    else if (l_pedido[i].CustoProd < 100)
        printf("  %.2f " , l_pedido[i].CustoProd);
    else
        printf(" %.2f "  , l_pedido[i].CustoProd); // valor máximo de CustoProd é de 100.00

    // Qtd
    // dependendo do valor de Qtd é necessário uma quantidade de espaços para alinhar ao quadro
    if (l_pedido[i].Qtd < 10)
        printf("   %i ", l_pedido[i].Qtd);
    else if (l_pedido[i].Qtd < 100)
        printf("  %i " , l_pedido[i].Qtd);
    else
        printf(" %i "  , l_pedido[i].Qtd); // valor máximo de Qtd é de 100

    // SubTotal
    // dependendo do valor de SubTotal é necessário uma quantidade de espaços para alinhar ao quadro
    if (l_pedido[i].SubTotal < 10)
        printf("     %.2f\n", l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 100)
        printf("    %.2f\n" , l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 1000)
        printf("   %.2f\n"  , l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 10000)
        printf("  %.2f\n"   , l_pedido[i].SubTotal);
    else
        printf(" %.2f\n"    , l_pedido[i].SubTotal); // valor máximo de SubTotal é de 10,000.00
}

void imprime_rodape_pedido (void)
{
    printf("\n                               Total ");
    // dependendo do valor de Total é necessário uma quantidade de espaços para alinhar ao quadro
    if (p_pedido.Total < 10)
        printf("     %.2f\n", p_pedido.Total);
    else if (p_pedido.Total < 100)
        printf("    %.2f\n" , p_pedido.Total);
    else if (p_pedido.Total < 1000)
        printf("   %.2f\n"  , p_pedido.Total);
    else if (p_pedido.Total < 10000)
        printf("  %.2f\n"   , p_pedido.Total);
    else if (p_pedido.Total < 100000)
        printf(" %.2f\n"    , p_pedido.Total);
    else
        printf("%.2f\n"     , p_pedido.Total); // valor máximo de Ttoal é de 100,000.00
}

void mostrar_menu (void)
/*Objetivo da função
Imprimir na tela os produtos disponíveis no 'menu'*/
{
    if (menu[0].CodProd == '\0') // se o primeiro CodProd for '\0' (vazio), então não há itens no 'menu'
        printf("\nNao ha itens no menu.\n");
    else // apresentação das informações do 'menu' caso haja produtos disponíveis
    {
        printf("\n============MENU============="); // título da quadro
        printf("\nCodigo Nome do Produto  Custo\n"); // cabeçalho da tabela
        for (int i = 0; menu[i].CodProd != '\0'; ++i) // ciclo que fará a impressão dos produtos disponíveis
        {
            // CodProd
            printf("%i     ", menu[i].CodProd); // imprime CodProd (como tem sempre dois caracteres não é necessário quantidade de espações diferentes

            // NomeProd
            for (int ii = 0; ii < t_NomeProd; ++ii) // ciclo que passa por todos os caracteres de NomeProd
            {
                if (menu[i].NomeProd[ii] != '\0') // se o caractere de NomeProd for diferente de vazio ele imprime na tela
                    printf("%c", menu[i].NomeProd[ii]);
                else // se for vazio ele imprime um espaço
                    printf(" ");
            }
            printf(" "); // um espaço para separação da próxima informação

            // Custo
            // dependendo do valor de CustoProd é necessário uma quantidade de espaços para alinhar ao quadro
            if (menu[i].CustoProd < 10)
                printf("  %.2f\n", menu[i].CustoProd);
            else if (menu[i].CustoProd < 100)
                printf(" %.2f\n" , menu[i].CustoProd);
            else
                printf("%.2f\n"  , menu[i].CustoProd); // valor máximo de CustoProd é de 100.00
        }
        printf("=============================\n"); // rodapé do quadro
    }
}

void mostrar_lancamento (int indice_lancamento)
/*Objetivo da função
Imprimir na tela um lançamento específico*/
{
    //funções utilizadas
    void imprime_cabecalho_pedido(void);
    void imprime_lancamento(int indice_ref);

    imprime_cabecalho_pedido(); // imprime o cabeçalho de lançamentos

    imprime_lancamento(indice_lancamento); // imprime o lançamento indicado
}

///Funções de iniciação ou gravação

void download_menu (void)
/*Objetivo da função
Carregar do arquivo "menu_produtos.DAT" quais são os produtos disponíveis e guardar na variável 'menu'*/
{
    // funções utilizadas
    void limpar_menu(void);

    limpar_menu(); // limpar qualquer "sujeira" na variável 'menu'

    FILE *pont_menu; // criação de uma variável ponteiro para o arquivo que contém os produtos disponíveis

    pont_menu = fopen(menu_dat, "r"); // leitura do arquivo onde se encontram os produtos disponíveis

    if (pont_menu == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        int i = 0; // índice da variável vetor 'menu'

        do
        {
            fscanf(pont_menu, "%i\t", &menu[i].CodProd); // grava o 'CodProd' que se encontra na linha (i + 1) do arquivo
            fscanf(pont_menu, "%s\t",  menu[i].NomeProd); // grava o 'NomeProd' que se encontra na linha (i + 1) do arquivo /não pode haver espaços na string
            fscanf(pont_menu, "%f\n", &menu[i].CustoProd); // grava o 'CustoProd' que se encontra na linha (i + 1) do arquivo
            ++i;
        } while (menu[i - 1].CodProd != '\0' && i < max_menu); // irá gravar em cada linha da variável 'menu' os dados do arquivo até que a última gravação seja '\0' (vazio)

        fclose(pont_menu); // fecha o arquivo
    }
}

void novo_n_pedido (void)
/*Objetivo da função
- Ler no arquivo qual foi o último nº do pedido aberto;
- Guardar o próximo nº da variável 'n_pedido_atual';
- Gravar no arquivo o 'n_pedido_atual'.*/
{
    FILE *pont_Npedido; // criação de uma variável ponteiro para o arquivo que contém o último nº de pedido aberto

    pont_Npedido = fopen(N_pedido_dat, "r"); // leitura do arquivo onde se encontra o último nº de pedido aberto

    if (pont_Npedido == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        fscanf(pont_Npedido, "%i", &n_pedido_atual); // grava o nº do último pedido aberto em 'n_pedido_atual'
        ++n_pedido_atual; // acrescenta +1 para se tornar o próximo nº de pedido sendo o atual

        fclose(pont_Npedido); // fecha o arquivo

        pont_Npedido = fopen(N_pedido_dat, "w"); // recriação do arquivo onde se encontrará o último nº de pedido aberto

        fprintf(pont_Npedido, "%i", n_pedido_atual); // grava o nº de pedido que acabou de abrir no arquivo para que este seja o último nº de pedido aberto

    }

    fclose(pont_Npedido); // fecha o arquivo

}

void upload_pedido (void)
/*Objetivo da função
Gravar nos arquivos "lancamentos_pedidos.DAT" e "pagamentos_pedidos.DAT os dados do pedido realizado"*/
{
    // funções utilizadas
    void limpar_l_pedido(void);
    void limpar_p_pedido(void);
    void novo_n_pedido(void);

    FILE *pont_lancamentos; // cria uma variável ponteiro para o arquivo que guarda os lançamentos confirmados
    FILE *pont_pagamentos; // cria uma variável ponteiro para o arquivo que guarda os pagamentos realizados

    pont_lancamentos = fopen(lancamentos_dat, "a"); // abre para anexar lançamentos confirmado do pedido realizado

    if (pont_lancamentos == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        for (int i = 0; i < indice_l; i++) // passar por todos os lançamentos do pedido
        {
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].N_pedido); // grava o nº do pedido
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].CodProd); // grava o código do produto
            fprintf(pont_lancamentos, "%s\t",   l_pedido[i].NomeProd); // grava o nome do produto
            fprintf(pont_lancamentos, "%.2f\t", l_pedido[i].CustoProd); // grava o custo unitário
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].Qtd); // grava a quantidade pedida
            fprintf(pont_lancamentos, "%.2f\n", l_pedido[i].SubTotal); // grava o sub-total do lançamento
        }
    }

    fclose(pont_lancamentos); // fecha o arquivo

    pont_pagamentos = fopen(pagamentos_dat, "a"); // abre para anexar pagamento realizado do pedido

    if (pont_pagamentos == NULL)
        printf("\nArquivo não pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        fprintf(pont_pagamentos, "%i\t",     p_pedido.N_pedido); // grava o nº do pedido
        fprintf(pont_pagamentos, "%.2f\t",   p_pedido.Total); // grava o total do pedido
        fprintf(pont_pagamentos, "%c\t",     p_pedido.FormaPgto); // grava a forma de pagamento
        fprintf(pont_pagamentos, "%s\t",     p_pedido.N_cartao); // grava o número do cartão
        fprintf(pont_pagamentos, "%s\n",     p_pedido.NomeCliente); // grava o nome do cliente
    }

    fclose(pont_pagamentos); // fecha o arquivo

    limpar_l_pedido(); // limpa todos os lancamento da variável 'l_pedido'
    limpar_p_pedido(); // limpa todos os dados de 'p_pedido'
    novo_n_pedido(); // gera um novo nº de pedido
}

///Funções diversas

int busca_CodProd (int CodProd)
/*Objetivo da função
Retorna o valor do índice de um CodProd específico, caso não encontre será retornado -1*/
{
    int indice_CodProd = -1; // variável que guardará a o índice do produto procurado, ele já é -1 para caso não seja encontrado o produto
    int i = 0; // índice que seja procurado, inicia em 0

    while (indice_CodProd == -1 && menu[i].CodProd != '\0') // ciclo que estará buscando o índice até que se encontre ou passe por todos os itens
    {
        if(CodProd != menu[i].CodProd) // caso o CodProd do 'menu' seja diferente do CodProd procurado
            ++i; // vai para o próximo índice
        else
            indice_CodProd = i; // copia o índice na variável que será retornada
    }

    return indice_CodProd; // retorna o índice do CodProd procurado ou -1 se não tiver sido encontrado
}

void calcula_total (void)
/*Objetivo da função
Calcular o Total de 'p_pedido' até o momento*/
{
    p_pedido.Total = 0; // zera o Total do pedido

    for (int i = 0; i < indice_l; ++i) // somará todos os SubTotal de 'l_pedido' em Total de todas as linhas preenchidas
        p_pedido.Total += l_pedido[i].SubTotal;
}

void finalizar_pedido (void) // revisar falta completar
{
    void get_string (char linha[]);
    void upload_pedido (void);
    void tela_inicial(void);
    int getch (void);

    printf("\n>>>FINALIZACAO DE PEDIDO\n");
    printf("\nDigite seu NOME: ");
    get_string (p_pedido.NomeCliente);
    upload_pedido();
    printf("\nPedido CONFIRMADO!\n");
    printf("\nPagamento REALIZADO!\n");
    printf("\nAGRADECEMOS a sua compra!");
    getch();
    system("clear || cls");
    tela_inicial();
}


//--------------------------------------
/**FUNÇÕES BÁSICAS**/
//--------------------------------------

int getch (void) // LINUX - criação da função 'getch' pois o sistema não tem a biblioteca 'conio.h'
/*Objetivo da função
Capturar o próximo caractere que o usuário digitar*/
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void limpa_buffer (void) // função criada para limpar o buffer do teclado que funciona em WINDOWS e LINUX
/*Objetivo da função
Limpar qualquer "sujeira" que tiver no buffer do teclado. Só funciona se realmente houver algo no buffer
Deve-se utilizar após a função 'scanf'*/
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_string (char linha[]) // função que funciona melhor que gets
/*Objetivo da função
Guarda em uma variável do tipo string (char com vetor) o que o usuário digitar até ele apertar ENTER*/
{
    char c; // caractere que irá compor a string
    int i = 0; // índice do caractere

    do {
        c = getchar (); // guarda o caractere digitado na variável 'c'
        linha [i] = c; // guarda o valor da variável 'c' na posição da string indicada
        ++i; // vai para a próxima posição da string
    } while (c != '\n'); // ciclo rodará enquanto a variável 'c' não for ENTER

    linha [i - 1] = '\0'; // substitui o ENTER por '\0' (vazio) para finalizar a string
}

//--------------------------------------
/**INFORMAÇÕES BÁSICAS**/
//--------------------------------------
/*
Nome do projeto: Drive-thru Mickey & Donald

Descrição:
Projeto para avaliação de desempenho na discipĺina de "Lógica de programação 1"
do curso "Tecnólogo de Análise e Desenvolvimento de Sistemas" no Instituto Federal
de São Paulo - campus São Paulo

Desenvolvedores:
Felippe Marques da Silva de Almeida SP3056686
Leandro Paiva Higa                  SP3052648
*/
