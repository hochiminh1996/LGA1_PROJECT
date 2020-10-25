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

Ultima atualizacao: 25-out.-2020 as 10h18
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
//#include <termios.h> //Linux (criacao de getch)
//#include <unistd.h> //Linux (criacao de getch)
#include <conio.h> //Windows (getch)
#include <string.h>

//--------------------------------------
/**CONSTANTES DO PROGRAMA**/
//--------------------------------------

///Tamanho de strings

const int t_nome_prod    = 15; //tamanho da string 'nome_prod'
const int t_n_cartao     = 17; //tamanho da string 'n_cartao'
const int t_nome_cliente = 21; //tamanho da string 'nome_cliente'

///Maximos e minimos

const int max_menu                  = 100; //maximo de itens no 'menu'
const int max_lancamentos           = 100; //maximo de itens no 'lanca_pedido'
const int min_cod_prod              =  10; //valor minimo de 'cod_prod'
const int max_cod_prod              =  99; //valor maximo de 'cod_prod'
const int min_qtd                   =   1; //valor minimo de 'qtd'
const int max_qtd                   = 100; //valor maximo de 'qtd'
const int max_pedidos_realizados    = 500;
const int max_pedidos_nao_entregues =  50;

///Endereço dos arquivos

const char menu_dat[]        = {"MENU.DAT"};        //nome do arquivo que se encontra os dados do 'menu'
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
    char  tipo_prod;              //tipo de produto (1 - hamburguer | 2 - acompanhamento | 3 - bebida | 4 - outros)
};

struct lancamento //estrutura para formar os lancamentos de um pedido
{
    int   n_pedido;               //n do pedido
    int   cod_prod;               //codigo do produto
    char  nome_prod[t_nome_prod]; //nome do produto (max caracteres: 14 | 15 = '\0')
    float custo_prod;             //custo do produto
    char  tipo_prod;              //tipo de produto (1 - hamburguer | 2 - acompanhamento | 3 - bebida | 4 - outros)
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

int    n_pedido_atual; //variavel que guardara o n do pedido que esta sendo montado

struct pagamento pedidos_realizados[max_pedidos_realizados] = {};

int qtd_pedidos_realizados;

struct pagamento pedidos_entregues[max_pedidos_realizados] = {};

int qtd_pedidos_entregues;

struct pagamento pedidos_nao_entregues[max_pedidos_nao_entregues] = {};

int qtd_pedidos_nao_entregues;

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
    //int  getch(void);
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
    //int  getch(void);
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
    void limpar_lancamento(int indice_lancamento_ref, bool subir);
    //int  getch(void);
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
        printf("\n>>>REGISTRO DE LANCAMENTOS\n"); //imprime titulo da pagina
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
            lanca_pedido[indice_lancamento].tipo_prod = menu[indice_cod_prod].tipo_prod; //guarda o 'tipo_prod' no lancamento

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
    //int  getch(void);
    void mostrar_pedido(void);
    void limpar_buffer(void);
    void limpar_lanca_pedido(void);
    void limpar_pgto_pedido(void);
    void mostrar_lancamento(int indice_ref);
    void limpar_lancamento(int indice_lancamento_ref, bool subir);

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
    //int  getch(void);
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
        printf("Entrando em FORMAS DE PAGAMENTO...\n");
        printf("\nPressione qualquer tecla para continuar...");
        getch(); //pausa a tela para leitura do usuario
        cobranca(); //direciona para o modulo 'cobranca'
    }
}

//--------------------------------------
/**MODULO COBRANCA**/
//--------------------------------------

void cobranca (void)
/*Objetivo da funcao
Apresentar as opcoes de pagamento do pedido para o usuario.*/
{
    //funcoes utilizadas
    void mostrar_pedido(void);
    //int  getch(void);
    bool pgto_dinheiro(void);
    bool pgto_cheque(void);
    bool pgto_debito(void);
    bool pgto_credito(void);
    void finalizar_pedido(void);

    //variaveis locais
    char opcao;
    bool voltar, conf_pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>FORMA DE PAGAMENTO\n"); //imprime o titulo da pagina
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
                voltar = true; //fara com que termine a execucao do modulo e volte a tela inicial
                getch(); //pausa a tela para leitura do usuario
                break;
            case '1': //opcao de pagar em dinheiro
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_dinheiro(); //direciona para o submodulo 'pgto_dinheiro'
                break;
            case '2': //opcao de pagar em cheque
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em CHEQUE...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_cheque(); //direciona para o submodulo 'pgto_cheque'
                break;
            case '3': //opcao de pagar com cartao de debito
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em DEBITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_debito(); //direciona para o submodulo 'pgto_debito'
                break;
            case '4': //opcao de pagar com cartao de credito
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nEntrando em pagamento em CREDITO...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); //pausa a tela para leitura do usuario
                conf_pgto = pgto_credito(); //direciona para o submodulo 'pgto_credito'
                break;
            default:
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); //pausa a tela para leitura do usuario
                break;
        }
        if (conf_pgto) //se o pagamento por confirmado
        {
            finalizar_pedido();  //direciona para o modulo 'finalizar_pedido'
            voltar = true; //fara com que termine a execucao do modulo e volte a tela inicial
        }
    } while (! voltar);
    /*O modulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/
}

//--------------------------------------
/**SUB-MODULOS DE COBRANCA**/
//--------------------------------------

bool pgto_dinheiro (void)
/*Objetivo da funcao
Realizar o processo de pagamento em dinheiro do pedido.*/
{
    //funcoes utilizadas
    void limpar_buffer(void);
    //int  getch(void);

    //variaveis locais
    float valor_recebido, troco;
    char  conf_pgto;
    bool  voltar, pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>pagamento EM DINHEIRO\n"); //imprime titulo da pagina
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total); //apresenta o valor a ser pago
        voltar = false; //por padrao ele nao permite voltar para o modulo 'cobranca'
        pgto = false; //por padrao ele considera que o pagamento nao foi realizado

        /*INTERACAO COM O USUARIO*/
        //inserindo valor recebido
        printf("\nInsira o dinheiro (digite 0 para voltar): ");
        scanf("%f", &valor_recebido);
        limpar_buffer(); //limpa o ENTER deixado pelo 'scanf'

        if (valor_recebido == 0) //se o usuario escolher por retornar ao modulo 'cobranca'
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
            getch(); //pausa a tela para leitura do usuario
        }
        else if (valor_recebido < pgto_pedido.total)
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("\nValor insuficiente.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa a tela para leitura do usuario
        }
        else
        {
            troco = valor_recebido - pgto_pedido.total; //calcula o troco a ser recebido pelo usuario

            do
            {
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nValor entregue: %.2f\n", valor_recebido); //apresenta o valor que o usuario entregou
                printf("\nValor do troco a ser recebido: %.2f\n", troco); //apresenta o calor que o usuario ira receber

                /*INTERACAO COM O USUARIO*/
                //apresentacao das opcoes ao usuario
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                conf_pgto = getch(); //capta qual eh a opcao escolhida pelo usuario

                switch (conf_pgto) //encaminha o usuario para opcao escolhida
                {
                    case '0': //opcao de cancelar o pagamento
                        system("clear || cls"); //limpa a tela para sensacao de pop-up
                        printf("\nVoltando para pagamento em DINHEIRO...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); //pausa a tela para leitura do usuario
                        break;

                    case '1': //opcao de confirmar o pagamento
                        pgto_pedido.n_pedido = n_pedido_atual; //grava o 'n_pedido'
                        pgto_pedido.forma_pgto = '1'; //grava a 'forma_pgto'
                        pgto_pedido.entregue = false; //informa que o pedido ainda não foi entregue
                        for (int i = 0; i < t_n_cartao - 1; ++i) //passa por todos os caracteres de 'n_cartao'
                            pgto_pedido.n_cartao[i] = '0'; //grava '0' em todos os espacos de 'n_cartao'
                        pgto_pedido.n_cartao[t_n_cartao - 1] = '\0'; //grava '\0' (vazio) na ultima posição de 'n_cartao'
                        pgto = true; //confirma o pagamento do pedido
                        voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
                        break;

                    default:
                        break;
                }
            } while (conf_pgto != '0' && conf_pgto != '1');
            /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
        }
    } while (! voltar);
    /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/

    if (pgto) //se o pagamento for confirmado
        return true; //retorna a confirmacao positiva
    else
        return false; //retorna a confirmacao negativa
}

bool pgto_cheque (void)
/*Objetivo da funcao
Realizar o processo de pagamento em cheque do pedido.*/
{
    //funcoes utilizadas
    void limpar_buffer(void);
    //int  getch(void);

    //variaveis locais
    float valor_recebido;
    char  conf_pgto;
    bool  voltar, pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>pagamento EM CHEQUE\n"); //imprime titulo da pagina
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total); //apresenta o valor a ser pago
        voltar = false; //por padrao ele nao permite voltar para o modulo 'cobranca'
        pgto = false; //por padrao ele considera que o pagamento nao foi realizado

        /*INTERACAO COM O USUARIO*/
        //inserindo valor recebido
        printf("\nInsira o valor do cheque (digite 0 para voltar): ");
        scanf("%f", &valor_recebido);
        limpar_buffer(); //limpa o ENTER deixado pelo 'scanf'

        if (valor_recebido == 0) //se o usuario escolher por retornar ao modulo 'cobranca'
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
            getch(); //pausa a tela para leitura do usuario
        }
        else if (valor_recebido != pgto_pedido.total)
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("\nValor nao corresponde.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa a tela para leitura do usuario
        }
        else
        {
            do
            {
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nValor entregue: %.2f\n", valor_recebido); //apresenta o valor que o usuario entregou

                /*INTERACAO COM O USUARIO*/
                //apresentacao das opcoes ao usuario
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                conf_pgto = getch(); //capta qual eh a opcao escolhida pelo usuario

                switch (conf_pgto) //encaminha o usuario para opcao escolhida
                {
                    case '0': //opcao de cancelar o pagamento
                        system("clear || cls"); //limpa a tela para sensacao de pop-up
                        printf("\nVoltando para pagamento em CHEQUE...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); //pausa a tela para leitura do usuario
                        break;

                    case '1': //opcao de confirmar o pagamento
                        pgto_pedido.n_pedido = n_pedido_atual; //grava o 'n_pedido'
                        pgto_pedido.forma_pgto = '2'; //grava a 'forma_pgto'
                        pgto_pedido.entregue = false; //informa que o pedido ainda não foi entregue
                        for (int i = 0; i < t_n_cartao - 1; ++i) //passa por todos os caracteres de 'n_cartao'
                            pgto_pedido.n_cartao[i] = '0'; //grava '0' em todos os espacos de 'n_cartao'
                        pgto_pedido.n_cartao[t_n_cartao - 1] = '\0'; //grava '\0' (vazio) na ultima posição de 'n_cartao'
                        pgto = true; //confirma o pagamento do pedido
                        voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
                        break;

                    default:
                        break;
                }
            } while (conf_pgto != '0' && conf_pgto != '1');
            /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
        }
    } while (! voltar);
    /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/

    if (pgto) //se o pagamento for confirmado
        return true; //retorna a confirmacao positiva
    else
        return false; //retorna a confirmacao negativa
}

bool pgto_debito (void)
/*Objetivo da funcao
Realizar o processo de pagamento em cartao de debito do pedido.*/
{
    //funcoes utilizadas
    void get_string(char linha[]);
    bool valida_cartao(char n_cartao[]);
    //int  getch(void);
    void esconder_n_cartao(void);

    //variaveis locais
    char n_cartao_digitado[t_n_cartao];
    char conf_pgto;
    bool voltar, pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>pagamento NO DEBITO\n"); //imprime titulo da pagina
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total); //apresenta o valor a ser pago
        voltar = false; //por padrao ele nao permite voltar para o modulo 'cobranca'
        pgto = false; //por padrao ele considera que o pagamento nao foi realizado

        /*INTERACAO COM O USUARIO*/
        //inserindo cartao
        printf("\nInsira o numero do cartao (digite 0 para voltar): ");
        get_string(n_cartao_digitado);

        if (n_cartao_digitado == 0) //se o usuario escolher por retornar ao modulo 'cobranca'
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
            getch(); //pausa a tela para leitura do usuario
        }
        else if (! valida_cartao(n_cartao_digitado))
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("\nNumero de cartao INVALIDO.\n");
            printf("\nCertifique-se de: ");
            printf("\n- Não inserir espacos entre os numeros;");
            printf("\n- Conter exatamente 16 algarismos;\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa a tela para leitura do usuario
        }
        else
        {
            do
            {
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nValor do pagamento: %.2f\n", pgto_pedido.total); //apresenta o valor que o usuario entregou

                /*INTERACAO COM O USUARIO*/
                //apresentacao das opcoes ao usuario
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                conf_pgto = getch(); //capta qual eh a opcao escolhida pelo usuario

                switch (conf_pgto) //encaminha o usuario para opcao escolhida
                {
                    case '0': //opcao de cancelar o pagamento
                        system("clear || cls"); //limpa a tela para sensacao de pop-up
                        printf("\nVoltando para pagamento no DEBITO...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); //pausa a tela para leitura do usuario
                        break;

                    case '1': //opcao de confirmar o pagamento
                        pgto_pedido.n_pedido = n_pedido_atual; //grava o 'n_pedido'
                        pgto_pedido.forma_pgto = '3'; //grava a 'forma_pgto'
                        pgto_pedido.entregue = false; //informa que o pedido ainda não foi entregue
                        strcpy(pgto_pedido.n_cartao, n_cartao_digitado); //grava o 'n_cartao' no 'pgto_pedido'
                        esconder_n_cartao(); //substitui os numeros do meio por '*'
                        pgto = true; //confirma o pagamento do pedido
                        voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
                        break;

                    default:
                        break;
                }
            } while (conf_pgto != '0' && conf_pgto != '1');
            /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
        }
    } while (! voltar);
    /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/

    if (pgto) //se o pagamento for confirmado
        return true; //retorna a confirmacao positiva
    else
        return false; //retorna a confirmacao negativa
}

bool pgto_credito (void)
/*Objetivo da funcao
Realizar o processo de pagamento em cartao de credito do pedido.*/
{
    //funcoes utilizadas
    void get_string(char linha[]);
    bool valida_cartao(char n_cartao[]);
    //int  getch(void);
    void esconder_n_cartao(void);

    //variaveis locais
    char n_cartao_digitado[t_n_cartao];
    char conf_pgto;
    bool voltar, pgto;

    do
    {
        system("clear || cls"); //limpa a tela para sensacao de nova pagina
        printf("\n>>>pagamento NO CREDITO\n"); //imprime titulo da pagina
        printf("\nValor total da compra: %.2f\n", pgto_pedido.total); //apresenta o valor a ser pago
        voltar = false; //por padrao ele nao permite voltar para o modulo 'cobranca'
        pgto = false; //por padrao ele considera que o pagamento nao foi realizado

        /*INTERACAO COM O USUARIO*/
        //inserindo cartao
        printf("\nInsira o numero do cartao (digite 0 para voltar): ");
        get_string(n_cartao_digitado);

        if (n_cartao_digitado[0] == '0' && n_cartao_digitado[1] == '\0') //se o usuario escolher por retornar ao modulo 'cobranca'
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("Voltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
            getch(); //pausa a tela para leitura do usuario
        }
        else if (! valida_cartao(n_cartao_digitado))
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("\nNumero de cartao INVALIDO.\n");
            printf("\nCertifique-se de: ");
            printf("\n- Não inserir espacos entre os numeros;");
            printf("\n- Conter exatamente 16 algarismos;\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa a tela para leitura do usuario
        }
        else
        {
            do
            {
                system("clear || cls"); //limpa a tela para sensacao de pop-up
                printf("\nValor do pagamento: %.2f\n", pgto_pedido.total); //apresenta o valor que o usuario entregou

                /*INTERACAO COM O USUARIO*/
                //apresentacao das opcoes ao usuario
                printf("\nConfirmar? (0 - voltar / 1 - sim)\n");
                conf_pgto = getch(); //capta qual eh a opcao escolhida pelo usuario

                switch (conf_pgto) //encaminha o usuario para opcao escolhida
                {
                    case '0': //opcao de cancelar o pagamento
                        system("clear || cls"); //limpa a tela para sensacao de pop-up
                        printf("\nVoltando para pagamento no CREDITO...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); //pausa a tela para leitura do usuario
                        break;

                    case '1': //opcao de confirmar o pagamento
                        pgto_pedido.n_pedido = n_pedido_atual; //grava o 'n_pedido'
                        pgto_pedido.forma_pgto = '4'; //grava a 'forma_pgto'
                        pgto_pedido.entregue = false; //informa que o pedido ainda não foi entregue
                        strcpy(pgto_pedido.n_cartao, n_cartao_digitado); //grava o 'n_cartao' no 'pgto_pedido'
                        esconder_n_cartao(); //substitui os numeros do meio por '*'
                        pgto = true; //confirma o pagamento do pedido
                        voltar = true; //fara com que termine a execucao do submodulo e volte ao modulo 'cobranca'
                        break;

                    default:
                        break;
                }
            } while (conf_pgto != '0' && conf_pgto != '1');
            /*Enquanto o usuario nao digitar uma opcao valida o ciclo continuara em execucao.*/
        }
    } while (! voltar);
    /*O submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true*/

    if (pgto) //se o pagamento for confirmado
        return true; //retorna a confirmacao positiva
    else
        return false; //retorna a confirmacao negativa
}

//--------------------------------------
/**MODULO FILA DE ESPERA**/
//--------------------------------------

void finalizar_pedido (void)
/*
Pedir ao usuario seu nome para identificacao e apresentar a estimativa
de tempo e numero de pedidos a sua frente.*/
{
    //funcoes utilizadas
    void get_string(char linha[]);
    //int  getch(void);
    bool validar_nome_cliente(char nome_cliente[]);
    void formatar_string(char linha[]);
    void import_pedidos(void);
    int  qtd_pedidos_frente(int n_pedido);
    void novo_n_pedido(void);
    void export_lancamentos(void);
    void export_pagamento(void);

    //variaveis locais
    bool terminar;
    int pedidos_frente;

    do
    {
        system("clear ||cls");
        printf("\n>>>FINALIZACAO DE PEDIDO\n");
        terminar = false;

        /*INTERACAO COM O USUARIO*/
        printf("\nDigite seu NOME: ");
        get_string (pgto_pedido.nome_cliente);

        if (strlen(pgto_pedido.nome_cliente) < 3 || ! validar_nome_cliente(pgto_pedido.nome_cliente))
        {
            system("clear ||cls"); //limpa a tela para sensacao de pop-up
            printf("\nDigite um nome VALIDO.\n");
            printf("\nCertifique-se que o nome:\n");
            printf("- Nao contenha caracteres especiais;\n");
            printf("- Tenha no mínimo 3 caracteres;\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); //pausa para leitura da mensagem pelo usuario
        }
        else
        {
            formatar_string(pgto_pedido.nome_cliente);
            import_pedidos ();
            pedidos_frente = qtd_pedidos_frente (pgto_pedido.n_pedido);
            export_lancamentos();
            export_pagamento();
            novo_n_pedido();
            system("clear ||cls");
            printf("\nPedido CONFIRMADO!\n");
            printf("\nPagamento REALIZADO!\n");
            printf("\nExistem %i pedidos na sua frente\n", pedidos_frente);
            printf("\nAGRADECEMOS a sua compra!");
            terminar = true; //termina o ciclo
            getch(); //pausa para leitura da mensagem pelo usuario
        }
    } while (! terminar);
}

//--------------------------------------
/**FUNCOES AUXILIARES**/
//--------------------------------------

///Manipulacao de dados

void limpar_lanca_pedido (void)
/*Objetivo da funcao
Limpar variavel 'lanca_pedido'.*/
{
    //funcoes utilizadas
    void limpar_lancamento(int indice_lancamento_ref, bool subir);

    for (int i = 0; i < indice_lancamento; ++i) //passa por todos os lancamentos realizados
        limpar_lancamento(i, false); //limpa todas os campos do lancamento

    indice_lancamento = 0; //zera o indice do lancamento a ser realizado
}

void limpar_pgto_pedido (void)
/*Objetivo da funcao
Limpar variavel 'pgto_pedido'.*/
{
    pgto_pedido.n_pedido = '\0'; //preenche '\0' (vazio) em 'n_pedido'
    pgto_pedido.total = '\0'; //preenche '\0' (vazio) em 'total'
    pgto_pedido.forma_pgto = '\0'; //preenche '\0' (vazio) em 'forma_pgto'
    for (int i = 0; i < t_n_cartao; ++i) //passa por todos os caracteres de 'n_cartao'
        pgto_pedido.n_cartao[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de 'n_cartao'
    for (int i = 0; i < t_nome_cliente; ++i) //passa por todos os caracteres de 'nome_cliente'
        pgto_pedido.nome_cliente[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de 'nome_cliente'
}

void limpar_lancamento (int indice_lancamento_ref, bool subir)
/*Objetivo da funcao
Limpar um lancamento especifico do pedido. Se for necessário trazer os pedidos seguintes ao apagado para cima,
utilize 'true' como segundo argumento.*/
{
    //funcoes utilizadas
    void subir_lancamento(int indice_ref);
    void calcular_pgto_total(void);

    lanca_pedido[indice_lancamento_ref].n_pedido = '\0'; //preenche '\0' (vazio) em 'n_pedido'
    lanca_pedido[indice_lancamento_ref].cod_prod = '\0'; //preenche '\0' (vazio) em 'cod_prod'
    for (int i = 0; i < t_nome_prod; ++i) //passa por todos os caracteres de 'nome_prod'
        lanca_pedido[indice_lancamento_ref].nome_prod[i] = '\0'; //preenche '\0' (vazio) em todos os caracteres de 'nome_prod'
    lanca_pedido[indice_lancamento_ref].custo_prod = '\0'; //preenche '\0' (vazio) em 'custo_prod'
    lanca_pedido[indice_lancamento_ref].tipo_prod = '\0'; //preenche '\0' (vazio) em 'tipo_prod'
    lanca_pedido[indice_lancamento_ref].qtd = '\0'; //preenche '\0' (vazio) em 'qtd'
    lanca_pedido[indice_lancamento_ref].subtotal = '\0'; //preenche '\0' (vazio) em 'subtotal'

    if (subir) //se pedir para subir os lancamentos abaixo do lancamento indicado
    {
        while (lanca_pedido[indice_lancamento_ref + 1].cod_prod != '\0') //o ciclo roda ate que o proximo lancamento seja '\0' (vazio)
        {
            subir_lancamento(indice_lancamento_ref + 1); //sobe o proximo lancamento para a o 'indice_lancamento'
            ++indice_lancamento_ref; //vai para o proximo indice de lancamento
        }
        /*O ciclo rodara ate que o proximo lancamento a ser subido seja '\0' (vazio)*/

        indice_lancamento = indice_lancamento_ref; //atualiza o indice do proximo lancamento a realizar
        calcular_pgto_total(); //calcula o 'total' do pedido atual
    }
}

void subir_lancamento (int indice_ref)
/*Objetivo da funcao
Copia os dados do indice referente para um indice anterior e apaga os dados do indice referente.*/
{
    //funcoes utilizadas
    void limpar_lancamento(int indice_lancamento_ref, bool subir);

    //variaveis locais
    int indice_novo = indice_ref - 1;

    lanca_pedido[indice_novo].n_pedido = lanca_pedido[indice_ref].n_pedido; //copia o 'n_pedido' do indice referente para o indice anterior
    lanca_pedido[indice_novo].cod_prod = lanca_pedido[indice_ref].cod_prod; //copia o 'cod_prod' do indice referente para o indice anterior
    for (int i = 0; i < t_nome_prod; ++i) //passa por todos os caracteres de 'nome_prod'
        lanca_pedido[indice_novo].nome_prod[i] = lanca_pedido[indice_ref].nome_prod[i]; //copia todos os caracteres de 'nome_prod' do indice referente para o indice anterior
    lanca_pedido[indice_novo].custo_prod = lanca_pedido[indice_ref].custo_prod; //copia o 'custo_prod' do indice referente para o indice anterior
    lanca_pedido[indice_novo].tipo_prod = lanca_pedido[indice_ref].tipo_prod; //copia o 'tipo_prod' do indice referente para o indice anterior
    lanca_pedido[indice_novo].qtd = lanca_pedido[indice_ref].qtd; //copia o 'qtd' do indice referente para o indice anterior
    lanca_pedido[indice_novo].subtotal = lanca_pedido[indice_ref].subtotal; //copia o 'subtotal' do indice referente para o indice anterior

    limpar_lancamento(indice_ref, false); //limpa os dados do indice referente que foram copiados para o indice anterior
}

void esconder_n_cartao (void)
/*Objetivo da funcao
Substituir os numeros de posicao 5 ate a posicao 12 do 'n_cartao' por '*'.*/
{
    for (int i = 4; i < 12; ++i)
        pgto_pedido.n_cartao[i] = '*';
}

void limpar_pedidos_realizados (void)
{
    for (int i = 0; i < max_pedidos_realizados; ++i)
    {
        pedidos_realizados[i].n_pedido = 0;
        pedidos_realizados[i].total = 0;
        pedidos_realizados[i].forma_pgto = '\0';
        for (int ii = 0; ii < t_n_cartao; ++ii)
            pedidos_realizados[i].n_cartao[ii] = '\0';
        for (int ii = 0; ii < t_nome_cliente; ++ii)
            pedidos_realizados[i].nome_cliente[ii] = '\0';
        pedidos_realizados[i].entregue = false;
    }
    qtd_pedidos_realizados = 0;
}

void limpar_pedidos_entregues (void)
{
    for (int i = 0; i < max_pedidos_realizados; ++i)
    {
        pedidos_entregues[i].n_pedido = 0;
        pedidos_entregues[i].total = 0;
        pedidos_entregues[i].forma_pgto = '\0';
        for (int ii = 0; ii < t_n_cartao; ++ii)
            pedidos_entregues[i].n_cartao[ii] = '\0';
        for (int ii = 0; ii < t_nome_cliente; ++ii)
            pedidos_entregues[i].nome_cliente[ii] = '\0';
        pedidos_entregues[i].entregue = false;
    }
    qtd_pedidos_entregues = 0;
}

void limpar_pedidos_nao_entregues (void)
{
    for (int i = 0; i < max_pedidos_nao_entregues; ++i)
    {
        pedidos_nao_entregues[i].n_pedido = 0;
        pedidos_nao_entregues[i].total = 0;
        pedidos_nao_entregues[i].forma_pgto = '\0';
        for (int ii = 0; ii < t_n_cartao; ++ii)
            pedidos_nao_entregues[i].n_cartao[ii] = '\0';
        for (int ii = 0; ii < t_nome_cliente; ++ii)
            pedidos_nao_entregues[i].nome_cliente[ii] = '\0';
        pedidos_nao_entregues[i].entregue = false;
    }
    qtd_pedidos_nao_entregues = 0;
}

///Visualizacao de dados

void mostrar_pedido (void)
/*Objetivo da funcao
Mostrar na tela as informacoes ate o momento do pedido.*/
{
    //funcoes utilizadas
    void imprime_n_pedido(void);
    void imprime_cabecalho_pedido(void);
    void imprime_lancamento(int indice_ref);
    void imprime_total_pedido(void);

    if(indice_lancamento == 0) //se nao houver lancamentos realizados
    {
        printf("\nPedido No %i", n_pedido_atual); //imprime 'n_pedido' atual
        printf("\nNao ha lancamentos no pedido.\n");
    }
    else
    {
        printf("\n====================SEU PEDIDO===================="); //imprime titulo do quadro

        imprime_n_pedido(); //imprime o 'n_pedido'

        imprime_cabecalho_pedido(); //imprime o cabecalho de lancamentos

        for(int i = 0; i < indice_lancamento; ++i) //passa por todos os lancamentos realizados
            imprime_lancamento(i); //imprime o lancamento

        imprime_total_pedido(); //imprime o total do pedido

        printf("==================================================\n"); //imprime rodape do quadro
    }
}

void mostrar_menu (void)
/*Objetivo da funcao
Mostrar na tela os produtos disponiveis no 'menu'.*/
{
    if (menu[0].cod_prod == '\0') //se nao houver produtos no 'menu'
        printf("\nNao ha itens no menu.\n");
    else
    {
        printf("\n============menu============="); //imprime titulo da quadro
        printf("\nCodigo Nome do produto  Custo\n"); //imprime cabecalho da tabela
        for (int i = 0; menu[i].cod_prod != '\0'; ++i) //ciclo que passa por todos os produtos do 'menu'
        {
            //impressao do 'cod_prod'
            printf("%i     ", menu[i].cod_prod); //imprime 'cod_prod' (como tem sempre dois caracteres nao eh necessario quantidade de espacos diferentes)

            //impressao do 'nome_prod'
            for (int ii = 0; ii < t_nome_prod; ++ii) //ciclo que passa por todos os caracteres de 'nome_prod'
            {
                if (menu[i].nome_prod[ii] != '\0') //se o caractere de 'nome_prod' for diferente de vazio ele imprime na tela
                    printf("%c", menu[i].nome_prod[ii]);
                else //se for vazio ele imprime um espaco
                    printf(" ");
            }
            printf(" "); //um espaco para separacao da proxima informacao

            //impressao do 'custo_prod'
            //dependendo do valor de 'custo_prod' eh necessario uma quantidade de espacos para alinhar ao quadro
            if (menu[i].custo_prod < 10)
                printf("  %.2f\n", menu[i].custo_prod);
            else if (menu[i].custo_prod < 100)
                printf(" %.2f\n" , menu[i].custo_prod);
            else
                printf("%.2f\n"  , menu[i].custo_prod);
        }
        printf("=============================\n"); //imrpime rodape do quadro
    }
}

void mostrar_lancamento (int indice_ref)
/*Objetivo da funcao
Mostrar na tela um lancamento especifico.*/
{
    //funcoes utilizadas
    void imprime_cabecalho_pedido(void);
    void imprime_lancamento(int indice_ref);

    imprime_cabecalho_pedido(); //imprime o cabecalho de lancamentos

    imprime_lancamento(indice_ref); //imprime o lancamento indicado
}

void imprime_n_pedido (void)
/*Objetivo da funcao
Imprimir o 'n_pedido' atual.*/
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
        printf("%i\n"     , n_pedido_atual);
}

void imprime_cabecalho_pedido (void)
/*Objetivo da funcao
Imprimir o cabecalho de lancamentos.*/
{
    printf("\ni   Codigo  Nome do produto   Custo  Qtd  Subtotal\n"); //imprime cabecalho da tabela
}

void imprime_lancamento (int indice_ref)
/*Objetivo da funcao
Imprimir um lancamento especifico.*/
{
    //impressao do 'indice'
    if (indice_ref + 1 < 10)
        printf("%i   ", indice_ref + 1);
    else if (indice_ref + 1 < 100)
        printf("%i  ", indice_ref + 1);
    else
        printf("%i ", indice_ref + 1);

    //impressao do 'cod_prod'
    printf("%i      ",   lanca_pedido[indice_ref].cod_prod); //imprime 'cod_prod' (como tem sempre dois caracteres nao eh necessario quantidade de espacos diferentes)

    //impressao do 'nome_prod'
    for (int ii = 0; ii < t_nome_prod; ++ii) //ciclo que passa por todos os caracteres de 'nome_prod'
    {
        if (lanca_pedido[indice_ref].nome_prod[ii] != '\0') //se o caractere de 'nome_prod' for diferente de vazio ele imprime na tela
            printf("%c", lanca_pedido[indice_ref].nome_prod[ii]);
        else //se for vazio ele imprime um espaco
            printf(" ");
    }
    printf(" "); //um espaco para separacao da proxima informacao

    //impressao do 'custo_prod'
    //dependendo do valor de 'custo_prod' eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[indice_ref].custo_prod < 10)
        printf("   %.2f ", lanca_pedido[indice_ref].custo_prod);
    else if (lanca_pedido[indice_ref].custo_prod < 100)
        printf("  %.2f " , lanca_pedido[indice_ref].custo_prod);
    else
        printf(" %.2f "  , lanca_pedido[indice_ref].custo_prod);

    //impressao da 'qtd'
    //dependendo do valor de 'qtd' eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[indice_ref].qtd < 10)
        printf("   %i ", lanca_pedido[indice_ref].qtd);
    else if (lanca_pedido[indice_ref].qtd < 100)
        printf("  %i " , lanca_pedido[indice_ref].qtd);
    else
        printf(" %i "  , lanca_pedido[indice_ref].qtd);

    //impressao do 'subtotal'
    //dependendo do valor de 'subtotal' eh necessario uma quantidade de espacos para alinhar ao quadro
    if (lanca_pedido[indice_ref].subtotal < 10)
        printf("     %.2f\n", lanca_pedido[indice_ref].subtotal);
    else if (lanca_pedido[indice_ref].subtotal < 100)
        printf("    %.2f\n" , lanca_pedido[indice_ref].subtotal);
    else if (lanca_pedido[indice_ref].subtotal < 1000)
        printf("   %.2f\n"  , lanca_pedido[indice_ref].subtotal);
    else if (lanca_pedido[indice_ref].subtotal < 10000)
        printf("  %.2f\n"   , lanca_pedido[indice_ref].subtotal);
    else
        printf(" %.2f\n"    , lanca_pedido[indice_ref].subtotal);
}

void imprime_total_pedido (void)
/*Objetivo da funcao
Imprimir o 'total' do pedido.*/
{
    printf("\n                                   Total ");
    //dependendo do valor de 'total' eh necessario uma quantidade de espacos para alinhar ao quadro
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
        printf("%.2f\n"     , pgto_pedido.total);
}

///Manipulacao de arquivos

void import_menu (void)
/*Objetivo da funcao
Importar do arquivo .DAT os produtos disponiveis e guardar na variavel 'menu'.*/
{
    //funcoes utilizadas
    //int getch(void);

    //variaveis locais
    FILE *pont_menu;

    pont_menu = fopen(menu_dat, "rb"); //abre o arquivo para leitura de binarios

    if (pont_menu == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
        fread(menu, sizeof(struct produto), max_menu, pont_menu); //le os produtos do arquivo e grava na variavel 'menu'

    fclose(pont_menu); //fecha o arquivo
}

void novo_n_pedido (void)
/*Objetivo da funcao
Importar o ultimo 'n_pedido' utilizado, guardar o proximo 'n_pedido' em 'n_pedido_atual'
e exportar o 'n_pedido' de volta no arquivo .DAT.*/
{
    //funcoes utilizadas
    //int getch(void);

    //variaveis locais
    FILE *pont_n_pedido;

    pont_n_pedido = fopen(n_pedido_dat, "rb"); //abre o arquivo para leitura de binarios

    if (pont_n_pedido == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        fread(&n_pedido_atual, sizeof(int), 1, pont_n_pedido); //le o 'n_pedido' salvo no arquivo e guarda em 'n_pedido_atual'

        ++n_pedido_atual; //acrescenta +1 para se tornar o proximo nº de pedido sendo o atual

        fclose(pont_n_pedido); //fecha o arquivo

        pont_n_pedido = fopen(n_pedido_dat, "wb"); //abre o arquivo para escrita de binarios

        fwrite(&n_pedido_atual, sizeof(int), 1, pont_n_pedido); //escreve o 'n_pedido_atual' no arquivo
    }
    fclose(pont_n_pedido); //fecha o arquivo
}

void export_lancamentos (void)
/*Objetivo da funcao
Exportar os lancamentos realizados para o arquivo .DAT.*/
{
    //funcoes utilizadas
    //int getch(void);
    void limpar_lanca_pedido(void);

    //variaveis locais
    FILE *pont_lancamentos;

    pont_lancamentos = fopen(lancamentos_dat, "ab"); //abre o arquivo para anexação de binarios

    if (pont_lancamentos == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        fwrite(lanca_pedido, sizeof(struct lancamento), indice_lancamento, pont_lancamentos); //anexa os lancamentos no arquivo

        limpar_lanca_pedido();  //limpa todos os dados de 'lanca_pedido'
    }

    fclose(pont_lancamentos); //fecha o arquivo
}

void export_pagamento (void)
/*Objetivo da funcao
Exportar o pagamento realizado para o arquivo .DAT.*/
{
    //funcoes utilizadas
    //int getch(void);
    void limpar_pgto_pedido(void);

    //variaveis locais
    FILE *pont_pagamentos;

    pont_pagamentos = fopen(pagamentos_dat, "ab"); //abre o arquivo para anexação de binarios

    if (pont_pagamentos == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        fwrite(&pgto_pedido, sizeof(struct pagamento), 1, pont_pagamentos); //anexa o pagamento no arquivo

        limpar_pgto_pedido();  //limpa todos os dados de 'pgto_pedido'
    }

    fclose(pont_pagamentos); //fecha o arquivo
}

void import_pedidos (void)
{
    //funcoes utilizadas
    //int getch(void);
    void limpar_pedidos_realizados(void);
    void limpar_pedidos_entregues(void);
    void limpar_pedidos_nao_entregues(void);

    //variaveis locais
    FILE *pont_pagamentos;
    int continua;
    int i, c;
    struct pagamento provisorio;

    pont_pagamentos = fopen(pagamentos_dat, "rb"); //abre o arquivo para leitura de binarios

    if (pont_pagamentos == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        limpar_pedidos_realizados();
        i = 0;
        continua = fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);

        while (continua == 1)
        {
            fseek(pont_pagamentos, i*sizeof(struct pagamento), SEEK_SET);
            fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);
            if (provisorio.entregue == 0)
            {
                pedidos_realizados[qtd_pedidos_realizados] = provisorio;
                ++qtd_pedidos_realizados;
            }
            ++i;
            continua = fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);
        }

        rewind(pont_pagamentos);
        limpar_pedidos_entregues();
        i = 0;
        continua = fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);

        while (continua == 1)
        {
            fseek(pont_pagamentos, i*sizeof(struct pagamento), SEEK_SET);
            fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);
            if (provisorio.entregue == 1)
            {
                pedidos_entregues[qtd_pedidos_entregues] = provisorio;
                ++qtd_pedidos_entregues;
            }
            ++i;
            continua = fread(&provisorio, sizeof(struct pagamento), 1, pont_pagamentos);
        }

        limpar_pedidos_nao_entregues();

        for (int i = 0; i < qtd_pedidos_realizados; ++i)
        {
            c = 0;
            for (int ii = 0; ii < qtd_pedidos_entregues; ++ii)
            {
                if (pedidos_realizados[i].n_pedido == pedidos_entregues[ii].n_pedido)
                    c = 1;
            }
            if (c == 0)
            {
                pedidos_nao_entregues[qtd_pedidos_nao_entregues] = pedidos_realizados[i];
                ++qtd_pedidos_nao_entregues;
            }
        }
    }
    fclose(pont_pagamentos); //fecha o arquivo
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
        while(indice_cod_prod == -1 && menu[i].cod_prod != 0)
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

    for (int i = 0; i <= indice_lancamento; ++i) //somara todos os subtotal de 'l_pedido' em total de todas as linhas preenchidas
        pgto_pedido.total += lanca_pedido[i].subtotal;
}

int qtd_pedidos_frente (int n_pedido)
{
    //variveis locais
    int pedidos_frente = 0;

    for (int i = 0; i < qtd_pedidos_nao_entregues; ++i)
    {
        if (pedidos_nao_entregues[i].n_pedido < n_pedido)
            ++pedidos_frente;
    }
    return pedidos_frente;
}

bool valida_cartao (char n_cartao[])
/*Objetivo da funcao
Verificar se o 'n_cartao' referido e valido.*/
{
    //variaveis locais
    int cont_invalidos = 0;

    for (int i = 0; i < t_n_cartao - 1; ++i) //passa por todos os caracteres do 'n_cartao'
    {
        if(n_cartao[i] < 48 || n_cartao[i] > 57) //se o caracter nao for um numero entre '0' e '9'
            ++cont_invalidos; // conta um caractere invalido
    }

    if(cont_invalidos == 0 && n_cartao[t_n_cartao - 1] == '\0') //se o 'n_cartao' nao tiver caractere invalido e haver '\0' (vazio) no ultimo
        return true; //valida 'n_cartao'
    else
        return false; //nao valida 'n_cartao'
}

bool validar_nome_cliente (char nome_cliente[])
/*Objetivo da funcao
Verifica se o 'nome_cliente' referido segue os padroes da variavel.*/
{
    //variaveis locais
    int cont_c_especial = 0;

    //ciclo que faz uma contagem de caracteres especiais no 'nome_cliente'
    for(int i = 0; i < t_nome_cliente && nome_cliente[i] != '\0'; ++i)
    {
        if((nome_cliente[i] >= 48 && nome_cliente[i] <= 57)  || //verifica se o caractere eh algarismos de 0 a 9 ou
           (nome_cliente[i] >= 65 && nome_cliente[i] <= 90)  || //letras maiusculas ou
           (nome_cliente[i] >= 97 && nome_cliente[i] <= 122) || //letras minusculas ou
            nome_cliente[i] == 95 || nome_cliente[i] == 32)     //espacos ou underlines (que sao permitidos)
        {
        }
        else //caso o caractere nao cumpra as condicoes acima
            ++cont_c_especial; //somara 1 na contagem
    }

    if(cont_c_especial == 0 &&              //se há algum tipo de caractere especial e
       nome_cliente[t_nome_cliente - 1] == '\0')  //se o último caractere é '\0' (vazio)
        return true;
    else
        return false;
    /*Retornara 'true', e portanto valido, caso o 'nome_prod' nao contenha caracteres especiais,
    nao seja um nome ja utilizado e não ultrapasse o limite de caracteres. Para esta segunda
    condicao eh usado a funcao 'buscar'. Śe esta retorna -1 eh porque o 'nome_prod' nao foi
    utilizado ainda.*/
}

//--------------------------------------
/**FUNCOES BASICAS**/
//--------------------------------------

/*
int getch (void) //LINUX - criacao da funcao 'getch' pois o sistema nao tem a biblioteca 'conio.h'
//Objetivo da funcao
//Capturar o proximo caractere que o usuario digitar
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
*/

void limpar_buffer (void) //funcao criada para limpar o buffer do teclado que funciona em WINDOWS e LINUX
/*Objetivo da funcao
Limpar qualquer "sujeira" que tiver no buffer do teclado. So funciona se realmente houver algo no buffer
Deve-se utilizar apos a funcao 'scanf'*/
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_string (char linha[]) //funcao que funciona melhor que gets
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

void formatar_string (char linha[])
/*Objetivo da funcao
Substitui da string referida os espacos por underline e as letras minusculas por maiusculas.*/
{
    //variaveis locais
    int i = 0;

    while (linha[i] != '\0')
    {
        if (linha[i] == 32) //caso o caractere seja espaco
            linha[i] = 95; //substitui por underline

        if (linha[i] >= 97 && linha[i] <= 122) //caso o caractere seja letra minuscula
            linha[i] -= 32; //substitui por sua letra maiuscula
        ++i; //parte para o proximo caractere
    }
    /*O ciclo rodara ate que o proximo caractere verificado seja '\0' (vazio).*/
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

Ultima atualizacao: 25-out.-2020 as 10h18
*/
