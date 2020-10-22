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
#include <string.h>

//--------------------------------------
/**CONSTANTES DO PROGRAMA**/
//--------------------------------------

const int tNomeProd    = 15;
const int tNCartao     = 17;
const int tNomeCliente = 21;

//t = tamanho da string

const int maxMenu        = 100;
const int maxLancamentos = 100;
const int minQtd         =   1;
const int maxQtd         = 100;

//max = máximo
//min = mínimo

const char MenuDat[]        = {"MENU.DAT"};
const char LancamentosDat[] = {"LANCAMENTOS.DAT"};
const char PagamentosDat[]  = {"PAGAMENTOS.DAT"};
const char NPedidoDat[]     = {"N_PEDIDO.DAT"};

//--------------------------------------
/**ESTRUTURAS UTILIZADAS**/
//--------------------------------------

struct Produto // estrutura para formar a lista de produtos disponíveis menu
{
    int   CodProd;   // código do produto (de 10 a 99)
    char  NomeProd[tNomeProd]; // nome do produto (máx caracteres: 14 / 15 = '\0') não pode haver espaços
    float CustoProd; // custo do produto (de 0.01 a 100.00)
};

struct Lancamento // estrutura para formar os lançamentos de um pedido
{
    int   NPedido;   // nº do pedido (de 1 a 100000)
    int   CodProd;   // código do produto (de 10 a 99)
    char  NomeProd[tNomeProd]; // nome do produto (máx caracteres: 14 / 15 = '\0') não pode haver espaços
    float CustoProd; // custo do produto (de 0.01 a 100.00)
    int   Qtd;       // quantidade do produto (de 1 a 100)
    float SubTotal;  // CustoProd * Qtd (de 0.01 a 10,000.00)
};

struct Pagamento // estrutura para formar o pagamento de um pedido
{
    int   NPedido;   // nº do pedido (de 1 a 100000)
    float Total;     // soma do SubTotal dos lançamentos (de 0.01 a 100,000.00)
    char  FormaPgto; // forma de pagamento ('1'=dinheiro '2'=cheque '3'=débito '4'=crédito)
    char  NCartao[tNCartao];         // nº do cartão (máx caracteres: 16 / 17 = '\0') para dinheiro e cheque '000...' não pode haver espaços
    char  NomeCliente[tNomeCliente]; // nome do cliente para identificação na entrega (máx caracteres: 20 / 21 = '\0') não pode haver espaços
};

//--------------------------------------
/**FORMAÇÃO DAS VARIÁVEIS GLOBAIS**/
//--------------------------------------

struct Produto Menu[maxMenu] = {}; // variável de estrutura que guardará os produtos disponíveis

struct Lancamento LancaPedido[maxLancamentos] = {}; // variável de estrutura que guardará os lançamentos registrados no pedido

struct Pagamento PgtoPedido = {}; // variável de estrutura que guardará o pagamento do pedido realizado

int    IndiceLancamento = 0; // variável inteira para guardará a próxima linha de 'LancaPedido' a ser escrita

int    NPedidoAtual; // variável que guardará o nº do pedido que está sendo montado

//--------------------------------------
/**FUNÇÃO PRINCIPAL**/
//--------------------------------------

int main (void)
{
    // funções utilizadas
    void DownloadMenu(void);
    void NovoNPedido(void);
    void TelaInicial(void);

    DownloadMenu(); // carrega na variável menu a lista de produtos disponíveis

    NovoNPedido();  // gera um novo nº de pedido

    TelaInicial();  // inicia o sistema de pedidos

    return 0;
}

//--------------------------------------
/**MÓDULO VENDAS**/
//--------------------------------------

void TelaInicial (void)
/*Objetivo da função
Apresentar as opções de manipulação de lançamentos do pedido e confirmação de pedido para o cliente*/
{
    // funções utilizadas
    void MostrarPedido(void);
    int  getch(void); // LINUX
    void CancelarPedido(void);
    void RegLancamento(void);
    void RmLancamento(void);
    void ConfirmarPedido(void);

    // variáveis locais
    char opcao; // criação da variável que guardará a opção digitada pelo cliente

    do
    {
        system("clear || cls"); // limpa a tela para sensação de uma nova página

        printf("\n>>>TELA INICIAL\n"); // título da página

        MostrarPedido(); // apresentará na tela todas as informações do pedido até o momento

        /*INTERAÇÃO COM O USUÁRIO*/
        // apresentará as opções do usuário na tela
        printf("\nOpcoes:\n");
        printf(  "(0) - Cancelar pedido\n");
        printf(  "(1) - Registrar lancamentos\n");
        printf(  "(2) - Remover lancamentos\n");
        printf(  "(3) - Confirmar pedido\n");

        opcao = getch(); // guarda o caractere digitado pelo usuário

        // direcionamento do usuário de acordo com a opção escolhida do usuário
        switch(opcao)
        {
            case '0': // cancelar pedido
                CancelarPedido(); // direciona o usuário para a seção de cancelamento do pedido
                break;
            case '1': // registrar lançamentos
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nEntrando em REGISTRAR lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                RegLancamento(); // direciona o usuário para a seção de registro de lançamentos no pedido
                break;
            case '2': // remover lançamentos
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nEntrando em REMOVER lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                RmLancamento(); // direciona o usuário para a seção de remoção de lançamentos no pedido
                break;
            case '3': // confirmar pedido
                ConfirmarPedido(); // direciona o usuário para a seção de confirmação do pedido
                break;
            case 27: // fechar o sistema (informação oculta para o cliente)
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nFechando sistema...\n");
                break;
                /*Como a função TelaInicial foi iniciada em main ao terminar esse ciclo a execução do
                programa volta para a função main e finaliza a execução pois a função TelaInicial era
                o último comando do programa.*/
            default: // caso o usuário digite uma opção diferente
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa para leitura da mensagem pelo usuário
                break;
        }
    } while (opcao != 27);
    // enquanto o usuário não ESC ele ficará realizando o ciclo da aprensentação da página da 'tela_inicial'
}

//--------------------------------------
/**SUB-MÓDULOS DE VENDAS**/
//--------------------------------------

void CancelarPedido (void)
/*Objetivo da função
Realizar o cancelamento do pedido montado*/
{
    // funções utilizadas
    int getch(void);
    void LimparLancaPedido(void);
    void LimparPgtoPedido(void);
    void NovoNPedido(void);
    void TelaInicial(void);

    // variáveis locais
    char ConfCancel; // variável que guardará a decisão de confirmação do cancelamento do pedido

    if (IndiceLancamento == 0) // caso não haja lançamentos no pedido
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

            ConfCancel = getch(); // guarda o caractere digitado pelo usuário
        } while (ConfCancel != '0' && ConfCancel != '1');
        // enquanto o usuário não digitar uma opção válida ele ficará realizando o ciclo da aprensentação do pop-up de confirmação do cancelamento do pedido

        if(ConfCancel == '0') // não cancelar pedido
        {
            system("clear || cls"); // limpa a tela para uma sensação de pop-up
            printf("\nPedido NAO CANCELADO.\n");
        }
        else // cancelar pedido
        {
            LimparLancaPedido();
            LimparPgtoPedido();
            NovoNPedido();
            system("clear || cls"); // limpa a tela para uma sensação de pop-up
            printf("\nPedido CANCELADO.\n");
        }
    }
    printf("\nPressione qualquer tecla para voltar...");
    getch(); // pausa para leitura da mensagem pelo usuário
}

void RegLancamento (void)
/*Objetivo da função
Realizar o registro de lançamentos no pedido*/
{
    // funções utilizadas
    void MostrarMenu(void);
    void MostrarPedido(void);
    void limpabuffer(void);
    void LimparLancamento(int IndiceLancamento, bool Subir);
    int  getch(void);
    int  BuscarCodProd(int CodProd);
    void MostrarLancamento(int IndiceRef);
    void CalculaPgtoTotal(void);

    // variáveis locais
    int IndiceCodProd; // criação da variável que guardará o índice do CodProd que está sendo registrado
    char ConfLancamento; // variável que guardará a decisão de confirmação do registro do lançamento no pedido
    bool Voltar; // variável para definir o usuário retornará na tela principal ou não

    do
    {
        system("clear || cls"); // limpa a tela para sensação de uma nova página

        printf("\n>>>REGISTRO DE LANCAMENTOS\n"); // título da página

        MostrarMenu(); // apresentar na tela os produtos disponíveis com o CodProd ao lado

        MostrarPedido(); // apresentar na tela todas as informações do pedido até o momento

        /*INTERAÇÃO COM O USUÁRIO*/
        printf("\nInsira o codigo do produto desejado (-1 para voltar): ");
        scanf("%i", &LancaPedido[IndiceLancamento].CodProd); // grava o valor digitado pelo usuário na variável CodProd
        limpabuffer(); // limpa o ENTER digitado

        if (LancaPedido[IndiceLancamento].CodProd < 0) // caso o usuário tenha digitado -1 para voltar a tela inicial
        {
            LimparLancamento(IndiceLancamento, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
            system("clear || cls"); // limpa a tela para uma sensação de pop-up
            printf("\nVoltando para a TELA INICIAL...\n");
            printf("\nPressione qualquer tecla para voltar...");
            Voltar = true; // fará com que o ciclo da função termine e o usuário retorne a tela principal
            getch(); // pausa para leitura da mensagem pelo usuário
        }
        else // caso o usuário tenha digitado algum valor de CodProd
        {
            IndiceCodProd = BuscarCodProd(LancaPedido[IndiceLancamento].CodProd); // realiza uma busca do índice que o CodProd digitado se encontra no 'menu'

            if (IndiceCodProd == -1) // caso o CodProd digitado não tenha sido encontrado no 'menu'
            {
                LimparLancamento(IndiceLancamento, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                system("clear || cls"); // limpa a tela para uma sensação de pop-up
                printf("\nCodigo de produto digitado INVALIDO.\n");
                printf("\nPor favor, digite um codigo valido.\n");
                getch(); // pausa para leitura da mensagem pelo usuário
            }
            else // caso o CodProd digitado tenha sido encontrado no 'menu'
            {
                LancaPedido[IndiceLancamento].NPedido = NPedidoAtual; // guarda o nº do pedido na linha do lançamento

                for (int i = 0; i < tNomeProd; ++i) // passa por todos os caracteres do NomeProd
                    LancaPedido[IndiceLancamento].NomeProd[i] = Menu[IndiceCodProd].NomeProd[i]; // guarda cada caractere do NomeProd do item escolhido para o NomeProd do lançamento
                LancaPedido[IndiceLancamento].NomeProd[tNomeProd] = '\0'; // torna o último caractere de NomeProd '\0' (vazio)

                LancaPedido[IndiceLancamento].CustoProd = Menu[IndiceCodProd].CustoProd; // guarda o CustoProd do item escolhido em CustoProd do lançamento

                /*INTERAÇÃO COM O USUÁRIO*/
                printf("\nInsira a quantidade desejada (-1 para voltar): ");
                scanf("%i", &LancaPedido[IndiceLancamento].Qtd); // grava o valor digitado pelo usuário na variável Qtd
                limpabuffer(); // limpa o ENTER digitado

                if (LancaPedido[IndiceLancamento].Qtd < 0) // caso o usuário tenha digitado -1 para voltar ao registro de lançamento
                {
                    LimparLancamento(IndiceLancamento, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                    system("clear || cls"); // limpa a tela para uma sensação de pop-up
                    printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    getch(); // pausa para leitura da mensagem pelo usuário
                }
                else if (LancaPedido[IndiceLancamento].Qtd < minQtd || LancaPedido[IndiceLancamento].Qtd > maxQtd) // caso o usuário tenha digitado um valor inválido de Qtd
                {
                    LimparLancamento(IndiceLancamento, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                    system("clear || cls"); // limpa a tela para uma sensação de pop-up
                    printf("\nLimite minimo por lancamento  = %i.", minQtd);
                    printf("\nLimite maximo por lancamento  = %i.\n", maxQtd);
                    printf("\nPressione qualquer tecla para voltar...");
                    getch(); // pausa para leitura da mensagem pelo usuário
                }
                else // caso o usuário tenha digitado um valor válido para Qtd
                {
                    LancaPedido[IndiceLancamento].SubTotal = LancaPedido[IndiceLancamento].CustoProd * LancaPedido[IndiceLancamento].Qtd; // guarda o SubTotal do lançamento

                    do
                    {
                        system("clear || cls"); // limpa a tela para uma sensação de pop-up
                        MostrarLancamento(IndiceLancamento); // apresenta o lançamento que o cliente acabou de escrever

                        /*INTERAÇÃO COM O USUÁRIO*/
                        // apresentará as opções do usuário na tela
                        printf("\nConfirmar lancamento?\n");
                        printf("0 - nao\n");
                        printf("1 - sim\n");

                        ConfLancamento = getch(); // guarda o caractere digitado pelo usuário
                    } while (ConfLancamento != '0' && ConfLancamento != '1');
                    // enquanto o usuário não digitar uma opção válida ele ficará realizando o ciclo de apresentação pop-up de confirmação do lançamento

                    if (ConfLancamento == '0') // lançamento cancelado
                    {
                        LimparLancamento(IndiceLancamento, true); // limpa qualquer dado registrado na linha do lançamento que estava sendo realizado
                        system("clear || cls"); // limpa a tela para uma sensação de pop-up
                        printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); // pausa para leitura da mensagem pelo usuário
                    }
                    else // lançamento confirmado
                    {
                        ++IndiceLancamento; // vai para o próximo índice de um lançamento novo
                        CalculaPgtoTotal(); // calcula o novo Total do pedido
                        system("clear || cls"); // limpa a tela para uma sensação de pop-up
                        printf("\nLancamento REGISTRADO!\n");
                        printf("\nPressione qualquer tecla para continuar...");
                        getch(); // pausa para leitura da mensagem pelo usuário
                    }
                }
            }
        }
    } while (! Voltar);
}

void RmLancamento (void) // em construcao
/*Objetivo da função
*/
{
    printf("\nVoce escolheu REMOVER lançamento.\n");
    /*
    Este seria o sub-módulo 'rm_lancamento' que tem no mapa lógico.
    Creio que deva ser montado com base nele.
    */
}

void ConfirmarPedido (void) // revisar
/*Objetivo da função
*/
{
    int getch(void); // LINUX
    void Cobranca(void);

    system("clear || cls");

    if(IndiceLancamento == 0)
    {
        printf("\nNao ha itens lancados no pedido.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
    }
    else
    {
        printf("Entrando em FORMAS DE PAGAMENTO...\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        Cobranca();
    }
}

//--------------------------------------
/**MÓDULO COBRANÇA**/
//--------------------------------------

void Cobranca (void) // revisar
{
    // funções utilizadas
    void MostrarPedido(void);
    int  getch(void); //LINUX
    bool PgtoDinheiro(void);
    bool PgtoCheque(void);
    bool PgtoDebito(void);
    bool PgtoCredito(void);

    // variáveis locais
    char opcao;
    bool Voltar;
    bool ConfPgto;

    do
    {
        Voltar = false;
        ConfPgto = false;

        system("clear || cls");

        printf("\n>>>FORMA DE PAGAMENTO\n");

        MostrarPedido();

        printf("\nOpcoes:\n");
        printf(  "(0) - Lancar itens/Cancelar pedido\n");
        printf(  "(1) - Dinheiro\n");
        printf(  "(2) - Cheque\n");
        printf(  "(3) - Cartao de Debito\n");
        printf(  "(4) - Cartao de Credito\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando para a TELA INICIAL...\n");
                printf("\nPressione qualquer tecla para voltar...");
                Voltar = true;
                getch();
                break;
            case '1':
                system("clear || cls");
                printf("\nEntrando em pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                ConfPgto = PgtoDinheiro();
                break;
            case '2':
                system("clear || cls");
                printf("\nEntrando em pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                ConfPgto = PgtoCheque();
                break;
            case '3':
                system("clear || cls");
                printf("\nEntrando em pagamento em DEBITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                ConfPgto = PgtoDebito();
                break;
            case '4':
                system("clear || cls");
                printf("\nEntrando em pagamento em CREDITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                ConfPgto = PgtoCredito();
                break;
            default:
                system("clear || cls");
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                break;
        }
        if (ConfPgto)
            Voltar = true;
    } while (! Voltar);
}

//--------------------------------------
/**SUB-MÓDULOS DE COBRANÇA**/
//--------------------------------------

bool PgtoDinheiro (void) // revisar
{
    // funções utilizadas
    void limpabuffer(void);
    int  getch(void);
    bool FinalizarPedido(void);

    // variáveis locais
    float ValorRecebido, Troco;
    char ConfPgto;
    bool Voltar, Pgto;

    do
    {
        Voltar = false;
        Pgto = false;

        system("clear || cls");
        printf("\n>>>PAGAMENTO EM DINHEIRO\n");
        printf("\nValor TOTAL da compra: %.2f\n", PgtoPedido.Total);
        printf("\nInsira o dinheiro (digite -1 para voltar): ");
        scanf("%f", &ValorRecebido);
        limpabuffer();

        if (ValorRecebido < 0)
        {
            ValorRecebido = '\0';
            Voltar = true;
            system("clear || cls");
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else if (ValorRecebido < PgtoPedido.Total)
        {
            ValorRecebido = '\0';
            system("clear || cls");
            printf("\nValor insuficiente.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else
        {
            Troco = ValorRecebido - PgtoPedido.Total;

            do
            {
                system("clear || cls");
                printf("\nValor entregue: %.2f\n", ValorRecebido);
                printf("\nValor do troco a ser recebido: %.2f\n", Troco);
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                ConfPgto = getch();
            } while (ConfPgto != '0' && ConfPgto != '1');

            if (ConfPgto == '0')
            {
                Troco = '\0';
                ValorRecebido = '\0';
                ConfPgto = '\0';
                system("clear || cls");
                printf("\nVoltando para pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
            }
            else
            {
                PgtoPedido.NPedido = NPedidoAtual;
                PgtoPedido.FormaPgto = '1';
                strcpy(PgtoPedido.NCartao, "0000000000000000");
                FinalizarPedido();
                Pgto = true;
                Voltar = true;
            }
        }
    } while (! Voltar);

    if (Pgto)
        return true;
    else
        return false;
}

bool PgtoCheque (void) // revisar
{
    // funções utilizadas
    void limpabuffer(void);
    int  getch(void);
    void FinalizarPedido(void);

    // variáveis locais
    float ValorRecebido;
    char ConfPgto;
    bool Voltar, Pgto;

    do
    {
        Voltar = false;
        Pgto = false;

        system("clear || cls");
        printf("\n>>>PAGAMENTO EM CHEQUE\n");
        printf("\nValor TOTAL da compra: %.2f\n", PgtoPedido.Total);
        printf("\nInsira o valor do cheque (digite -1 para voltar): ");
        scanf("%f", &ValorRecebido);
        limpabuffer();

        if (ValorRecebido < 0)
        {
            ValorRecebido = '\0';
            Voltar = true;
            system("clear || cls");
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else if (ValorRecebido != PgtoPedido.Total)
        {
            ValorRecebido = '\0';
            system("clear || cls");
            printf("\nValor nao correspondente.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else
        {
            do
            {
                system("clear || cls");
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                ConfPgto = getch();
            } while (ConfPgto != '0' && ConfPgto != '1');

            if (ConfPgto == '0')
            {
                ValorRecebido = '\0';
                ConfPgto = '\0';
                system("clear || cls");
                printf("\nVoltando pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
            }
            else
            {
                PgtoPedido.NPedido = NPedidoAtual;
                PgtoPedido.FormaPgto = '2';
                strcpy(PgtoPedido.NCartao, "0000000000000000");
                FinalizarPedido();
                Pgto = true;
                Voltar = true;
            }
        }
    } while (! Voltar);

    if (Pgto)
        return true;
    else
        return false;
}

bool PgtoDebito (void) // falta fazer
{
    printf("\nVocê escolheu DEBITO.\n");
    return false;
}

bool PgtoCredito (void) // falta fazer
{
    printf("\nVocê escolheu CREDITO.\n");
    return false;
}

//--------------------------------------
/**FUNÇÕES AUXILIARES**/
//--------------------------------------

///Funções de limpeza

void LimparMenu (void)
/*Objetivo da função
Limpar qualquer "sujeira" da variável 'menu'*/
{
    for (int i = 0; i < maxMenu; ++i) // passa por todas as linhas da variável 'menu'
    {
        Menu[i].CodProd = '\0'; // preenche '\0' (vazio) em CodProd
        for (int ii = 0; ii < tNomeProd; ++ii) // passa por todos os caracteres de 'NomeProd'
            Menu[i].NomeProd[ii] = '\0'; // preenche '\0' (vazio) em todos os caracteres de 'NomeProd'
        Menu[i].CustoProd = '\0'; // preenche '\0' (vazio) em CustoProd
    }
}

void LimparLancaPedido (void)
/*Objetivo da função
Limpar todos os lançamentos de 'LancaPedido'*/
{
    void LimparLancamento(int IndiceLancamento, bool Subir);

    for (int i = 0; i < IndiceLancamento; ++i)
        LimparLancamento(i, false);

    IndiceLancamento = 0;
}

void LimparPgtoPedido (void)
/*Objetivo da função
Limpar 'PgtoPedido'*/
{
    PgtoPedido.NPedido = '\0'; // preenche '\0' (vazio) em N_pedido

    PgtoPedido.Total = '\0'; // preenche '\0' (vazio) em Total

    PgtoPedido.FormaPgto = '\0'; // preenche '\0' (vazio) em FormaPgto

    for (int i = 0; i < tNCartao; ++i) // passa por todos os caracteres de N_Cartao
        PgtoPedido.NCartao[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de N_Cartao

    for (int i = 0; i < tNomeCliente; ++i) // passa por todos os caracteres de NomeCliente
        PgtoPedido.NomeCliente[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de NomeCliente
}

void LimparLancamento (int IndiceLancamentoRef, bool Subir)
/*Objetivo da função
- Limpar um lançamento específico do pedido;
- Chama a função 'subir_lancamento' se o segundo argumento for 'true'
  (usado quando se quer limpar um lançamento específico apenas;*/
{
    // funções utilizadas
    void SubirLancamento(int IndiceRef);
    void CalculaPgtoTotal(void);

    LancaPedido[IndiceLancamentoRef].NPedido = '\0'; // preenche '\0' (vazio) em N_pedido

    LancaPedido[IndiceLancamentoRef].CodProd = '\0'; // preenche '\0' (vazio) em CodProd

    for (int i = 0; i < tNomeProd; ++i) // passa por todos os caracteres de NomeProd
        LancaPedido[IndiceLancamentoRef].NomeProd[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de NomeProd

    LancaPedido[IndiceLancamentoRef].CustoProd = '\0'; // preenche '\0' (vazio) em CustoProd

    LancaPedido[IndiceLancamentoRef].Qtd = '\0'; // preenche '\0' (vazio) em Qtd

    LancaPedido[IndiceLancamentoRef].SubTotal = '\0'; // preenche '\0' (vazio) em SubTotal

    if (Subir) // caso se queira subir os lançamentos abaixo
    {
        while (LancaPedido[IndiceLancamentoRef + 1].CodProd != '\0') // o ciclo roda até que a próxima linha seja '\0' (vazio)
        {
            SubirLancamento(IndiceLancamentoRef + 1); // sobe o próximo lancamento para a o 'indice_lancamento'
            ++IndiceLancamentoRef; // vai para o próximo índice de lançamento
        }

        IndiceLancamento = IndiceLancamentoRef; // atualiza o índice do próximo lançamento a realizar

        CalculaPgtoTotal(); // calcula o 'Total' do pedido atual
    }
}

void SubirLancamento (int IndiceRef)
/*Objetivo da função
Copia os dados do índice referente para um índice anterior e apaga os dados do índice referente*/
{
    //funções utilizadas
    void LimparLancamento(int IndiceLancamentoRef, bool subir);

    int IndiceNovo = IndiceRef - 1; // cria uma variável que guardará o índice anterior (onde que ser gravar os dados do índice referente)

    LancaPedido[IndiceNovo].NPedido = LancaPedido[IndiceRef].NPedido; // copia o N_pedido do índice referente para o índice anterior

    LancaPedido[IndiceNovo].CodProd = LancaPedido[IndiceRef].CodProd; // copia o CodProd do índice referente para o índice anterior

    strcpy(LancaPedido[IndiceNovo].NomeProd, LancaPedido[IndiceRef].NomeProd); // copia o NomeProd do índice referente para o índice anterior

    LancaPedido[IndiceNovo].CustoProd = LancaPedido[IndiceRef].CustoProd; // copia o CustoProd do índice referente para o índice anterior

    LancaPedido[IndiceNovo].Qtd = LancaPedido[IndiceRef].Qtd; // copia o Qtd do índice referente para o índice anterior

    LancaPedido[IndiceNovo].SubTotal = LancaPedido[IndiceRef].SubTotal; // copia o SubTotal do índice referente para o índice anterior

    LimparLancamento(IndiceRef, false); // limpa os dados do índice referente que foram copiados para o índice anterior
}

///Funções de impressão

void MostrarPedido (void)
/*Objetivo da função
Imprimir na tela as informações até o momento do pedido*/
{
    //funções utilizadas
    void ImprimeNPedido(void);
    void ImprimeCabecalhoPedido(void);
    void ImprimeLancamento(int IndiceRef);
    void ImprimeRodapePedido(void);

    if(IndiceLancamento == 0) // se 'indice_l' for igual a zero quer dizer que ainda não houveram lançamentos no pedido
    {
        printf("\nPedido No %i", NPedidoAtual); // informações apresentadas caso não haja lançamentos no pedido
        printf("\nNao ha lancamentos no pedido.\n");
    }
    else // apresentação das informações do pedido caso haja lançamentos
    {
        printf("\n==================SEU PEDIDO=================="); // título do quadro

        ImprimeNPedido(); // imprime o nº do pedido

        ImprimeCabecalhoPedido(); // imprime o cabeçalho de lançamentos

        for(int i = 0; i < IndiceLancamento; ++i) // ciclo que fará a impressão de lançamento por lançamento a partir da variável 'l_pedido'
            ImprimeLancamento(i); // imprime um lançamento

        ImprimeRodapePedido(); // imprime o total do pedido

        printf("==============================================\n"); // rodapé do quadro
    }
}

void ImprimeNPedido (void)
{
    printf("\n                              Pedido No ");
    // dependendo do valor de 'n_pedido_atual' é necessário uma quantidade de espaços para alinhar ao quadro
    if (NPedidoAtual < 10)
        printf("     %i\n", NPedidoAtual);
    else if (NPedidoAtual < 100)
        printf("    %i\n" , NPedidoAtual);
    else if (NPedidoAtual < 1000)
        printf("   %i\n"  , NPedidoAtual);
    else if (NPedidoAtual < 10000)
        printf("  %i\n"   , NPedidoAtual);
    else if (NPedidoAtual < 100000)
        printf(" %i\n"    , NPedidoAtual);
    else
        printf("%i\n"     , NPedidoAtual); // valor máximo do 'n_pedido_atual' é de 100000
}

void ImprimeCabecalhoPedido (void)
{
    printf("\nCodProd Nome do Produto   Custo  Qtd  SubTotal\n"); // cabeçalho da tabela
}

void ImprimeLancamento (int IndiceRef)
{
    int i = IndiceRef; // cria uma variável que guardará o índica do lançamento que se deseja imprimir

    // CodProd
    printf("%i      ",   LancaPedido[i].CodProd); // imprime CodProd (como tem sempre dois caracteres não é necessário quantidade de espações diferentes

    // Nome do Produto
    for (int ii = 0; ii < tNomeProd; ++ii) // ciclo que passa por todos os caracteres de NomeProd
    {
        if (LancaPedido[i].NomeProd[ii] != '\0') // se o caractere de NomeProd for diferente de vazio ele imprime na tela
            printf("%c", LancaPedido[i].NomeProd[ii]);
        else // se for vazio ele imprime um espaço
            printf(" ");
    }
    printf(" "); // um espaço para separação da próxima informação

    // Custo
    // dependendo do valor de CustoProd é necessário uma quantidade de espaços para alinhar ao quadro
    if (LancaPedido[i].CustoProd < 10)
        printf("   %.2f ", LancaPedido[i].CustoProd);
    else if (LancaPedido[i].CustoProd < 100)
        printf("  %.2f " , LancaPedido[i].CustoProd);
    else
        printf(" %.2f "  , LancaPedido[i].CustoProd); // valor máximo de CustoProd é de 100.00

    // Qtd
    // dependendo do valor de Qtd é necessário uma quantidade de espaços para alinhar ao quadro
    if (LancaPedido[i].Qtd < 10)
        printf("   %i ", LancaPedido[i].Qtd);
    else if (LancaPedido[i].Qtd < 100)
        printf("  %i " , LancaPedido[i].Qtd);
    else
        printf(" %i "  , LancaPedido[i].Qtd); // valor máximo de Qtd é de 100

    // SubTotal
    // dependendo do valor de SubTotal é necessário uma quantidade de espaços para alinhar ao quadro
    if (LancaPedido[i].SubTotal < 10)
        printf("     %.2f\n", LancaPedido[i].SubTotal);
    else if (LancaPedido[i].SubTotal < 100)
        printf("    %.2f\n" , LancaPedido[i].SubTotal);
    else if (LancaPedido[i].SubTotal < 1000)
        printf("   %.2f\n"  , LancaPedido[i].SubTotal);
    else if (LancaPedido[i].SubTotal < 10000)
        printf("  %.2f\n"   , LancaPedido[i].SubTotal);
    else
        printf(" %.2f\n"    , LancaPedido[i].SubTotal); // valor máximo de SubTotal é de 10,000.00
}

void ImprimeRodapePedido (void)
{
    printf("\n                               Total ");
    // dependendo do valor de Total é necessário uma quantidade de espaços para alinhar ao quadro
    if (PgtoPedido.Total < 10)
        printf("     %.2f\n", PgtoPedido.Total);
    else if (PgtoPedido.Total < 100)
        printf("    %.2f\n" , PgtoPedido.Total);
    else if (PgtoPedido.Total < 1000)
        printf("   %.2f\n"  , PgtoPedido.Total);
    else if (PgtoPedido.Total < 10000)
        printf("  %.2f\n"   , PgtoPedido.Total);
    else if (PgtoPedido.Total < 100000)
        printf(" %.2f\n"    , PgtoPedido.Total);
    else
        printf("%.2f\n"     , PgtoPedido.Total); // valor máximo de Ttoal é de 100,000.00
}

void MostrarMenu (void)
/*Objetivo da função
Imprimir na tela os produtos disponíveis no 'menu'*/
{
    if (Menu[0].CodProd == '\0') // se o primeiro CodProd for '\0' (vazio), então não há itens no 'menu'
        printf("\nNao ha itens no menu.\n");
    else // apresentação das informações do 'menu' caso haja produtos disponíveis
    {
        printf("\n============MENU============="); // título da quadro
        printf("\nCodigo Nome do Produto  Custo\n"); // cabeçalho da tabela
        for (int i = 0; Menu[i].CodProd != '\0'; ++i) // ciclo que fará a impressão dos produtos disponíveis
        {
            // CodProd
            printf("%i     ", Menu[i].CodProd); // imprime CodProd (como tem sempre dois caracteres não é necessário quantidade de espações diferentes

            // NomeProd
            for (int ii = 0; ii < tNomeProd; ++ii) // ciclo que passa por todos os caracteres de NomeProd
            {
                if (Menu[i].NomeProd[ii] != '\0') // se o caractere de NomeProd for diferente de vazio ele imprime na tela
                    printf("%c", Menu[i].NomeProd[ii]);
                else // se for vazio ele imprime um espaço
                    printf(" ");
            }
            printf(" "); // um espaço para separação da próxima informação

            // Custo
            // dependendo do valor de CustoProd é necessário uma quantidade de espaços para alinhar ao quadro
            if (Menu[i].CustoProd < 10)
                printf("  %.2f\n", Menu[i].CustoProd);
            else if (Menu[i].CustoProd < 100)
                printf(" %.2f\n" , Menu[i].CustoProd);
            else
                printf("%.2f\n"  , Menu[i].CustoProd); // valor máximo de CustoProd é de 100.00
        }
        printf("=============================\n"); // rodapé do quadro
    }
}

void MostrarLancamento (int IndiceRef)
/*Objetivo da função
Imprimir na tela um lançamento específico*/
{
    //funções utilizadas
    void ImprimeCabecalhoPedido(void);
    void ImprimeLancamento(int IndiceRef);

    ImprimeCabecalhoPedido(); // imprime o cabeçalho de lançamentos

    ImprimeLancamento(IndiceRef); // imprime o lançamento indicado
}

///Funções de iniciação ou gravação

void DownloadMenu (void)
/*Objetivo da função
Carregar do arquivo "menu_produtos.DAT" quais são os produtos disponíveis e guardar na variável 'menu'*/
{
    // funções utilizadas
    void LimparMenu(void);

    LimparMenu(); // limpar qualquer "sujeira" na variável 'menu'

    FILE *PontMenu; // criação de uma variável ponteiro para o arquivo que contém os produtos disponíveis

    PontMenu = fopen(MenuDat, "r"); // leitura do arquivo onde se encontram os produtos disponíveis

    if (PontMenu == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        int i = 0; // índice da variável vetor 'menu'

        do
        {
            fscanf(PontMenu, "%i\t", &Menu[i].CodProd); // grava o 'CodProd' que se encontra na linha (i + 1) do arquivo
            fscanf(PontMenu, "%s\t",  Menu[i].NomeProd); // grava o 'NomeProd' que se encontra na linha (i + 1) do arquivo /não pode haver espaços na string
            fscanf(PontMenu, "%f\n", &Menu[i].CustoProd); // grava o 'CustoProd' que se encontra na linha (i + 1) do arquivo
            ++i;
        } while (Menu[i - 1].CodProd != '\0' && i < maxMenu); // irá gravar em cada linha da variável 'menu' os dados do arquivo até que a última gravação seja '\0' (vazio)

        fclose(PontMenu); // fecha o arquivo
    }
}

void NovoNPedido (void)
/*Objetivo da função
- Ler no arquivo qual foi o último nº do pedido aberto;
- Guardar o próximo nº da variável 'n_pedido_atual';
- Gravar no arquivo o 'n_pedido_atual'.*/
{
    FILE *PontNPedido; // criação de uma variável ponteiro para o arquivo que contém o último nº de pedido aberto

    PontNPedido = fopen(NPedidoDat, "r"); // leitura do arquivo onde se encontra o último nº de pedido aberto

    if (PontNPedido == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        fscanf(PontNPedido, "%i", &NPedidoAtual); // grava o nº do último pedido aberto em 'n_pedido_atual'
        ++NPedidoAtual; // acrescenta +1 para se tornar o próximo nº de pedido sendo o atual

        fclose(PontNPedido); // fecha o arquivo

        PontNPedido = fopen(NPedidoDat, "w"); // recriação do arquivo onde se encontrará o último nº de pedido aberto

        fprintf(PontNPedido, "%i", NPedidoAtual); // grava o nº de pedido que acabou de abrir no arquivo para que este seja o último nº de pedido aberto
    }
    fclose(PontNPedido); // fecha o arquivo
}

void UploadPedido (void)
/*Objetivo da função
Gravar nos arquivos "lancamentos_pedidos.DAT" e "pagamentos_pedidos.DAT os dados do pedido realizado"*/
{
    // funções utilizadas
    void LimparLancaPedido(void);
    void LimparPgtoPedido(void);
    void NovoNPedido(void);

    FILE *PontLancamentos; // cria uma variável ponteiro para o arquivo que guarda os lançamentos confirmados
    FILE *PontPagamentos; // cria uma variável ponteiro para o arquivo que guarda os pagamentos realizados

    PontLancamentos = fopen(LancamentosDat, "a"); // abre para anexar lançamentos confirmado do pedido realizado

    if (PontLancamentos == NULL)
        printf("\nArquivo nao pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        for (int i = 0; i < IndiceLancamento; i++) // passar por todos os lançamentos do pedido
        {
            fprintf(PontLancamentos, "%i\t",   LancaPedido[i].NPedido);   // grava o nº do pedido
            fprintf(PontLancamentos, "%i\t",   LancaPedido[i].CodProd);   // grava o código do produto
            fprintf(PontLancamentos, "%s\t",   LancaPedido[i].NomeProd);  // grava o nome do produto
            fprintf(PontLancamentos, "%.2f\t", LancaPedido[i].CustoProd); // grava o custo unitário
            fprintf(PontLancamentos, "%i\t",   LancaPedido[i].Qtd);       // grava a quantidade pedida
            fprintf(PontLancamentos, "%.2f\n", LancaPedido[i].SubTotal);  // grava o sub-total do lançamento
        }
    }

    fclose(PontLancamentos); // fecha o arquivo

    PontPagamentos = fopen(PagamentosDat, "a"); // abre para anexar pagamento realizado do pedido

    if (PontPagamentos == NULL)
        printf("\nArquivo não pode ser aberto\n"); // caso ocorra problemas na leitura do arquivo
    else
    {
        fprintf(PontPagamentos, "%i\t",   PgtoPedido.NPedido);     // grava o nº do pedido
        fprintf(PontPagamentos, "%.2f\t", PgtoPedido.Total);       // grava o total do pedido
        fprintf(PontPagamentos, "%c\t",   PgtoPedido.FormaPgto);   // grava a forma de pagamento
        fprintf(PontPagamentos, "%s\t",   PgtoPedido.NCartao);     // grava o número do cartão
        fprintf(PontPagamentos, "%s\n",   PgtoPedido.NomeCliente); // grava o nome do cliente
    }

    fclose(PontPagamentos); // fecha o arquivo

    LimparLancaPedido(); // limpa todos os lancamento da variável 'l_pedido'
    LimparPgtoPedido();  // limpa todos os dados de 'p_pedido'
    NovoNPedido();       // gera um novo nº de pedido
}

///Funções diversas

int BuscarCodProd (int CodProd)
/*Objetivo da função
Retorna o valor do índice de um CodProd específico, caso não encontre será retornado -1*/
{
    int IndiceCodProd = -1; // variável que guardará a o índice do produto procurado, ele já é -1 para caso não seja encontrado o produto
    int i = 0; // índice que seja procurado, inicia em 0

    while (IndiceCodProd == -1 && Menu[i].CodProd != '\0') // ciclo que estará buscando o índice até que se encontre ou passe por todos os itens
    {
        if(CodProd != Menu[i].CodProd) // caso o CodProd do 'menu' seja diferente do CodProd procurado
            ++i; // vai para o próximo índice
        else
            IndiceCodProd = i; // copia o índice na variável que será retornada
    }

    return IndiceCodProd; // retorna o índice do CodProd procurado ou -1 se não tiver sido encontrado
}

void CalculaPgtoTotal (void)
/*Objetivo da função
Calcular o Total de 'p_pedido' até o momento*/
{
    PgtoPedido.Total = 0; // zera o Total do pedido

    for (int i = 0; i < IndiceLancamento; ++i) // somará todos os SubTotal de 'l_pedido' em Total de todas as linhas preenchidas
        PgtoPedido.Total += LancaPedido[i].SubTotal;
}

void FinalizarPedido (void) // revisar falta completar
{
    void getstring(char linha[]);
    void UploadPedido(void);
    int  getch (void);

    system("clear ||cls");
    printf("\n>>>FINALIZACAO DE PEDIDO\n");
    printf("\nDigite seu NOME: ");
    getstring (PgtoPedido.NomeCliente);
    UploadPedido();
    system("clear ||cls");
    printf("\nPedido CONFIRMADO!\n");
    printf("\nPagamento REALIZADO!\n");
    printf("\nAGRADECEMOS a sua compra!");
    getch();
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

void limpabuffer (void) // função criada para limpar o buffer do teclado que funciona em WINDOWS e LINUX
/*Objetivo da função
Limpar qualquer "sujeira" que tiver no buffer do teclado. Só funciona se realmente houver algo no buffer
Deve-se utilizar após a função 'scanf'*/
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getstring (char linha[]) // função que funciona melhor que gets
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
