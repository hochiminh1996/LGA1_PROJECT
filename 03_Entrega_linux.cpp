//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: Entrega Mickey & Donald

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

const int max_pedidos_realizados    = 500;
const int max_pedidos_nao_entregues = 50;
const int max_lancamentos           = 100;
const int max_imprimir              = 10; //numero maximo de lancamentos apresentados na tela

///Endereço dos arquivos

const char lancamentos_dat[] = {"LANCAMENTOS.DAT"}; //nome do arquivo que se encontra os dados de lancamentos
const char pagamentos_dat[]  = {"PAGAMENTOS.DAT"};  //nome do arquivo que se encontra os dados de pagamentos

///Parâmetros da cozinha

//tempo medio de preparo por categoria de produto:
const int tempo_hamburguer      = 5; //'1'
const int tempo_acomapanhamento = 7; //'2'
const int tempo_bebida          = 1; //'3'
const int tempo_outros          = 5; //'4'

//tempo limite maximo de producao por categoria de produto:
const int lim_max_hamburguer     = 10; //'1'
const int lim_max_acompanhamento = 20; //'2'
const int lim_max_bebida         =  5; //'3'
const int lim_max_outros         =  8; //'4'

//--------------------------------------
/**ESTRUTURAS UTILIZADAS**/
//--------------------------------------

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

struct pagamento pedidos_realizados[max_pedidos_realizados] = {};

int qtd_pedidos_realizados;

struct pagamento pedidos_entregues[max_pedidos_realizados] = {};

int qtd_pedidos_entregues;

struct pagamento pedidos_nao_entregues[max_pedidos_nao_entregues] = {};

int qtd_pedidos_nao_entregues;

struct lancamento lanca_pedido [max_lancamentos] = {};

int qtd_lancamentos;

//--------------------------------------
/**FUNCAO PRINCIPAL**/
//--------------------------------------

int main (void)
{
    //funcoes utilizadas
    void reg_entrega(void);

    reg_entrega();

    return 0;
}

//--------------------------------------
/**MODULO ENTREGA**/
//--------------------------------------

void reg_entrega (void)
{
    //funcoes utilizadas
    void import_pedidos(void);
    void mostrar_pedidos_nao_entregues(void);
    void mostrar_total_pedidos(void);
    void limpar_buffer(void);
    int  getch(void);
    int  buscar_n_pedido(int n_pedido_ref);
    void import_lancamentos(int n_pedido_ref);
    void mostrar_pedido_digitado(int n_pedido);
    void export_entrega(int n_pedido);
    void export_preparo(int n_pedido);

    //variaveis locais
    int n_pedido_digitado;
    bool sair;
    char conf_entrega;

    do
    {
        system("clear || cls");
        printf("\n>>>MODULO COZINHA\n");
        import_pedidos ();
        mostrar_pedidos_nao_entregues();
        mostrar_total_pedidos();
        sair = false;

        /*INTERACAO COM O USUARIO*/
        //inserindo 'i' preparado
        printf("\nDigite o N do pedido para mais detalhes (0 - sair): ");
        scanf("%i", &n_pedido_digitado);
        limpar_buffer();

        if (n_pedido_digitado == 0)
        {
            system("clear || cls");
            printf("\nFechando o sistema...\n");
            sair = true;
            getch();
        }
        else if (buscar_n_pedido(n_pedido_digitado) == -1)
        {
            system("clear || cls");
            printf("\nNumero do pedido digitado INVALIDO\n");
            getch();
        }
        else
        {
            do
            {
                import_lancamentos(n_pedido_digitado);
                mostrar_pedido_digitado(n_pedido_digitado);
                /*INTERACAO COM O USUARIO*/
                //inserindo 'i' preparado
                printf("\nConfirmar entrega (0 - nao | 1 - sim)?\n");
                conf_entrega = getch();

                switch (conf_entrega)
                {
                    case '0':
                        break;

                    case '1':
                        export_entrega(n_pedido_digitado);
                        export_preparo(n_pedido_digitado);
                        system("clear || cls");
                        printf("\nEntrega CONFIRMADA.\n");
                        getch();
                        break;

                    default:
                        break;
                }

            } while (conf_entrega != '0' && conf_entrega != '1');
        }
    } while (! sair);
}

//--------------------------------------
/**FUNCOES AUXILIARES**/
//--------------------------------------

///Manipulacao de arquivos

void import_pedidos (void)
{
    //funcoes utilizadas
    int getch(void);
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

void import_lancamentos (int n_pedido_ref)
{
    //funcoes utilizadas
    int  getch(void);
    void limpar_lanca_pedido(void);

    //variaveis locais
    FILE *pont_lancamentos;
    int i, continua;
    struct lancamento provisorio;

    pont_lancamentos = fopen(lancamentos_dat, "rb"); //abre o arquivo para leitura de binarios

    if (pont_lancamentos == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
    {
        limpar_lanca_pedido();
        i = 0;
        continua = fread(&provisorio, sizeof(struct lancamento), 1, pont_lancamentos);

        while (continua == 1)
        {
            fseek(pont_lancamentos, i*sizeof(struct lancamento), SEEK_SET);
            fread(&provisorio, sizeof(struct lancamento), 1, pont_lancamentos);
            if (provisorio.n_pedido == n_pedido_ref)
            {
                lanca_pedido[qtd_lancamentos] = provisorio;
                ++qtd_lancamentos;
            }
            ++i;
            continua = fread(&provisorio, sizeof(struct lancamento), 1, pont_lancamentos);
        }
    }
    fclose(pont_lancamentos);
}

void export_entrega (int n_pedido)
{
    //funcoes utilizadas
    int buscar_n_pedido(int n_pedido_ref);
    int getch(void);

    //variaveis locais
    int indice_pedido;
    FILE *pont_pagamentos;

    indice_pedido = buscar_n_pedido(n_pedido);

    if(indice_pedido == -1)
    {
        system("clear || cls");
        printf("\nPedido não encontrado.\n");
        getch();
    }
    else
    {
        pedidos_nao_entregues[indice_pedido].entregue = true;

        pont_pagamentos = fopen(pagamentos_dat, "ab"); //abre o arquivo para anexação de binarios

        if (pont_pagamentos == NULL) //caso o arquivo nao seja encontrado
        {
            system("clear || cls"); //limpa a tela para sensacao de pop-up
            printf("\nArquivo nao encontrado.\n");
            getch(); //pausa a tela para leitura do usuario
        }
        else
            fwrite(&pedidos_nao_entregues[indice_pedido], sizeof(struct pagamento), 1, pont_pagamentos);
        fclose(pont_pagamentos);
    }

}

void export_preparo (int n_pedido)
{
    //funcoes utilizadas
    int  getch(void);

    //variaveis locais
    FILE *pont_lancamentos;

    for (int i = 0; i < qtd_lancamentos; ++i)
        lanca_pedido[i].preparado = true;

    pont_lancamentos = fopen(lancamentos_dat, "ab"); //abre o arquivo para anexação de binarios

    if (pont_lancamentos == NULL) //caso o arquivo nao seja encontrado
    {
        system("clear || cls"); //limpa a tela para sensacao de pop-up
        printf("\nArquivo nao encontrado.\n");
        getch(); //pausa a tela para leitura do usuario
    }
    else
        fwrite(lanca_pedido, sizeof(struct lancamento), qtd_lancamentos, pont_lancamentos);

    fclose(pont_lancamentos);
}

///Limpeza de dados

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

void limpar_lanca_pedido (void)
{
    for (int i = 0; i < max_lancamentos; ++i)
    {
        lanca_pedido[i].n_pedido = 0;
        lanca_pedido[i].cod_prod = 0;
        for (int ii = 0; ii < t_nome_prod; ++ii)
            lanca_pedido[i].nome_prod[ii] = '\0';
        lanca_pedido[i].custo_prod = 0;
        lanca_pedido[i].tipo_prod = '\0';
        lanca_pedido[i].qtd = 0;
        lanca_pedido[i].subtotal = 0;
        lanca_pedido[i].preparado = false;
    }
    qtd_lancamentos = 0;
}

///Visualizacao de dados

void mostrar_pedidos_nao_entregues (void)
{
    printf("\n==========Pedidos nao preparados==========");
    printf("\nNpedido Nome do Cliente       Situacao\n");

    for (int i = 0; i < max_imprimir && i < qtd_pedidos_nao_entregues; ++i)
    {
        //impressao do 'n_pedido'
        if (pedidos_nao_entregues[i].n_pedido < 10)
            printf("%i       ", pedidos_nao_entregues[i].n_pedido);
        else if (pedidos_nao_entregues[i].n_pedido < 100)
            printf("%i      " , pedidos_nao_entregues[i].n_pedido);
        else if (pedidos_nao_entregues[i].n_pedido < 1000)
            printf("%i     "  , pedidos_nao_entregues[i].n_pedido);
        else if (pedidos_nao_entregues[i].n_pedido < 10000)
            printf("%i    "   , pedidos_nao_entregues[i].n_pedido);
        else if (pedidos_nao_entregues[i].n_pedido < 100000)
            printf("%i   "    , pedidos_nao_entregues[i].n_pedido);
        else
            printf("%i  "     , pedidos_nao_entregues[i].n_pedido);

        //impressao do 'nome_cliente'
        for (int ii = 0; ii < t_nome_cliente; ++ii)
        {
            if (pedidos_nao_entregues[i].nome_cliente[ii] != '\0')
                printf("%c", pedidos_nao_entregues[i].nome_cliente[ii]);
            else
                printf(" ");
        }
        printf(" ");

        //impressao da situacao
        if (pedidos_nao_entregues[i].entregue == false)
            printf("Não entregue\n");
        else
            printf("Entregue\n");
    }

    printf("==========================================\n");
}

void mostrar_total_pedidos (void)
{
    printf("\nTotal de pedidos a serem entregues: %i\n", qtd_pedidos_nao_entregues);
}

void mostrar_pedido_digitado(int n_pedido)
{
    printf("\n=====Lancamentos do pedido======");
    printf("\nNpedido Cod Nome do Produto  Qtd\n");

    for (int i = 0; i < qtd_lancamentos; ++i)
    {
        //impressao do 'n_pedido'
        if (lanca_pedido[i].n_pedido < 10)
            printf("%i       ", lanca_pedido[i].n_pedido);
        else if (lanca_pedido[i].n_pedido < 100)
            printf("%i      " , lanca_pedido[i].n_pedido);
        else if (lanca_pedido[i].n_pedido < 1000)
            printf("%i     "  , lanca_pedido[i].n_pedido);
        else if (lanca_pedido[i].n_pedido < 10000)
            printf("%i    "   , lanca_pedido[i].n_pedido);
        else if (lanca_pedido[i].n_pedido < 100000)
            printf("%i   "    , lanca_pedido[i].n_pedido);
        else
            printf("%i  "     , lanca_pedido[i].n_pedido);

        //impressao do 'cod_prod'
        printf(" %i ", lanca_pedido[i].cod_prod);

        //impressao do 'nome_prod'
        for (int ii = 0; ii < t_nome_prod; ++ii)
        {
            if (lanca_pedido[i].nome_prod[ii] != '\0')
                printf("%c", lanca_pedido[i].nome_prod[ii]);
            else
                printf(" ");
        }
        printf(" ");

        //impressao da 'qtd'
        if (lanca_pedido[i].qtd < 10)
            printf("   %i\n", lanca_pedido[i].qtd);
        else if (lanca_pedido[i].qtd < 100)
            printf("  %i\n" , lanca_pedido[i].qtd);
        else
            printf(" %i\n"  , lanca_pedido[i].qtd);

    }

    printf("================================\n");
}

///Diversos

int buscar_n_pedido (int n_pedido_ref)
{
    //variaveis locais
    int i = 0;
    int indice_pedido = -1;

    while (indice_pedido == -1 && i < qtd_pedidos_nao_entregues)
    {
        if(pedidos_nao_entregues[i].n_pedido == n_pedido_ref)
            indice_pedido = i;
        ++i;
    }

    return indice_pedido;
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

//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: Entrega Mickey & Donald

Descricao:
Projeto para avaliacao de desempenho na discipĺina de "Logica de programacao 1"
do curso "Tecnologo de Analise e Desenvolvimento de Sistemas" no Instituto Federal
de Sao Paulo - campus Sao Paulo

Desenvolvedor(es):
Felippe Marques da Silva de Almeida SP3056686
Leandro Paiva Higa                  SP3052648

Ultima atualizacao: 25-out.-2020 as 10h18
*/
