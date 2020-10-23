//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: Drive-thru Mickey & Donald

Descricao:
Projeto para avaliacao de desempenho na discipĺina de "Logica de programacao 1"
do curso "Tecnologo de Analise e Desenvolvimento de Sistemas" no Instituto Federal
de Sao Paulo - campus Sao Paulo

Desenvolvedor(es):
Felippe Marques da Silva de Almeida SP3056686
Leandro Paiva Higa                  SP3052648

Ultima atualizacao: 23-out.-2020 as 12h51
*/

//--------------------------------------
/**ORGANIZACAO DE DADOS**/
//--------------------------------------

///DEFINICOES:
/*
> lancamento: registro de uma escolha de um produto e sua quantidade
              pelo cliente.
> pedido: conjunto de lancamentos realizados pelo cliente.
> menu: lista de produtos disponiveis para o cliente.
*/

///DADOS:
/*
Ha 3 tabelas onde sao gravados/lidos dados:
-- menu_produtos: onde estao indicados os produtos disponiveis.
-- lancamentos_pedidos: onde estao gravados todos os lancamentos
  ja confirmados para preparacao.
-- pagamentos_pedidos: onde estao gravados todos os pagamentos de
  pedidos realizados.

Os dados sao acompanhados por '/t' quando ha algum dado na linha e
por '\n' quando a linha termina.

Os dados do tipo 'string' devem conter '_' em vez de ' '.
*/

//--------------------------------------
/**BIBLIOTECAS UTILIZADAS**/
//--------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <termios.h> //Linux (criacao de getch)
#include <unistd.h> //Linux (criacao de getch)
//#include <conio.h> //Windows (getch)
#include <string.h>

//--------------------------------------
/**CONSTANTES DO PROGRAMA**/
//--------------------------------------

///Tamanho de strings

const int t_nome_prod    = 15; //tamanho da string 'nome_prod'
const int t_n_cartao     = 17; //tamanho da string 'n_cartao'
const int t_nome_cliente = 21; //tamanho da string 'nome_cliente'

///Maximos e minimos

const int max_menu        = 100; //maximo de itens no 'menu'
const int max_lancamentos = 100; //maximo de itens no 'lanca_pedido'
const int min_cod_prod    =  10; //valor minimo de 'cod_prod'
const int max_cod_prod    =  10; //valor maximo de 'cod_prod'
const int min_qtd         =   1; //valor minimo de 'qtd'
const int max_qtd         = 100; //valor maximo de 'qtd'

///Endereço dos arquivos

const char menu_dat[]        = {"menu.DAT"};        //nome do arquivo que se encontra os dados do 'menu'
const char lancamentos_dat[] = {"LANCAMENTOS.DAT"}; //nome do arquivo que se encontra os dados de 'lanca_pedido'
const char pagamentos_dat[]  = {"PAGAMENTOS.DAT"};  //nome do arquivo que se encontra os dados de 'pgto_pedido'
const char n_pedido_dat[]    = {"N_PEDIDO.DAT"};    //nome do arquivo que se encontra os dados do 'n_pedido'

//--------------------------------------
/**ESTRUTURAS UTILIZADAS**/
//--------------------------------------

struct produto //estrutura para formar a lista de produtos disponiveis menu
{
    int   cod_prod;               //codigo do produto
    char  nome_prod[t_nome_prod]; //nome do produto (max caracteres: 14 | 15 = '\0')
    float custo_prod;             //custo do produto
};

struct lancamento //estrutura para formar os lancamentos de um pedido
{
    int   n_pedido;               //n do pedido
    int   cod_prod;               //codigo do produto
    char  nome_prod[t_nome_prod]; //nome do produto (max caracteres: 14 | 15 = '\0')
    float custo_prod;             //custo do produto
    int   qtd;                    //quantidade do produto
    float subtotal;               //subtotal = custo_prod * qtd
    bool  preparado;              //true = preparado | false = a ser preparado
};

struct pagamento //estrutura para formar o pagamento de um pedido
{
    int   n_pedido;                     //n do pedido
    float total;                        //soma dos subtotais dos lancamentos
    char  forma_pgto;                   //forma de pagamento ('1'=dinheiro '2'=cheque '3'=debito '4'=credito)
    char  n_cartao[t_n_cartao];         //n do cartao (max caracteres: 16 / 17 = '\0') para dinheiro e cheque '000...'
    char  nome_cliente[t_nome_cliente]; //nome do cliente para identificacao na entrega (max caracteres: 20 / 21 = '\0')
    bool  entregue;                     //true = entregue | false = nao entregue
};


//--------------------------------------
/**FORMACAO DAS VARIAVEIS GLOBAIS**/
//--------------------------------------

struct produto menu[max_menu] = {}; //variavel de estrutura que guardara os produtos disponiveis

struct lancamento lanca_pedido[max_lancamentos] = {}; //variavel de estrutura que guardara os lancamentos registrados no pedido

struct pagamento pgto_pedido = {}; //variavel de estrutura que guardara o pagamento do pedido realizado

int    indice_lancamento = 0; //variavel inteira para guardara a proxima linha de 'lanca_pedido' a ser escrita

int    n_pedido_atual; //variavel que guardara o nº do pedido que esta sendo montado

//--------------------------------------
/**FUNCAO PRINCIPAL**/
//--------------------------------------

int main (void)
/*Objetivo da funcao
Acionada no inicio do programa.*/
{
    //funcoes utilizadas
    void import_menu(void);
    void novo_n_pedido(void);
    void tela_inicial(void);

    import_menu(); //importa os produtos do aquivo para o 'menu'

    novo_n_pedido(); //gera um novo n de pedido

    tela_inicial(); //inicia a tela inicial do programa

    return 0;
}

//--------------------------------------
/**MODULO VENDAS**/
//--------------------------------------

void tela_inicial (void)
/*Objetivo da funcao
Apresentar as opcoes de manipulacao de lancamentos do pedido e confirmacao de pedido para o usuario.*/
{
    //funcoes utilizadas
    void mostrar_pedido(void);
    int  getch(void);
    void cancelar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    //variaveis locais
    char opcao;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>TELA INICIAL\n"); //imprime titulo da pagina
        mostrar_pedido(); //imprime na tela todas as informacoes do pedido ate o momento

        /*INTERACAO COM O USUARIO*/
        //apresentacao das opcoes ao usuario
        printf("\nOpcoes:\n");
        printf(  "(0) - Cancelar pedido\n");
        printf(  "(1) - Registrar lancamentos\n");
        printf(  "(2) - Remover lancamentos\n");
        printf(  "(3) - Confirmar pedido\n");
        opcao = getch(); //capta qual eh a opcao escolhida pelo usuario

        switch(opcao) //encaminha o usuario para opcao escolhida
        {
            case '0': //opcao de cancelar o pedido
                cancelar_pedido(); //direciona o usuario para a secao de cancelamento do pedido
                break;

            case '1': //opcao de registrar lancamentos no pedido
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nEntrando em REGISTRAR lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                reg_lancamento(); //direciona para o submodulo 'reg_lancamento'
                break;

            case '2': //opcao de remover lancamentos no pedido
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nEntrando em REMOVER lancamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                rm_lancamento(); //direciona para o submodulo 'rm_lancamento'
                break;

            case '3': //opcao de confirmar o pedido
                confirmar_pedido(); //direciona para o submodulo 'confirmar_pedido'
                break;

            case 27: //sair o sistema (informacao oculta para o usuario)
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nFechando sistema...\n");
                break;

            default:
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); //pausa a tela para leitura do usuario
                break;
        }
    } while (opcao != 27);
    /*Enquanto o usuario nao apertar ESC para sair do programa
    ele continuara em execucao.*/
}

//--------------------------------------
/**SUB-MODULOS DE VENDAS**/
//--------------------------------------

void cancelar_pedido (void)
/*Objetivo da funcao
Realizar o processo de cancelamento do pedido montado.*/
{
    //funcoes utilizadas
    int getch(void);
    void limpar_lanca_pedido(void);
    void limpar_pgto_pedido(void);
    void novo_n_pedido(void);

    //variaveis locais
    char conf_cancel;

    if (indice_lancamento == 0) //se nao houver lancamentos no pedido
    {
        system("clear || cls"); //limpa a tela para uma sensacao de pop-up
        printf("\nNao ha lancamentos. Portanto, nao e possivel CANCELAR o pedido.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        do
        {
            system("clear || cls"); //limpa a tela para uma sensacao de pop-up

            /*INTERACAO COM O USUARIO*/
            //apresentara as opcoes do usuario na tela
            printf("\n>>>ATENCAO!!!\n");
            printf("\nTem certeza que deseja CANCELAR o pedido?\n");
            printf("(0) - nao\n");
            printf("(1) - sim\n");
            conf_cancel = getch(); //capta qual eh a opcao escolhida pelo usuario

            switch(conf_cancel) //encaminha o usuario para opcao escolhida
            {
                case '0': //opcao de nao cancelar pedido
                    system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                    printf("\nPedido NAO CANCELADO.\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    getch(); //pausa a tela para leitura do usuario
                    break;

                case '1': //opcao de cancelar pedido
                    limpar_lanca_pedido();
                    limpar_pgto_pedido();
                    novo_n_pedido();
                    system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                    printf("\nPedido CANCELADO.\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    getch(); //pausa a tela para leitura do usuario
                    break;

                default:
                    break;
            }
        } while (conf_cancel != '0' && conf_cancel != '1');
        /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
    }
}

void reg_lancamento (void)
/*Objetivo da funcao
Realizar o processo de registro de lancamentos no pedido.*/
{
    //funcoes utilizadas
    void mostrar_menu(void);
    void mostrar_pedido(void);
    void limpar_buffer(void);
    void limpar_lancamento(int indice_lancamento, bool subir);
    int  getch(void);
    int  buscar_cod_prod(int cod_prod);
    void mostrar_lancamento(int indice_ref);
    void calcular_pgto_total(void);

    //variaveis locais
    bool voltar;
    int  indice_cod_prod;
    char conf_lancamento;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>REGISTRO DE lancamentoS\n"); //imprime titulo da pagina
        mostrar_menu(); //imprime na tela o 'menu'
        mostrar_pedido(); //imprime na tela todas as informacoes do pedido ate o momento
        voltar = false; //por padrao ele nao permite voltar para a tela inicial

        /*INTERACAO COM O USUARIO*/
        //inserindo 'cod_prod' do produto desejado
        printf("\nInsira o codigo do produto desejado (0 para voltar): ");
        scanf("%i", &lanca_pedido[indice_lancamento].cod_prod);
        limpar_buffer(); //limpa o ENTER deixado pelo 'scanf'

        if (lanca_pedido[indice_lancamento].cod_prod == 0) //se o usuario escolher por retornar a tela inicial
        {
            limpar_lancamento(indice_lancamento, false); //limpa o que tiver registrado até o momento sem precisar subir o próximo
            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
            printf("\nVoltando para a TELA INICIAL...\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; //fara com que termine a execucao do submodulo e volte a tela inicial
            getch(); //pausa a tela para leitura do usuario
        }
        else if (buscar_cod_prod(lanca_pedido[indice_lancamento].cod_prod) == -1)
        {
            limpar_lancamento(indice_lancamento, false); //limpa o que tiver registrado até o momento sem precisar subir o próximo
            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
            printf("\nCodigo de produto digitado INVALIDO.\n");
            printf("\nPor favor, digite um codigo valido.\n");
            getch(); //pausa a tela para leitura do usuario
        }
        else
        {
            indice_cod_prod = buscar_cod_prod(lanca_pedido[indice_lancamento].cod_prod); //grava o indice do produto
            lanca_pedido[indice_lancamento].n_pedido = n_pedido_atual; //guarda o 'n_pedido' no lancamento
            lanca_pedido[indice_lancamento].preparado = false; //informa que lancamento nao esta preparado
            for (int i = 0; i < t_nome_prod; ++i) //passa por todos os caracteres de 'nome_prod'
                lanca_pedido[indice_lancamento].nome_prod[i] = menu[indice_cod_prod].nome_prod[i]; //guarda todos os caracteres de 'nome_prod' no lancamento
            lanca_pedido[indice_lancamento].nome_prod[t_nome_prod] = '\0'; //torna o ultimo caractere de 'nome_prod' '\0' (vazio)
            lanca_pedido[indice_lancamento].custo_prod = menu[indice_cod_prod].custo_prod; //guarda o 'custo_prod' no lancamento

            /*INTERACAO COM O USUARIO*/
            //inserindo 'qtd' do produto desejado
            printf("\nInsira a quantidade desejada (0 para voltar): ");
            scanf("%i", &lanca_pedido[indice_lancamento].qtd);
            limpar_buffer(); //limpa o ENTER deixado pelo 'scanf'

            if (lanca_pedido[indice_lancamento].qtd == 0) //se o usuario escolher por retornar a tela inicial
            {
                limpar_lancamento(indice_lancamento, false); //limpa o que tiver registrado até o momento sem precisar subir o próximo
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nVoltando para a TELA INICIAL...\n");
                printf("\nPressione qualquer tecla para voltar...");
                voltar = true; //fara com que termine a execucao do submodulo e volte a tela inicial
                getch(); //pausa a tela para leitura do usuario
            }
            else if (lanca_pedido[indice_lancamento].qtd < min_qtd || lanca_pedido[indice_lancamento].qtd > max_qtd) //se a quantidade deseja estiver fora do limite definido
            {
                limpar_lancamento(indice_lancamento, false); //limpa o que tiver registrado até o momento sem precisar subir o próximo
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nLimite minimo por lancamento  = %i.", min_qtd);
                printf("\nLimite maximo por lancamento  = %i.\n", max_qtd);
                printf("\nPressione qualquer tecla para voltar...");
                getch(); //pausa a tela para leitura do usuario
            }
            else
            {
                lanca_pedido[indice_lancamento].subtotal = lanca_pedido[indice_lancamento].custo_prod * lanca_pedido[indice_lancamento].qtd; //guarda o subtotal do lancamento

                do
                {
                    system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                    mostrar_lancamento(indice_lancamento); //imprime o lancamento que o cliente acabou de escrever

                    /*INTERACAO COM O USUARIO*/
                    //apresentara as opcoes do usuario na tela
                    printf("\nConfirmar lancamento?\n");
                    printf(  "0 - nao\n");
                    printf(  "1 - sim\n");
                    conf_lancamento = getch(); //capta qual eh a opcao escolhida pelo usuario

                    switch (conf_lancamento) //encaminha o usuario para opcao escolhida
                    {
                        case '0': //opcao de cancelar lancamento
                            limpar_lancamento(indice_lancamento, false); //limpa o que tiver registrado até o momento sem precisar subir o próximo
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nVoltando para a REGISTRO DE lancamentos...\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        case '1': //opcao de confirmar lancamento
                            calcular_pgto_total(); //calcula o novo total do pedido
                            ++indice_lancamento; //vai para o proximo indice de um lancamento novo
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nlancamento REGISTRADO!\n");
                            printf("\nPressione qualquer tecla para continuar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        default:
                            break;
                    }
                } while (conf_lancamento != '0' && conf_lancamento != '1');
                /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
            }
        }
    } while (! voltar);
    /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/
}

void rm_lancamento (void)
/*Objetivo da funcao
Realizar o processo de remocao de lancamentos no pedido.*/
{
    //funcoes utilizadas
    int  getch(void);
    void mostrar_pedido(void);
    void limpar_buffer(void);
    void limpar_lanca_pedido(void);
    void limpar_pgto_pedido(void);
    void mostrar_lancamento(int indice_ref);
    void limpar_lancamento(int indice_lancamentoRef, bool subir);

    //variaveis locais
    int  indice_remover;
    bool voltar;
    char conf_rm;

    if (indice_lancamento == 0) //se nao houver lancamentos no pedido
    {
        system("clear || cls"); //limpa a tela para uma sensacao de pop-up
        printf("\nNao ha lancamentos. Portanto, nao e possivel REMOVER lancamentos.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        do
        {
            system("clear || cls"); //limpa a tela para sensacao de uma nova pagina
            printf("\n>>>REMOCAO DE LANCAMENTOS\n"); //titulo da pagina
            mostrar_pedido(); //apresentar na tela todas as informacoes do pedido ate o momento
            voltar = false; //por padrao ele nao permite voltar para a tela inicial

            /*INTERACAO COM O USUARIO*/
            //inserindo indice do lancamento a ser removido
            printf("\nInsira o numero da linha (i) que deseja remover\n");
            printf("(0 para voltar/ -1 para remover tudo): ");
            scanf("%i", &indice_remover);
            limpar_buffer(); //limpa o ENTER deixado pelo 'scanf'

            if (indice_remover == 0) //se o usuario escolher por retornar a tela inicial
            {
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nVoltando para a TELA INICIAL...\n");
                printf("\nPressione qualquer tecla para voltar...");
                voltar = true; //fara com que termine a execucao do submodulo e volte a tela inicial
                getch(); //pausa a tela para leitura do usuario
            }
            else if (indice_remover < 0) //se o usuarui escolher por remover todos os lancamentos
            {
                do
                {
                    system("clear || cls"); //limpa a tela para uma sensacao de pop-up

                    /*INTERACAO COM O USUARIO*/
                    //apresentara as opcoes do usuario na tela
                    printf("\n>>>ATENCAO!!!\n");
                    printf("\nTem certeza que deseja REMOVER todos os lancamentos?\n");
                    printf("(0) - nao\n");
                    printf("(1) - sim\n");
                    conf_rm = getch(); //capta qual eh a opcao escolhida pelo usuario

                    switch(conf_rm) //encaminha o usuario para opcao escolhida
                    {
                        case '0': //opcao de nao remocao total dos lancamentos
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nRemocao total CANCELADA.\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        case '1': //opcao de remocao total dos lancamentos
                            limpar_lanca_pedido();
                            limpar_pgto_pedido();
                            voltar = true; //fara com que termine a execucao do submodulo e volte a tela inicial
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nRemocao total REALIZADA.\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        default:
                            break;
                    }
                } while (conf_rm != '0' && conf_rm != '1');
                /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
            }
            else if (indice_remover - 1 >= indice_lancamento) //se o usuario digitar um indice invalido
            {
                system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                printf("\nNao ha lancamento no INDICE digitado.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); //pausa a tela para leitura do usuario
            }
            else
            {
                do
                {
                    system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                    printf("\nO seguinte lancamento sera REMOVIDO.\n");
                    mostrar_lancamento(indice_remover - 1); //imprime o lancamento indicado pelo usuario

                    /*INTERACAO COM O USUARIO*/
                    //apresentacao das opcoes ao usuario
                    printf("\nConfirmar REMOCAO (0 - nao / 1 - sim):\n");
                    conf_rm = getch(); //capta qual eh a opcao escolhida pelo usuario

                    switch(conf_rm) //encaminha o usuario para opcao escolhida
                    {
                        case '0': //opcao de cancelar remocao
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nlancamento NAO removido.\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        case '1': //opcao de confirmar remocao
                            limpar_lancamento(indice_remover - 1, true); //remove o lancamento indicado e sobe os seguintes
                            system("clear || cls"); //limpa a tela para uma sensacao de pop-up
                            printf("\nlancamento REMOVIDO.\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); //pausa a tela para leitura do usuario
                            break;

                        default:
                            break;
                    }
                } while (conf_rm != '0' && conf_rm != '1');
                /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
            }
        } while (! voltar);
        /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
        faca-o terminar a execucao com voltar = true*/
    }
}

void confirmar_pedido (void)
/*Objetivo da funcao
Verifica se ha lancamentos, e em caso positivo, leva o usuario para o modulo 'cobranca'.*/
{
    //funcoes utilizadas
    int  getch(void);
    void cobranca(void);

    system("clear || cls"); //limpa a tela para sensacao de pop-up

    if(indice_lancamento == 0) //se nao houver lancamentos realizados
    {
        printf("\nNao ha itens lancados no pedido.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        printf("Entrando em FORMAS DE pagamento...\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch(); //pausa a tela para leitura do usuario
        cobranca(); //direciona para o modulo 'cobranca'
    }
}

//--------------------------------------
/**MODULO COBRANCA**/
//--------------------------------------

void cobranca (void)
{
    //funcoes utilizadas
    void mostrar_pedido(void);
    int  getch(void);
    bool pgto_dinheiro(void);
    bool pgto_cheque(void);
    bool pgto_debito(void);
    bool pgto_credito(void);

    //variaveis locais
    char opcao;
    bool voltar, conf_pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>FORMA DE pagamento\n"); //imprime o titulo da pagina
        mostrar_pedido(); //imprime na tela todas as informacoes do pedido ate o momento
        voltar = false; //por padrao ele nao permite voltar para a tela inicial
        conf_pgto = false; //por padrao ele considera que o pagamento nao foi realizado

        /*INTERACAO COM O USUARIO*/
        //apresentacao das opcoes ao usuario
        printf("\nOpcoes:\n");
        printf(  "(0) - Voltar para o pedido\n");
        printf(  "(1) - Dinheiro\n");
        printf(  "(2) - Cheque\n");
        printf(  "(3) - Cartao de Debito\n");
        printf(  "(4) - Cartao de Credito\n");
        opcao = getch(); //capta qual eh a opcao escolhida pelo usuario

        switch(opcao) //encaminha o usuario para opcao escolhida
        {
            case '0': //opcao de voltar para tela inicial
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nVoltando para a TELA INICIAL...\n");
                printf("\nPressione qualquer tecla para voltar...");
                voltar = true;
                getch(); //pausa a tela para leitura do usuario
                break;
            case '1': //opcao de pagar em dinheiro
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_dinheiro();
                break;
            case '2': //opcao de pagar em cheque
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_cheque();
                break;
            case '3': //opcao de pagar com cartao de debito
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em DEBITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_debito();
                break;
            case '4': //opcao de pagar com cartao de credito
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em CREDITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_credito();
                break;
            default:
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); //pausa a tela para leitura do usuario
                break;
        }
        if (conf_pgto)
            voltar = true;
    } while (! voltar);
}

//--------------------------------------
/**SUB-MODULOS DE COBRANCA**/
//--------------------------------------

bool pgto_dinheiro (void) //revisar
{
    //funcoes utilizadas
    void limpar_buffer(void);
    int  getch(void);
    bool FinalizarPedido(void);

    //variaveis locais
    float ValorRecebido, Troco;
    char conf_pgto;
    bool voltar, Pgto;

    do
    {
        voltar = false;
        Pgto = false;

        system("clear || cls");
        printf("\n>>>pagamento EM DINHEIRO\n");
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total);
        printf("\nInsira o dinheiro (digite -1 para voltar): ");
        scanf("%f", &ValorRecebido);
        limpar_buffer();

        if (ValorRecebido < 0)
        {
            ValorRecebido = '\0';
            voltar = true;
            system("clear || cls");
            printf("Voltando para FORMAS DE pagamento...\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else if (ValorRecebido < pgto_pedido.total)
        {
            ValorRecebido = '\0';
            system("clear || cls");
            printf("\nValor insuficiente.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else
        {
            Troco = ValorRecebido - pgto_pedido.total;

            do
            {
                system("clear || cls");
                printf("\nValor entregue: %.2f\n", ValorRecebido);
                printf("\nValor do troco a ser recebido: %.2f\n", Troco);
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                conf_pgto = getch();
            } while (conf_pgto != '0' && conf_pgto != '1');

            if (conf_pgto == '0')
            {
                Troco = '\0';
                ValorRecebido = '\0';
                conf_pgto = '\0';
                system("clear || cls");
                printf("\nVoltando para pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
            }
            else
            {
                pgto_pedido.n_pedido = n_pedido_atual;
                pgto_pedido.forma_pgto = '1';
                pgto_pedido.entregue = false;
                strcpy(pgto_pedido.n_cartao, "0000000000000000");
                FinalizarPedido();
                Pgto = true;
                voltar = true;
            }
        }
    } while (! voltar);

    if (Pgto)
        return true;
    else
        return false;
}

bool pgto_cheque (void) //revisar
{
    //funcoes utilizadas
    void limpar_buffer(void);
    int  getch(void);
    void FinalizarPedido(void);

    //variaveis locais
    float ValorRecebido;
    char conf_pgto;
    bool voltar, Pgto;

    do
    {
        voltar = false;
        Pgto = false;

        system("clear || cls");
        printf("\n>>>pagamento EM CHEQUE\n");
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total);
        printf("\nInsira o valor do cheque (digite -1 para voltar): ");
        scanf("%f", &ValorRecebido);
        limpar_buffer();

        if (ValorRecebido < 0)
        {
            ValorRecebido = '\0';
            voltar = true;
            system("clear || cls");
            printf("Voltando para FORMAS DE pagamento...\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
        }
        else if (ValorRecebido != pgto_pedido.total)
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
                conf_pgto = getch();
            } while (conf_pgto != '0' && conf_pgto != '1');

            if (conf_pgto == '0')
            {
                ValorRecebido = '\0';
                conf_pgto = '\0';
                system("clear || cls");
                printf("\nVoltando pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
            }
            else
            {
                pgto_pedido.n_pedido = n_pedido_atual;
                pgto_pedido.forma_pgto = '2';
                pgto_pedido.entregue = false;
                strcpy(pgto_pedido.n_cartao, "0000000000000000");
                FinalizarPedido();
                Pgto = true;
                voltar = true;
            }
        }
    } while (! voltar);

    if (Pgto)
        return true;
    else
        return false;
}

bool pgto_debito (void) //falta fazer
{
    printf("\nVoce escolheu DEBITO.\n");
    return false;
}

bool pgto_credito (void) //falta fazer
{
    printf("\nVoce escolheu CREDITO.\n");
    return false;
}

//--------------------------------------
/**FUNCOES AUXILIARES**/
//--------------------------------------

///Funcoes de limpeza

void Limparmenu (void)
/*Objetivo da funcao
Limpar qualquer "sujeira" da variavel 'menu'*/
{
    for (int i = 0; i < max_menu; ++i) //passa por todas as linhas da variavel 'menu'
    {
        menu[i].cod_prod = '\0'; //preenche '\0' (vazio) em cod_prod
        for (int ii = 0; ii < t_nome_prod; ++ii) //passa por todos os caracteres de 'nome_prod'
            menu[i].nome_prod[ii] = '\0'; //preenche '\0' (vazio) em todos os caracteres de 'nome_prod'
        menu[i].custo_prod = '\0'; //preenche '\0' (vazio) em custo_prod
    }
}

void limpar_lanca_pedido (void)
/*Objetivo da funcao
Limpar todos os lancamentos de 'lanca_pedido'*/
{
    void limpar_lancamento(int indice_lancamento, bool subir);

    for (int i = 0; i < indice_lancamento; ++i)
        limpar_lancamento(i, false);

    indice_lancamento = 0;
}

void limpar_pgto_pedido (void)
/*Objetivo da funcao
Limpar 'pgto_pedido'*/
{
    pgto_pedido.n_pedido = '\0'; //preenche '\0' (vazio) em N_pedido

    pgto_pedido.total = '\0'; //preenche '\0' (vazio) em total

    pgto_pedido.forma_pgto = '\0'; //preenche '\0' (vazio) em forma_pgto

    for (int i = 0; i < t_n_cartao; ++i) //passa por todos os caracteres de N_Cartao
        pgto_pedido.n_cartao[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de N_Cartao

    for (int i = 0; i < t_nome_cliente; ++i) //passa por todos os caracteres de nome_cliente
        pgto_pedido.nome_cliente[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de nome_cliente
}

void limpar_lancamento (int indice_lancamentoRef, bool subir)
/*Objetivo da funcao
- Limpar um lancamento especifico do pedido;
- Chama a funcao 'subir_lancamento' se o segundo argumento for 'true'
  (usado quando se quer limpar um lancamento especifico apenas;*/
{
    //funcoes utilizadas
    void subirlancamento(int indice_ref);
    void calcular_pgto_total(void);

    lanca_pedido[indice_lancamentoRef].n_pedido = '\0'; //preenche '\0' (vazio) em N_pedido

    lanca_pedido[indice_lancamentoRef].cod_prod = '\0'; //preenche '\0' (vazio) em cod_prod

    for (int i = 0; i < t_nome_prod; ++i) //passa por todos os caracteres de nome_prod
        lanca_pedido[indice_lancamentoRef].nome_prod[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de nome_prod

    lanca_pedido[indice_lancamentoRef].custo_prod = '\0'; //preenche '\0' (vazio) em custo_prod

    lanca_pedido[indice_lancamentoRef].qtd = '\0'; //preenche '\0' (vazio) em qtd

    lanca_pedido[indice_lancamentoRef].subtotal = '\0'; //preenche '\0' (vazio) em subtotal

    if (subir) //caso se queira subir os lancamentos abaixo
    {
        while (lanca_pedido[indice_lancamentoRef + 1].cod_prod != '\0') //o ciclo roda ate que a proxima linha seja '\0' (vazio)
        {
            subirlancamento(indice_lancamentoRef + 1); //sobe o proximo lancamento para a o 'indice_lancamento'
            ++indice_lancamentoRef; //vai para o proximo indice de lancamento
        }

        indice_lancamento = indice_lancamentoRef; //atualiza o indice do proximo lancamento a realizar

        calcular_pgto_total(); //calcula o 'total' do pedido atual
    }
}

void subirlancamento (int indice_ref)
/*Objetivo da funcao
Copia os dados do indice referente para um indice anterior e apaga os dados do indice referente*/
{
    //funcoes utilizadas
    void limpar_lancamento(int indice_lancamentoRef, bool subir);

    int IndiceNovo = indice_ref - 1; //cria uma variavel que guardara o indice anterior (onde que ser gravar os dados do indice referente)

    lanca_pedido[IndiceNovo].n_pedido = lanca_pedido[indice_ref].n_pedido; //copia o N_pedido do indice referente para o indice anterior

    lanca_pedido[IndiceNovo].cod_prod = lanca_pedido[indice_ref].cod_prod; //copia o cod_prod do indice referente para o indice anterior

    strcpy(lanca_pedido[IndiceNovo].nome_prod, lanca_pedido[indice_ref].nome_prod); //copia o nome_prod do indice referente para o indice anterior

    lanca_pedido[IndiceNovo].custo_prod = lanca_pedido[indice_ref].custo_prod; //copia o custo_prod do indice referente para o indice anterior

    lanca_pedido[IndiceNovo].qtd = lanca_pedido[indice_ref].qtd; //copia o qtd do indice referente para o indice anterior

    lanca_pedido[IndiceNovo].subtotal = lanca_pedido[indice_ref].subtotal; //copia o subtotal do indice referente para o indice anterior

    limpar_lancamento(indice_ref, false); //limpa os dados do indice referente que foram copiados para o indice anterior
}

///Funcoes de impressao

void mostrar_pedido (void)
/*Objetivo da funcao
Imprimir na tela as informacoes ate o momento do pedido*/
{
    //funcoes utilizadas
    void Imprimen_pedido(void);
    void ImprimeCabecalhoPedido(void);
    void Imprimelancamento(int indice_ref);
    void ImprimeRodapePedido(void);

    if(indice_lancamento == 0) //se 'indice_l' for igual a zero quer dizer que ainda nao houveram lancamentos no pedido
    {
        printf("\n_pedido No %i", n_pedido_atual); //informacoes apresentadas caso nao haja lancamentos no pedido
        printf("\nNao ha lancamentos no pedido.\n");
    }
    else //apresentacao das informacoes do pedido caso haja lancamentos
    {
        printf("\n====================SEU PEDIDO===================="); //titulo do quadro

        Imprimen_pedido(); //imprime o nº do pedido

        ImprimeCabecalhoPedido(); //imprime o cabecalho de lancamentos

        for(int i = 0; i < indice_lancamento; ++i) //ciclo que fara a impressao de lancamento por lancamento a partir da variavel 'l_pedido'
            Imprimelancamento(i); //imprime um lancamento

        ImprimeRodapePedido(); //imprime o total do pedido

        printf("==================================================\n"); //rodape do quadro
    }
}

void Imprimen_pedido (void)
{
    printf("\n                                  Pedido No ");
    //dependendo do valor de 'n_pedido_atual' eh necessario uma quantidade de espacos para alinhar ao quadro
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
        printf("%i\n"     , n_pedido_atual); //valor maximo do 'n_pedido_atual' eh de 100000
}

void ImprimeCabecalhoPedido (void)
{
    printf("\ni   cod_prod Nome do produto   Custo  qtd  subtotal\n"); //cabecalho da tabela
}

void Imprimelancamento (int indice_ref)
{
    int i = indice_ref; //cria uma variavel que guardara o indica do lancamento que se deseja imprimir

    //Indice
    if (indice_ref + 1 < 10)
        printf("%i   ", indice_ref + 1);
    else if (indice_ref + 1 < 100)
        printf("%i  ", indice_ref + 1);
    else
        printf("%i ", indice_ref + 1);

    //cod_prod
    printf("%i      ",   lanca_pedido[i].cod_prod); //imprime cod_prod (como tem sempre dois caracteres nao eh necessario quantidade de espacoes diferentes

    //Nome do produto
    for (int ii = 0; ii < t_nome_prod; ++ii) //ciclo que passa por todos os caracteres de nome_prod
    {
        if (lanca_pedido[i].nome_prod[ii] != '\0') //se o caractere de nome_prod for diferente de vazio ele imprime na tela
            printf("%c", lanca_pedido[i].nome_prod[ii]);
        else //se for vazio ele imprime um espaco
            printf(" ");
    }
    printf(" "); //um espaco para separacao da proxima informacao

    //Custo
    //dependendo do valor de custo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[i].custo_prod < 10)
        printf("   %.2f ", lanca_pedido[i].custo_prod);
    else if (lanca_pedido[i].custo_prod < 100)
        printf("  %.2f " , lanca_pedido[i].custo_prod);
    else
        printf(" %.2f "  , lanca_pedido[i].custo_prod); //valor maximo de custo_prod eh de 100.00

    //qtd
    //dependendo do valor de qtd eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[i].qtd < 10)
        printf("   %i ", lanca_pedido[i].qtd);
    else if (lanca_pedido[i].qtd < 100)
        printf("  %i " , lanca_pedido[i].qtd);
    else
        printf(" %i "  , lanca_pedido[i].qtd); //valor maximo de qtd eh de 100

    //subtotal
    //dependendo do valor de subtotal eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[i].subtotal < 10)
        printf("     %.2f\n", lanca_pedido[i].subtotal);
    else if (lanca_pedido[i].subtotal < 100)
        printf("    %.2f\n" , lanca_pedido[i].subtotal);
    else if (lanca_pedido[i].subtotal < 1000)
        printf("   %.2f\n"  , lanca_pedido[i].subtotal);
    else if (lanca_pedido[i].subtotal < 10000)
        printf("  %.2f\n"   , lanca_pedido[i].subtotal);
    else
        printf(" %.2f\n"    , lanca_pedido[i].subtotal); //valor maximo de subtotal eh de 10,000.00
}

void ImprimeRodapePedido (void)
{
    printf("\n                                   total ");
    //dependendo do valor de total eh necessario uma quantidade de espacos para alinhar ao quadro
    if (pgto_pedido.total < 10)
        printf("     %.2f\n", pgto_pedido.total);
    else if (pgto_pedido.total < 100)
        printf("    %.2f\n" , pgto_pedido.total);
    else if (pgto_pedido.total < 1000)
        printf("   %.2f\n"  , pgto_pedido.total);
    else if (pgto_pedido.total < 10000)
        printf("  %.2f\n"   , pgto_pedido.total);
    else if (pgto_pedido.total < 100000)
        printf(" %.2f\n"    , pgto_pedido.total);
    else
        printf("%.2f\n"     , pgto_pedido.total); //valor maximo de Ttoal eh de 100,000.00
}

void mostrar_menu (void)
/*Objetivo da funcao
Imprimir na tela os produtos disponiveis no 'menu'*/
{
    if (menu[0].cod_prod == '\0') //se o primeiro cod_prod for '\0' (vazio), entao nao ha itens no 'menu'
        printf("\nNao ha itens no menu.\n");
    else //apresentacao das informacoes do 'menu' caso haja produtos disponiveis
    {
        printf("\n============menu============="); //titulo da quadro
        printf("\nCodigo Nome do produto  Custo\n"); //cabecalho da tabela
        for (int i = 0; menu[i].cod_prod != '\0'; ++i) //ciclo que fara a impressao dos produtos disponiveis
        {
            //cod_prod
            printf("%i     ", menu[i].cod_prod); //imprime cod_prod (como tem sempre dois caracteres nao eh necessario quantidade de espacoes diferentes

            //nome_prod
            for (int ii = 0; ii < t_nome_prod; ++ii) //ciclo que passa por todos os caracteres de nome_prod
            {
                if (menu[i].nome_prod[ii] != '\0') //se o caractere de nome_prod for diferente de vazio ele imprime na tela
                    printf("%c", menu[i].nome_prod[ii]);
                else //se for vazio ele imprime um espaco
                    printf(" ");
            }
            printf(" "); //um espaco para separacao da proxima informacao

            //Custo
            //dependendo do valor de custo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
            if (menu[i].custo_prod < 10)
                printf("  %.2f\n", menu[i].custo_prod);
            else if (menu[i].custo_prod < 100)
                printf(" %.2f\n" , menu[i].custo_prod);
            else
                printf("%.2f\n"  , menu[i].custo_prod); //valor maximo de custo_prod eh de 100.00
        }
        printf("=============================\n"); //rodape do quadro
    }
}

void mostrar_lancamento (int indice_ref)
/*Objetivo da funcao
Imprimir na tela um lancamento especifico*/
{
    //funcoes utilizadas
    void ImprimeCabecalhoPedido(void);
    void Imprimelancamento(int indice_ref);

    ImprimeCabecalhoPedido(); //imprime o cabecalho de lancamentos

    Imprimelancamento(indice_ref); //imprime o lancamento indicado
}

///Funcoes de iniciacao ou gravacao

void import_menu (void)
/*Objetivo da funcao
Carregar do arquivo "menu_produtos.DAT" quais sao os produtos disponiveis e guardar na variavel 'menu'*/
{
    //funcoes utilizadas
    void Limparmenu(void);

    Limparmenu(); //limpar qualquer "sujeira" na variavel 'menu'

    FILE *Pontmenu; //criacao de uma variavel ponteiro para o arquivo que contem os produtos disponiveis

    Pontmenu = fopen(menu_dat, "r"); //leitura do arquivo onde se encontram os produtos disponiveis

    if (Pontmenu == NULL)
        printf("\nArquivo nao pode ser aberto\n"); //caso ocorra problemas na leitura do arquivo
    else
    {
        int i = 0; //indice da variavel vetor 'menu'

        do
        {
            fscanf(Pontmenu, "%i\t", &menu[i].cod_prod); //grava o 'cod_prod' que se encontra na linha (i + 1) do arquivo
            fscanf(Pontmenu, "%s\t",  menu[i].nome_prod); //grava o 'nome_prod' que se encontra na linha (i + 1) do arquivo /nao pode haver espacos na string
            fscanf(Pontmenu, "%f\n", &menu[i].custo_prod); //grava o 'custo_prod' que se encontra na linha (i + 1) do arquivo
            ++i;
        } while (menu[i - 1].cod_prod != '\0' && i < max_menu); //ira gravar em cada linha da variavel 'menu' os dados do arquivo ate que a ultima gravacao seja '\0' (vazio)

        fclose(Pontmenu); //fecha o arquivo
    }
}

void novo_n_pedido (void)
/*Objetivo da funcao
- Ler no arquivo qual foi o ultimo nº do pedido aberto;
- Guardar o proximo nº da variavel 'n_pedido_atual';
- Gravar no arquivo o 'n_pedido_atual'.*/
{
    FILE *Pontn_pedido; //criacao de uma variavel ponteiro para o arquivo que contem o ultimo nº de pedido aberto

    Pontn_pedido = fopen(n_pedido_dat, "r"); //leitura do arquivo onde se encontra o ultimo nº de pedido aberto

    if (Pontn_pedido == NULL)
        printf("\nArquivo nao pode ser aberto\n"); //caso ocorra problemas na leitura do arquivo
    else
    {
        fscanf(Pontn_pedido, "%i", &n_pedido_atual); //grava o nº do ultimo pedido aberto em 'n_pedido_atual'
        ++n_pedido_atual; //acrescenta +1 para se tornar o proximo nº de pedido sendo o atual

        fclose(Pontn_pedido); //fecha o arquivo

        Pontn_pedido = fopen(n_pedido_dat, "w"); //recriacao do arquivo onde se encontrara o ultimo nº de pedido aberto

        fprintf(Pontn_pedido, "%i", n_pedido_atual); //grava o nº de pedido que acabou de abrir no arquivo para que este seja o ultimo nº de pedido aberto
    }
    fclose(Pontn_pedido); //fecha o arquivo
}

void UploadPedido (void)
/*Objetivo da funcao
Gravar nos arquivos "lancamentos_pedidos.DAT" e "pagamentos_pedidos.DAT os dados do pedido realizado"*/
{
    //funcoes utilizadas
    void limpar_lanca_pedido(void);
    void limpar_pgto_pedido(void);
    void novo_n_pedido(void);

    FILE *Pontlancamentos; //cria uma variavel ponteiro para o arquivo que guarda os lancamentos confirmados
    FILE *Pontpagamentos; //cria uma variavel ponteiro para o arquivo que guarda os pagamentos realizados

    Pontlancamentos = fopen(lancamentos_dat, "a"); //abre para anexar lancamentos confirmado do pedido realizado

    if (Pontlancamentos == NULL)
        printf("\nArquivo nao pode ser aberto\n"); //caso ocorra problemas na leitura do arquivo
    else
    {
        for (int i = 0; i < indice_lancamento; i++) //passar por todos os lancamentos do pedido
        {
            fprintf(Pontlancamentos, "%i\t",   lanca_pedido[i].n_pedido);   //grava o nº do pedido
            fprintf(Pontlancamentos, "%i\t",   lanca_pedido[i].cod_prod);   //grava o codigo do produto
            fprintf(Pontlancamentos, "%s\t",   lanca_pedido[i].nome_prod);  //grava o nome do produto
            fprintf(Pontlancamentos, "%.2f\t", lanca_pedido[i].custo_prod); //grava o custo unitario
            fprintf(Pontlancamentos, "%i\t",   lanca_pedido[i].qtd);       //grava a quantidade pedida
            fprintf(Pontlancamentos, "%.2f\n", lanca_pedido[i].subtotal);  //grava o sub-total do lancamento
        }
    }

    fclose(Pontlancamentos); //fecha o arquivo

    Pontpagamentos = fopen(pagamentos_dat, "a"); //abre para anexar pagamento realizado do pedido

    if (Pontpagamentos == NULL)
        printf("\nArquivo nao pode ser aberto\n"); //caso ocorra problemas na leitura do arquivo
    else
    {
        fprintf(Pontpagamentos, "%i\t",   pgto_pedido.n_pedido);     //grava o nº do pedido
        fprintf(Pontpagamentos, "%.2f\t", pgto_pedido.total);       //grava o total do pedido
        fprintf(Pontpagamentos, "%c\t",   pgto_pedido.forma_pgto);   //grava a forma de pagamento
        fprintf(Pontpagamentos, "%s\t",   pgto_pedido.n_cartao);     //grava o numero do cartao
        fprintf(Pontpagamentos, "%s\n",   pgto_pedido.nome_cliente); //grava o nome do cliente
    }

    fclose(Pontpagamentos); //fecha o arquivo

    limpar_lanca_pedido(); //limpa todos os lancamento da variavel 'l_pedido'
    limpar_pgto_pedido();  //limpa todos os dados de 'p_pedido'
    novo_n_pedido();       //gera um novo nº de pedido
}

///Funcoes diversas

int buscar_cod_prod (int cod_prod)
/*Objetivo da funcao
Retorna o valor do indice do 'cod_prod' referido, caso nao encontre sera retornado -1.*/
{
    //variaveis locais
    int indice_cod_prod = -1;
    int i = 0;

    if (cod_prod >= min_cod_prod && cod_prod <= max_cod_prod) //se o 'cod_prod' referido estiver no limite definido
    {
        while(indice_cod_prod == -1 && menu[i].cod_prod != '\0')
        {
            if(cod_prod != menu[i].cod_prod) //caso o cod_prod do 'menu' seja diferente do 'cod_prod' procurado
                ++i; //vai para o proximo indice
            else
                indice_cod_prod = i; //copia o indice na variavel que sera retornada
        }
        /*O ciclo permanecera rodando ate que se encontre o 'cod_prod' no 'menu' ou passe por todos os
        produtos disponiveis.*/
    }

    return indice_cod_prod; //retorna o indice do 'cod_prod' procurado ou -1 se nao tiver sido encontrado
}

void calcular_pgto_total (void)
/*Objetivo da funcao
Calcular o total de 'p_pedido' ate o momento*/
{
    pgto_pedido.total = 0; //zera o total do pedido

    for (int i = 0; i < indice_lancamento; ++i) //somara todos os subtotal de 'l_pedido' em total de todas as linhas preenchidas
        pgto_pedido.total += lanca_pedido[i].subtotal;
}

void FinalizarPedido (void) //revisar falta completar
{
    //funcoes utilizadas
    void getstring(char linha[]);
    void UploadPedido(void);
    int  getch (void);

    //variaveis locais
    bool Terminar

    do
    {
        Terminar = false;

        system("clear ||cls");

        printf("\n>>>FINALIZACAO DE PEDIDO\n");

        /*INTERACAO COM O USUARIO*/
        printf("\nDigite seu NOME: ");
        getstring (pgto_pedido.nome_cliente);

        if (strlen(pgto_pedido.nome_cliente) < 3)
        {
            system("clear ||cls"); //limpa a tela para sensacao de pop-up
            printf("\nDigite um nome com pelo menos 3 caracteres.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa para leitura da mensagem pelo usuario
        }
        else
        {
            UploadPedido(); //limpa a tela para sensacao de pop-up
            system("clear ||cls");
            printf("\n_pedido CONFIRMADO!\n");
            printf("\npagamento REALIZADO!\n");
            printf("\nAGRADECEMOS a sua compra!");
            Terminar = true; //termina o ciclo
            getch(); //pausa para leitura da mensagem pelo usuario
        }
    } while (! Terminar);
}


//--------------------------------------
/**FUNCOES BASICAS**/
//--------------------------------------

int getch (void) //LINUX - criacao da funcao 'getch' pois o sistema nao tem a biblioteca 'conio.h'
/*Objetivo da funcao
Capturar o proximo caractere que o usuario digitar*/
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

void limpar_buffer (void) //funcao criada para limpar o buffer do teclado que funciona em WINDOWS e LINUX
/*Objetivo da funcao
Limpar qualquer "sujeira" que tiver no buffer do teclado. So funciona se realmente houver algo no buffer
Deve-se utilizar apos a funcao 'scanf'*/
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getstring (char linha[]) //funcao que funciona melhor que gets
/*Objetivo da funcao
Guarda em uma variavel do tipo string (char com vetor) o que o usuario digitar ate ele apertar ENTER*/
{
    char c; //caractere que ira compor a string
    int i = 0; //indice do caractere

    do {
        c = getchar (); //guarda o caractere digitado na variavel 'c'
        linha [i] = c; //guarda o valor da variavel 'c' na posicao da string indicada
        ++i; //vai para a proxima posicao da string
    } while (c != '\n'); //ciclo rodara enquanto a variavel 'c' nao for ENTER

    linha [i - 1] = '\0'; //substitui o ENTER por '\0' (vazio) para finalizar a string
}

//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: Drive-thru Mickey & Donald

Descricao:
Projeto para avaliacao de desempenho na discipĺina de "Logica de programacao 1"
do curso "Tecnologo de Analise e Desenvolvimento de Sistemas" no Instituto Federal
de Sao Paulo - campus Sao Paulo

Desenvolvedor(es):
Felippe Marques da Silva de Almeida SP3056686
Leandro Paiva Higa                  SP3052648

Ultima atualizacao: 23-out.-2020 as 12h51
*/
