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
    char FormaPgto; // '1'=dinheiro '2'=cheque '3'=débito '4'=crédito
    char N_cartao[17]; // apenas para débito e crédito, pra outros deve ser '\0'
    char NomeCliente[21]; // nome para chamar o cliente quando produto pronto
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
    char NomeCliente[20]; // nome para chamar o cliente quando produto pronto
    bool Situacao1; // pronto para entrega ou não
    bool Situacao2; // entregue ou não
};

struct produto menu[100] = {};

//struct lancamento l_pedido[100] = {}; // cria um pedido com lancamentos limpos

//struct pagamento p_pedido = {}; // cria um pedido com pagamento limpo

//int indice_l = 0; // para acompanhar as linhas já escritas em 'l_pedido'

int n_pedido_atual;

struct lancamento l_pedido[100] = {{123, 10, "X_BURGUER",    12.00, 2, 24.00},
                                   {123, 20, "BATATA_FRITA", 10.00, 3, 30.00},
                                   {123, 31, "GUARANA",       8.00, 1,  8.00},
                                   {123, 32, "FANTA_UVA",     8.50, 1,  8.50}};

int indice_l = 4;

struct pagamento p_pedido = {123, 70.5, {}, {}, {}};

int main (void)
{
//    setlocale(LC_ALL,"PORTUGUESE"); // WINDOWS - para caracteres especiais

    void download_menu(void);
    void vendas(void);
    void novo_n_pedido(void);

    download_menu();

    novo_n_pedido();

    vendas();

    return 0;
}

void vendas(void)
{
    void mostrar_menu(void);
    void mostrar_pedido(void);
    int getch(void); // LINUX
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    char opcao;

    do
    {
        printf("\n===MONTE O PEDIDO===\n");

        mostrar_menu();

        mostrar_pedido();

        printf("\nN_pedido = %i\n", n_pedido_atual);
        printf("\nN_lancamentos = %i\n", indice_l);

        printf("\nOpções:\n");
        printf("(0) - Cancelar pedido\n");
        printf("(1) - Registrar lançamentos\n");
        printf("(2) - Remover lançamentos\n");
        printf("(3) - Confirmar pedido\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                char resposta1;
                do
                {
                system("clear || cls");
                printf("\n===ATENCAO===\n");
                printf("\nTem certeza que deseja CANCELAR o pedido? (0 - voltar / 1 - sim) ");
                resposta1 = getch();
                } while (resposta1 != '0' && resposta1 != '1');
                if (resposta1 == '1')
                {
                    limpar_pedido();
                    system("clear || cls");
                    printf("\nPedido CANCELADO.\n");
                }
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                opcao = '\0';
                system("clear || cls");
                break;
            case '1':
                system("clear || cls");
                printf("\nEntrando em REGISTRAR lançamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                reg_lancamento();
                break;
            case '2':
                system("clear || cls");
                printf("\nEntrando em REMOVER lançamentos...\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch();
                system("clear || cls");
                rm_lancamento();
                break;
            case '3':
                system("clear || cls");
                confirmar_pedido();
                break;
            default:
                system("clear || cls");
                printf("\nOpção inválida.\n");
                printf("\nPressione qualquer tecla para voltar...");
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

void download_menu (void) // insere os dados de 'menu_produtos.txt' na variável de estrutura 'menu'
{
    void limpar_menu();

    limpar_menu();

    FILE *pont_menu;

    pont_menu = fopen("menu_produtos.txt", "r");

    if (pont_menu == NULL)
        printf("\nArquivo não pode ser aberto\n");
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

    printf("\n==MENU==\n");

    if (menu[i].CodProd == '\0') // caso não haja itens no menu
        printf("\nNão há itens no menu.\n\n");
    else
    {
        printf("\nCódigo\tNome do Produto\tCusto\n");
        do
        {
            printf("%i\t%15s\t%.2f\n", menu[i].CodProd, menu[i].NomeProd, menu[i].CustoProd);
            ++i;
        } while (menu[i].CodProd != '\0');
    }
}

void mostrar_pedido(void)
{
    printf("\nMostrar pedido na tela\n");
    /*
    Aqui devemos fazer com que na tela apareça as informações dos lançamentos feitos e o nº do pedido.
    */
}

void limpar_pedido(void)
{
    void novo_n_pedido(void);

    for (int i = 0; i < 100; ++i)
    {
        l_pedido[i].N_pedido         = '\0';
        l_pedido[i].CodProd          = '\0';
        for (int ii = 0; ii < 15; ++ii)
            l_pedido[i].NomeProd[ii] = '\0';
        l_pedido[i].CustoProd        = '\0';
        l_pedido[i].Qtd              = '\0';
        l_pedido[i].SubTotal         = '\0';
    }

    indice_l = 0;

    p_pedido.N_pedido            = '\0';
    p_pedido.Total               = '\0';
    p_pedido.FormaPgto           = '\0';
    for (int ii = 0; ii < 17; ++ii)
        p_pedido.N_cartao[ii]    = '\0';
    for (int ii = 0; ii < 21; ++ii)
        p_pedido.NomeCliente[ii] = '\0';

    novo_n_pedido();
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
    int getch(void); // LINUX
    void vendas(void);
    void cobranca(void);

    if(l_pedido[0].N_pedido == '\0')
    {
        system("clear || cls");
        printf("\nNão há itens lançados no pedido.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        vendas();
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

void cobranca(void)
{
    void mostrar_pedido(void);
    int getch(void); //LINUX
    void vendas(void);
    void pgto_dinheiro(void);
    void pgto_cheque(void);
    void pgto_debito(void);
    void pgto_credito(void);

    char opcao;

    do
    {
        printf("\n===FORMA DE PAGAMENTO===\n");

        mostrar_pedido();

        printf("\nOpções:\n");
        printf("(0) - Lançar itens/Cancelar pedido\n");
        printf("(1) - Dinheiro\n");
        printf("(2) - Cheque\n");
        printf("(3) - Cartão de Débito\n");
        printf("(4) - Cartão de Crédito\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando para a MONTAGEM DO PEDIDO...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                vendas();
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
                printf("\nOpção inválida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3' && opcao != '4');

}

void pgto_dinheiro(void)
{
    void limpa_buffer(void);
    int getch(void);
    void cobranca(void);
    void get_string (char linha[]);
    void upload_pedido(void);

    float valor_recebido, troco;
    char conf_troco;

    printf("\n===PAGAMENTO EM DINHEIRO===\n");
    printf("\nValor a receber: %.2f\n", p_pedido.Total);
    printf("\nInsira o valor recebido (digite -1 para voltar): ");
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
        printf("\n===PAGAMENTO EM DINHEIRO===\n");
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
            printf("\n===PAGAMENTO EM DINHEIRO===\n");
            printf("\nValor recebido: %.2f\n", valor_recebido);
            printf("\nValor do troco: %.2f\n", troco);
            printf("\nTroco entregue? (0 - voltar / 1 - sim) ");
            scanf("%c", &conf_troco);
            limpa_buffer();
        } while (conf_troco != '0' && conf_troco != '1');

        switch(conf_troco)
        {
            case '0':
                system("clear || cls");
                printf("\nVoltando para pagamento em DINHEIRO...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                troco = '\0';
                valor_recebido = '\0';
                conf_troco = '\0';
                pgto_dinheiro();
                break;
            case '1':
                system("clear || cls");
                printf("\n===FINALIZACAO DE PEDIDO===\n");
                printf("\nDigite o nome do CLIENTE: ");
                get_string (p_pedido.NomeCliente);
                p_pedido.FormaPgto = '1';
                for (int i = 0; i < 16; ++i)
                    p_pedido.N_cartao[i] = '0';
                p_pedido.N_cartao[16] = '\0';
                printf("\nPedido CONFIRMADO!\n");
                printf("\nPagamento REALIZADO!\n");
                troco = '\0';
                valor_recebido = '\0';
                conf_troco = '\0';
                upload_pedido();
                printf("\nPressione qualquer tecla para montar um NOVO PEDIDO...");
                getch();
                system("clear || cls");
                vendas();
                break;
        }
    }

}

void pgto_cheque(void)
{
    void limpa_buffer(void);
    int getch(void);
    void cobranca(void);
    void get_string (char linha[]);
    void upload_pedido(void);

    char conf_pgto;

    do
    {
        system("clear || cls");
        printf("\n===PAGAMENTO EM CHEQUE===\n");
        printf("\nO cheque foi recebido? (0 - voltar / 1 - sim) ");
        scanf("%c", &conf_pgto);
        limpa_buffer();
    } while (conf_pgto != '0' && conf_pgto != '1');

    switch(conf_pgto)
    {
        case '0':
            system("clear || cls");
            printf("\nVoltando para FORMAS DE PAGAMENTO...\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch();
            system("clear || cls");
            conf_pgto = '\0';
            cobranca();
            break;
        case '1':
            system("clear || cls");
            printf("\n===FINALIZACAO DE PEDIDO===\n");
            printf("\nDigite o nome do CLIENTE: ");
            get_string (p_pedido.NomeCliente);
            p_pedido.FormaPgto = '2';
            for (int i = 0; i < 16; ++i)
                p_pedido.N_cartao[i] = '0';
            p_pedido.N_cartao[16] = '\0';
            printf("\n%s\n", p_pedido.N_cartao);
            printf("\nPedido CONFIRMADO!\n");
            printf("\nPagamento REALIZADO!\n");
            conf_pgto = '\0';
            upload_pedido();
            printf("\nPressione qualquer tecla para montar um NOVO PEDIDO...");
            getch();
            system("clear || cls");
            vendas();
            break;
        }
}

void pgto_debito(void)
{
    printf("\nVocê escolheu DEBITO.\n");
}

void pgto_credito(void)
{
    printf("\nVocê escolheu CREDITO.\n");
}

void limpa_buffer(void) // função criada para limpar o buffer do teclado.
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void upload_pedido(void)
{
    printf("\nupload_pedido\n");

    void limpar_pedido();

    FILE *pont_lancamentos;
    FILE *pont_pagamentos;

    pont_lancamentos = fopen("lancamentos_pedidos.txt", "a");

    if (pont_lancamentos == NULL)
        printf("\nArquivo não pode ser aberto\n");
    else
    {
        for (int i = 0; i < indice_l; i++)
        {
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].N_pedido);
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].CodProd);
            fprintf(pont_lancamentos, "%s\t",   l_pedido[i].NomeProd);
            fprintf(pont_lancamentos, "%.2f\t", l_pedido[i].CustoProd);
            fprintf(pont_lancamentos, "%i\t",   l_pedido[i].Qtd);
            fprintf(pont_lancamentos, "%.2f\n", l_pedido[i].SubTotal);
        }
    }

    fclose(pont_lancamentos);

    pont_pagamentos = fopen("pagamentos_pedidos.txt", "a");

    if (pont_pagamentos == NULL)
        printf("\nArquivo não pode ser aberto\n");
    else
    {
        fprintf(pont_pagamentos, "%i\t",     p_pedido.N_pedido);
        fprintf(pont_pagamentos, "%.2f\t",   p_pedido.Total);
        fprintf(pont_pagamentos, "%c\t",     p_pedido.FormaPgto);
        fprintf(pont_pagamentos, "%s\t",     p_pedido.N_cartao);
        fprintf(pont_pagamentos, "%s\n",     p_pedido.NomeCliente);
    }

    fclose(pont_pagamentos);

    limpar_pedido();

}

void get_string (char linha[]) // função substitui 'gets'
{
    char c; // caractere que irá compor a string
    int i = 0; // índice do caractere

    do {
        c = getchar (); // inseri o caractere digitado no  'c'
        linha [i] = c; // inseri o caractere digitado na posição
        ++i; // próxima posição da string
    } while ( c != '\n'); // enquanto não for ENTER ele continua com a string

    linha [i - 1] = '\0'; // limpa o ENTER
}

void novo_n_pedido()
{
    printf("\nnovo n de pedido puxado.\n");

    FILE *pont_Npedido;

    pont_Npedido = fopen("n_pedido.txt", "r");

    if (pont_Npedido == NULL)
        printf("\nArquivo não pode ser aberto\n");
    else
    {
        fscanf(pont_Npedido, "%i", &n_pedido_atual);
        ++n_pedido_atual;

        fclose(pont_Npedido);

        pont_Npedido = fopen("n_pedido.txt", "w");

        fprintf(pont_Npedido, "%i", n_pedido_atual);

    }

    fclose(pont_Npedido);

}


