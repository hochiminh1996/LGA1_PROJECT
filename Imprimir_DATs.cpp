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

struct produto prod;

struct lancamento lanca;

struct pagamento pgto;

int n_pedido;

int main (void)
{
    //variaveis locais
    FILE *pont_n_pedido;
    FILE *pont_menu;
    FILE *pont_lancamentos;
    FILE *pont_pagamentos;
    int i, n;


    pont_n_pedido = fopen(n_pedido_dat, "rb");

    if (pont_n_pedido == NULL)
    {
        printf("\nERRO N_PEDIDO\n");
    }
    else
    {
        fread(&n_pedido, sizeof(int), 1, pont_n_pedido);

        printf("\nULTIMO N_PEDIDO: %i\n", n_pedido);
    }

    fclose(pont_n_pedido);

    pont_menu = fopen(menu_dat, "rb");

    if (pont_menu == NULL)
    {
        printf("\nERRO MENU\n");
    }
    else
    {
        i = 0;
        n = 0;
        printf("\n");
        do
        {
            fseek(pont_menu, i*sizeof(struct produto), SEEK_SET);
            fread(&prod, sizeof(struct produto), 1, pont_menu);
            printf(  "%i\t", prod.cod_prod  );
            printf(    "%s", prod.nome_prod );
            for(int x = 0; t_nome_prod-1 - strlen(prod.nome_prod) - x != 0; ++x)
                printf(" ");
            printf("\t");
            printf("%.2f\t", prod.custo_prod);
            printf(  "%c\n", prod.tipo_prod );
            ++i;
            n = fread(&prod, sizeof(struct produto), 1, pont_menu);
        } while (n != 0);
    }

    fclose(pont_menu);

    pont_lancamentos = fopen(lancamentos_dat, "rb");

    if (pont_lancamentos == NULL)
    {
        printf("\nERRO LANCAMENTOS\n");
    }
    else
    {
        i = 0;
        n = 0;
        printf("\n");
        do
        {
            fseek(pont_lancamentos, i*sizeof(struct lancamento), SEEK_SET);
            fread(&lanca, sizeof(struct lancamento), 1, pont_lancamentos);
            printf(  "%i\t", lanca.n_pedido  );
            printf(  "%i\t", lanca.cod_prod  );
            printf(    "%s", lanca.nome_prod );
            for(int x = 0; t_nome_prod-1 - strlen(lanca.nome_prod) - x != 0; ++x)
                printf(" ");
            printf("\t");
            printf("%.2f\t", lanca.custo_prod);
            printf(  "%c\t", lanca.tipo_prod );
            printf(  "%i\t", lanca.qtd       );
            printf("%.2f\t", lanca.subtotal  );
            printf(  "%i\n", lanca.preparado );
            ++i;
            n = fread(&lanca, sizeof(struct lancamento), 1, pont_lancamentos);
        } while (n != 0);
    }

    fclose(pont_lancamentos);

        pont_pagamentos = fopen(pagamentos_dat, "rb");

    if (pont_pagamentos == NULL)
    {
        printf("\nERRO PAGAMENTOS\n");
    }
    else
    {
        i = 0;
        n = 0;
        printf("\n");
        do
        {
            fseek(pont_pagamentos, i*sizeof(struct pagamento), SEEK_SET);
            fread(&pgto, sizeof(struct pagamento), 1, pont_pagamentos);
            printf(  "%i\t",      pgto.n_pedido);
            printf("%.2f\t",         pgto.total);
            printf(  "%c\t",    pgto.forma_pgto);
            printf(    "%s",      pgto.n_cartao);
            for(int x = 0; t_n_cartao-1 - strlen(pgto.n_cartao) - x != 0; ++x)
                printf(" ");
            printf("\t");
            printf(    "%s", pgto.nome_cliente );
            for(int x = 0; t_nome_cliente-1 - strlen(pgto.nome_cliente) - x != 0; ++x)
                printf(" ");
            printf("\t");
            printf(  "%i\n",      pgto.entregue);
            ++i;
            n = fread(&lanca, sizeof(struct pagamento), 1, pont_pagamentos);
        } while (n != 0);
    }

    fclose(pont_pagamentos);

    return 0;
}
