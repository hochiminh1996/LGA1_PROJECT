/*
Leandro Higa (atualizado - XX-out.-2020):
>> Perceba que todas as funções que foram criadas estão abaixo de 'main' e se
   eu preciso de uma função qualquer dentro de outra função ou na própria 'main',
   eu declaro ela antes de tudo. Vi pela internet que esse é um padrão recomendado.
>> É preciso checar se a função 'setlocale' precisa estar apenas na função 'main'
   ou deve estar em todas as funções.
*/

#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
//#include <conio.h> // WINDOWS - para utlizar 'getch'
#include <termios.h> // LINUX - para utilizar 'getch'
#include <unistd.h> // LINUX - para utilizar 'getch'
//#include <locale.h> // WINDOWS - para utilizar 'setlocale'

/*
Nas estruturas montadas, observa apenas 'produto', 'lancamento'
e 'pagamento' pois as outras eu criei, mas serão usadas nos outros
módulos apenas. Só queria deixar registrado já.
*/

struct produto // estrutura para formar a lista de produtos
{
    int CodProd; // código do produto
    char NomeProd[15]; // nome do produto
    float CustoProd; // custo do produto
};

struct lancamento // estrutura para formar os lançamentos do pedido
{
    int N_pedido; // nº do pedido
    int CodProd; // código do produto
    char NomeProd[15]; // nome do produto
    float CustoProd; // custo do produto
    int Qtd; // quantidade do produto
    float SubTotal; // CustoProd * Qtd
};

struct pagamento // estrutura para confirmar o pagamento do pedido
{
    int N_pedido; // nº do pedido
    float Total; // soma de todos os SubTotal do pedido
    char FormaPgto; // '1'=dinheiro '2'=cheque '3'=crédito '4'=débito
    char N_cartao[16]; // apenas para débito e crédito, pra outros deve ser '\0'
};

struct preparo // estrutura para formar os lançamentos a serem preparados
{
    int N_pedido; // nº do pedido
    int CodProd; // código do produto
    char NomeProd[15]; // nome do produto
    int Qtd; // quantidade do produto
    bool Situacao; // pronto para entrega ou não
};

struct entrega // estrutura para formar os pedidos a serem entregues
{
    int N_pedido; // nº do pedido
    bool Situacao1; // pronto para entrega ou não
    bool Situacao2; // entregue ou não
};

struct produto menu = {{10, 'X_BURGUER'   , 12.00}, // aqui está nosso menu
                       {11, 'X_SALADA'    , 14.00},
                       {12, 'X_BACON'     , 15.00},
                       {20, 'BATATA_FRITA', 10.00},
                       {21, 'NUGGETS'     , 11.00},
                       {30, 'COCA_COLA'   ,  9.00},
                       {31, 'GUARANA'     ,  8.00}};

struct lancamento pedido[100] = {}; // cria um pedido com lancamentos limpos;

struct pagamento pedido = {}; // cria um pedido com pagamento limpo;

int main (void)
{
//    setlocale(LC_ALL,"PORTUGUESE"); // WINDOWS - para caracteres especiais

    void vendas(void);

    vendas();

    return 0;
}

void vendas(void)
{
    void mostrar_pedido(void);
    int getch(void); //LINUX
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    char opcao;

    do
    {
        mostrar_pedido();

        printf("\nOpções:\n(0) - Cancelar pedido\n(1) - Registrar lançamentos\n(2) - Remover lançamentos\n(3) - Confirmar pedido\n\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                limpar_pedido();
                break;
            case '1':
                system("clear || cls");
                reg_lancamento();
                break;
            case '2':
                system("clear || cls");
                rm_lancamento();
                break;
            case '3':
                system("clear || cls");
                confirmar_pedido(); // ainda é necessário criar um verificador de nº de lançamentos. Só poderá continuar o processo se nº de lançamentos > 1.
                break;
            default:
                system("clear || cls");
                printf("\nOpção inválida.\n\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3');
}

int getch(void) // LINUX - nem me pergunte como funciona, só sei que funciona kkkk.
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

void mostrar_pedido(void)
{
    printf("Mostrar pedido na tela\n");
    /*
    Aqui devemos fazer com que na tela apareça as informações dos lançamentos feitos e o nº do pedido.
    */
}

void limpar_pedido(void)
{
    printf("\nVocê escolheu CANCELAR pedido.\n");
    /*
    Aqui devemos fazer com que todos os lançamentos sejam apagados.
    */
}

void reg_lancamento(void)
{
    printf("\nVocê escolheu REGISTRAR lançamento.\n");
    /*
    Este seria o sub-módulo 'reg_lancamento' que tem no mapa lógico.
    Creio que deva ser montado com base nele. É possível aproveitar funções que você já montou.
    */
}

void rm_lancamento(void)
{
    printf("\nVocê escolheu REMOVER lançamento.\n");
    /*
    Este seria o sub-módulo 'rm_lancamento' que tem no mapa lógico.
    Creio que deva ser montado com base nele.
    */
}

void confirmar_pedido(void)
{
    printf("\nVocê escolheu CONFIRMAR pedido.\n");
    /*

    */

}
