/*
Leandro Higa (atualizado - 7-out.-2020):
>> Perceba que todas as funções que foram criadas estão abaixo de 'main' e se
   eu preciso de uma função qualquer dentro de outra função ou na própria 'main',
   eu declaro ela antes de tudo. Vi pela internet que esse é um padrão recomendado.
>> É preciso checar se a função 'setlocale' precisa estar apenas na função 'main'
   ou deve estar em todas as funções.

Leandro Higa (atualizado - 11-out.-2020):
>> Troquei de C para C++ pois assim será possível utilizar variáveis 'bool' mais
   facilmente.
>> Acrescentei o módulo 'cobranca', mas ainda não montei os sub-módulos dele pois
   quero ver como você montou as variáveis que inputarão as funções do módulo
   'cobranca'.
   >> Agora nosso menu está num arquivo .txt chamado menu_produtos.txt. Veja como eu
   faço para trazer do arquivo e jogar as informações dentro da variável
   'produto.menu'.
*/

#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
#include <conio.h> // WINDOWS - para utlizar 'getch'
//#include <termios.h> // LINUX - para utilizar 'getch'
//#include <unistd.h> // LINUX - para utilizar 'getch'
#include <locale.h> // WINDOWS - para utilizar 'setlocale'

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

struct produto menu[100] = {};

struct lancamento l_pedido[100] = {}; // cria um pedido com lancamentos limpos

struct pagamento p_pedido = {}; // cria um pedido com pagamento limpo

int main (void)
{
    setlocale(LC_ALL,"PORTUGUESE"); // WINDOWS - para caracteres especiais

    void carregar_menu(void);
    void vendas(void);

    carregar_menu();

    vendas();

    return 0;
}

void vendas(void)
{
    void mostrar_menu(void);
    void mostrar_pedido(void);
//    int getch(void); //LINUX
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    char opcao;

    do
    {
        mostrar_menu();

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

/*
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
*/

void limpar_menu (void) // limpa a variável de estrutura 'menu'
{
    for (int i = 0; i < 100; ++i)
    {
        menu[i].CodProd = '\0';
        for (int ii = 0; ii < 15; ++ii)
            menu[i].NomeProd[ii] = '\0';
        menu[i].CustoProd = '\0';
    }
}

void carregar_menu (void) // insere os dados de 'menu_produtos.txt' na variável de estrutura 'menu'
{
    void limpar_menu();

    limpar_menu();

    FILE *pont_menu;

    pont_menu = fopen("menu_produtos.txt", "r");

    if (pont_menu == NULL)
        printf("Arquivo não pode ser aberto\n");
    else
    {
        int i = 0; // índice do vetor menu

        do // irá puxar os dados até que o último que tenha sido lido foi '\0'
        {
            fscanf(pont_menu, "%i\t", &menu[i].CodProd);
            fscanf(pont_menu, "%s\t", menu[i].NomeProd); // strings não podem ter espaço
            fscanf(pont_menu, "%f\n", &menu[i].CustoProd);
            ++i;
        } while (menu[i - 1].CodProd != '\0');

        fclose(pont_menu);
    }
}

void mostrar_menu (void) // mostra os itens do menu
{
    int i = 0; // índice do vetor menu

    if (menu[i].CodProd == '\0') // caso não haja itens no menu
        printf("Não há itens no menu.\n\n");
    else
    {
        printf("Código\tNome do Produto\tCusto\n");
        do
        {
            printf("%i\t%15s\t%.2f\n", menu[i].CodProd, menu[i].NomeProd, menu[i].CustoProd);
            ++i;
        } while (menu[i].CodProd != '\0');
        printf("\n");
    }
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

void cobranca(void)
{
    void mostrar_pedido(void);
    int getch(void); //LINUX
    void limpar_pedido(void);
    void pgto_dinheiro(void);
    void pgto_cheque(void);
    void pgto_debito(void);
    void pgto_credito(void);

    char opcao;

    do
    {
        mostrar_pedido();

        printf("\nOpções:\n(0) - Cancelar pedido\n(1) - Dinheiro\n(2) - Cheque\n(3) - Cartão de Débito\n(4) - Cartão de Crédito\n\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                limpar_pedido();
                break;
            case '1':
                system("clear || cls");
                pgto_dinheiro();
                break;
            case '2':
                system("clear || cls");
                pgto_cheque();
                break;
            case '3':
                system("clear || cls");
                pgto_debito();
            case '4':
                system("clear || cls");
                pgto_credito();
                break;
            default:
                system("clear || cls");
                printf("\nOpção inválida.\n\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3' && opcao != '4');

}

void pgto_dinheiro(void)
{
    printf("\nVocê escolheu DINHEIRO.\n");
}

void pgto_cheque(void)
{
    printf("\nVocê escolheu CHEQUE.\n");
}

void pgto_debito(void)
{
    printf("\nVocê escolheu DEBITO.\n");
}

void pgto_credito(void)
{
    printf("\nVocê escolheu CREDITO.\n");
}
