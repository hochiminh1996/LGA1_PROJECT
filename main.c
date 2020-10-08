/*
Leandro Higa:
>> Criei essa interface baseado no mapa lógico que tinha montado já.
>> Perceba que todas as funções que foram criadas estão abaixo de 'main' e se eu preciso de uma função qualquer
   dentro de outra função ou na própria 'main', eu declaro ela antes de tudo. Vi pela internet que esse é um
   padrão recomendado.
>> Criei a função 'getch' pois não há no LINUX, creio que deva funcionar normal no WINDOWS.
>> Perceba que coloquei no que usei 'clear || cls'. Isso porque no LINUX o comando de limpar a tela é 'clear'.
   No formato escrito ela funcionará em ambos os sistemas.
>> Também fiz com que a função 'main' chame o módulo 'vendas'. Acho que os módulos não funcionam de maneira
   linear. Logo, na main deverá ser possível escolher o módulo que você quer entrar.
>> Pode dar play pois, em teoria, deve funcionar.
*/

#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
#include <termios.h> // para utilizar 'getch'
#include <unistd.h> // para utilizar 'getch'

/*
Eu identifiquei 3 estruturas diferentes que precisamos trabalhar:
- Produto [CodProd / NomeProd / CustoProd] >> Eu vi que você já montou ela e daí teríamos nossa base de produtos a partir dela.
- Pedido_lancamentos [Npedido / CodProd / NomeProd / CustoProd / Qtd / Valor] >> Será muito usada no módulo 'Vendas'.
- Pedido_pagamentos [Npedido / ValorTotal / FormaPgto] >> Será muito usada no módulo 'Cobrança'.
*/

int main (void)
{
    void vendas(void);

    vendas();

    return 0;
}

void vendas(void)
{
    void mostrar_pedido(void);
    int getch(void);
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

int getch(void) // nem me pergunte como funciona, só sei que funciona kkkk.
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
