#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
//#include <conio.h> // WINDOWS - para utlizar 'getch'
#include <termios.h> // LINUX - para utilizar 'getch'
#include <unistd.h> // LINUX - para utilizar 'getch'
//#include <locale.h> // WINDOWS - para utilizar 'setlocale'

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

struct lancamento l_pedido[100] = {}; // cria um pedido com lancamentos limpos

struct pagamento p_pedido = {}; // cria um pedido com pagamento limpo

int indice_l = 0; // para acompanhar as linhas já escritas em 'l_pedido'

int n_pedido_atual;

int main (void)
{
//    setlocale(LC_ALL,"PORTUGUESE"); // WINDOWS - para caracteres especiais

    void download_menu(void);
    void tela_inicial(void);
    void novo_n_pedido(void);

    download_menu();

    novo_n_pedido();

    tela_inicial();

    return 0;
}

void tela_inicial(void)
{
    void mostrar_pedido(void);
    int getch(void); // LINUX
    void cancelar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);

    char opcao;

    do
    {
        printf("\n>>>TELA INICIAL\n");

        mostrar_pedido();

        printf("\nOpções:\n");
        printf("(0) - Cancelar pedido\n");
        printf("(1) - Registrar lançamentos\n");
        printf("(2) - Remover lançamentos\n");
        printf("(3) - Confirmar pedido\n");

        opcao = getch();

        switch(opcao)
        {
            case '0':
                system("clear || cls");
                cancelar_pedido();
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
            case 27:
                system("clear || cls");
                printf("\nFechando sistema...\n");
                break;
            default:
                system("clear || cls");
                printf("\nOpção inválida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3' && opcao != 27);
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
    int i = 0, ii; // índice do vetor menu e índice vetor string

    if (menu[i].CodProd == '\0') // caso não haja itens no menu
        printf("\nNão há itens no menu.\n");
    else
    {
        printf("\n============MENU=============");
        printf("\nCódigo Nome do Produto  Custo\n");
        do
        {
            printf("%i     ", menu[i].CodProd);

            ii = 0;
            while (ii < 15)
            {
                if (menu[i].NomeProd[ii] != '\0')
                    printf("%c", menu[i].NomeProd[ii]);
                else
                    printf(" ");
                ++ii;
            }
            printf(" ");

            if (menu[i].CustoProd < 10)
                printf("  %.2f\n", menu[i].CustoProd);
            else if (menu[i].CustoProd < 100)
                printf(" %.2f\n" , menu[i].CustoProd);
            else
                printf("%.2f\n"  , menu[i].CustoProd);

            ++i;
        } while (menu[i].CodProd != '\0');

        printf("=============================\n");
    }
}

void mostrar_pedido(void)
{

    int ii;

    if(indice_l == 0)
    {
        printf("\nPedido Nº %i", n_pedido_atual);
        printf("\nNão há lançamentos no pedido.\n");
    }
    else
    {
        printf("\n==================SEU PEDIDO==================");
        printf("\n                              Pedido Nº ");

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


        printf("\nCodProd Nome do Produto   Custo  Qtd  SubTotal\n");

        for(int i = 0; i < indice_l; ++i)
        {
            printf("%i      ",   l_pedido[i].CodProd);

            ii = 0;
            while (ii < 15)
            {
                if (l_pedido[i].NomeProd[ii] != '\0')
                    printf("%c", l_pedido[i].NomeProd[ii]);
                else
                    printf(" ");
                ++ii;
            }
            printf(" ");

            if (l_pedido[i].CustoProd < 10)
                printf("   %.2f ", l_pedido[i].CustoProd);
            else if (l_pedido[i].CustoProd < 100)
                printf("  %.2f " , l_pedido[i].CustoProd);
            else
                printf(" %.2f "  , menu[i].CustoProd);

            if (l_pedido[i].Qtd < 10)
                printf("   %i ", l_pedido[i].Qtd);
            else if (l_pedido[i].Qtd < 100)
                printf("  %i " , l_pedido[i].Qtd);
            else
                printf(" %i "  , l_pedido[i].Qtd);

            if (l_pedido[i].SubTotal < 10)
                printf("     %.2f\n", l_pedido[i].SubTotal);
            else if (l_pedido[i].SubTotal < 100)
                printf("    %.2f\n" , l_pedido[i].SubTotal);
            else if (l_pedido[i].SubTotal < 1000)
                printf("   %.2f\n"  , l_pedido[i].SubTotal);
            else if (l_pedido[i].SubTotal < 10000)
                printf("  %.2f\n"   , l_pedido[i].SubTotal);
            else
                printf(" %.2f\n"    , l_pedido[i].SubTotal);
        }
        printf("\n                               Total ");

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
            printf("%.2f\n"     , p_pedido.Total);
        printf("==============================================\n");
    }
}

void mostrar_lancamento(int indice_lancamento)
{
    int i = indice_lancamento, ii = 0;

    printf("\nCodProd Nome do Produto   Custo  Qtd  SubTotal\n");

    printf("%i      ",   l_pedido[i].CodProd);

    ii = 0;
    while (ii < 15)
    {
            if (l_pedido[i].NomeProd[ii] != '\0')
                printf("%c", l_pedido[i].NomeProd[ii]);
            else
                printf(" ");
            ++ii;
    }
    printf(" ");

    if (l_pedido[i].CustoProd < 10)
        printf("   %.2f ", l_pedido[i].CustoProd);
    else if (l_pedido[i].CustoProd < 100)
        printf("  %.2f " , l_pedido[i].CustoProd);
    else
        printf(" %.2f "  , menu[i].CustoProd);

    if (l_pedido[i].Qtd < 10)
        printf("   %i ", l_pedido[i].Qtd);
    else if (l_pedido[i].Qtd < 100)
        printf("  %i " , l_pedido[i].Qtd);
    else
        printf(" %i "  , l_pedido[i].Qtd);

    if (l_pedido[i].SubTotal < 10)
        printf("     %.2f\n", l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 100)
        printf("    %.2f\n" , l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 1000)
        printf("   %.2f\n"  , l_pedido[i].SubTotal);
    else if (l_pedido[i].SubTotal < 10000)
        printf("  %.2f\n"   , l_pedido[i].SubTotal);
    else
        printf(" %.2f\n"    , l_pedido[i].SubTotal);
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

void limpar_lancamento(int indice_lancamento)
{
    void calcula_total(void);

    l_pedido[indice_lancamento].CodProd = '\0';
    for (int i = 0; i < 15; ++i)
        l_pedido[indice_lancamento].NomeProd[i] = '\0';
    l_pedido[indice_lancamento].CustoProd = '\0';
    l_pedido[indice_lancamento].Qtd = '\0';

    while (l_pedido[indice_lancamento + 1].CodProd != '\0')
    {
        l_pedido[indice_lancamento].CodProd = l_pedido[indice_lancamento + 1].CodProd;
        for (int i = 0; i < 15; ++i)
            l_pedido[indice_lancamento].NomeProd[i] = l_pedido[indice_lancamento + 1].NomeProd[i];
        l_pedido[indice_lancamento].CustoProd = l_pedido[indice_lancamento + 1].CustoProd;
        l_pedido[indice_lancamento].Qtd = l_pedido[indice_lancamento + 1].Qtd;

        l_pedido[indice_lancamento + 1].CodProd = '\0';
        for (int i = 0; i < 15; ++i)
            l_pedido[indice_lancamento + 1].NomeProd[i] = '\0';
        l_pedido[indice_lancamento + 1].CustoProd = '\0';
        l_pedido[indice_lancamento + 1].Qtd = '\0';

        ++indice_lancamento;
    }

    indice_l = indice_lancamento;

    calcula_total();

}

void cancelar_pedido(void)
{
    void tela_inicial(void);

    char resposta;

    if (indice_l == 0)
    {
        printf("\nNão há lançamentos. Portanto, não é possível CANCELAR o pedido.\n");
    }
    else
    {
        do
        {
            printf("\n>>>ATENCAO!!!\n");
            printf("\nTem certeza que deseja CANCELAR o pedido? (0 - voltar / 1 - sim) ");
            resposta = getch();
        } while (resposta != '0' && resposta != '1');
        if (resposta == '1')
        {
            limpar_pedido();
            system("clear || cls");
            printf("\nPedido CANCELADO.\n");
        }
        printf("\n");
    }
    printf("\nPressione qualquer tecla para voltar...");
    getch();
    system("clear || cls");
    tela_inicial();
}

void reg_lancamento(void)
{
    void mostrar_menu(void);
    void mostrar_pedido(void);
    void limpa_buffer(void);
    void tela_inicial(void);
    int getch(void);
    void limpar_lancamento(int indice_lancamento);
    int busca_CodProd(int CodProd);
    void calcula_total(void);
    void mostrar_lancamento(int indice_lancamento);

    int indice_CodProd;
    char conf_lancamento;

    printf("\n>>>REGISTRO DE LANCAMENTOS\n");

    mostrar_menu();

    mostrar_pedido();

    printf("\nInsira o código do produto (-1 para voltar): ");
    scanf("%i", &l_pedido[indice_l].CodProd);
    limpa_buffer();

    if (l_pedido[indice_l].CodProd < 0)
    {
        limpar_lancamento(indice_l);
        system("clear || cls");
        printf("\nVoltando para a TELA INICIAL...\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch();
        system("clear || cls");
        tela_inicial();
    }
    else
    {
        indice_CodProd = busca_CodProd (l_pedido[indice_l].CodProd);

        if (indice_CodProd == -1)
        {
            system("clear || cls");
            printf("\nCódigo de produto digitado INVALIDO.\n");
            printf("\nPor favor, digite um código válido.\n");
            getch();
            system("clear || cls");
            limpar_lancamento(indice_l);
            reg_lancamento();
        }
        else
        {
            for (int i = 0; i < 15; ++i)
                l_pedido[indice_l].NomeProd[i] = menu[indice_CodProd].NomeProd[i];
            l_pedido[indice_l].NomeProd[15] = '\0';
            l_pedido[indice_l].CustoProd = menu[indice_CodProd].CustoProd;

            printf("\nInsira a quantidade desejada (-1 para voltar): ");
            scanf("%i", &l_pedido[indice_l].Qtd);
            limpa_buffer();

            if (l_pedido[indice_l].Qtd < 0)
            {
                limpar_lancamento(indice_l);
                system("clear || cls");
                printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                reg_lancamento();
            }
            else if (l_pedido[indice_l].Qtd < 1 || l_pedido[indice_l].Qtd > 100)
            {
                limpar_lancamento(indice_l);
                system("clear || cls");
                printf("\nLimite mínimo por lançamento  = 1.");
                printf("\nLimite máximo por lançamento  = 100.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                reg_lancamento();
            }
            else
            {
                l_pedido[indice_l].SubTotal = l_pedido[indice_l].CustoProd * l_pedido[indice_l].Qtd;

                do
                {
                    system("cls || clear");
                    mostrar_lancamento(indice_l);
                    printf("\nConfirmar lançamento (0 - não / 1 - sim)?\n");
                    conf_lancamento = getch();
                } while (conf_lancamento != '0' && conf_lancamento != '1');

                switch(conf_lancamento)
                {
                    case '0':
                        limpar_lancamento(indice_l);
                        system("clear || cls");
                        printf("\nVoltando para a REGISTRO DE LANCAMENTOS...\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        getch();
                        system("clear || cls");
                        reg_lancamento();
                        break;
                    case '1':
                        l_pedido[indice_l].N_pedido = n_pedido_atual;
                        ++indice_l;
                        calcula_total();
                        system("clear || cls");
                        printf("\nLançamento REGISTRADO!\n");
                        printf("\nPressione qualquer tecla para continuar...");
                        getch();
                        system("clear || cls");
                        reg_lancamento();
                        break;
                }
            }
        }
    }
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
    void tela_inicial(void);
    void cobranca(void);

    if(indice_l == 0)
    {
        system("clear || cls");
        printf("\nNão há itens lançados no pedido.\n");
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

void cobranca(void)
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

void pgto_cheque(void)
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
        printf("\nValor não correspondente.\n");
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

void novo_n_pedido(void)
{
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

int busca_CodProd(int CodProd)
{
    int indice_CodProd = -1;
    int i = 0;

    while (indice_CodProd == -1 && menu[i].CodProd != '\0')
    {
        if(CodProd != menu[i].CodProd)
            ++i;
        else
            indice_CodProd = i;
    }

    return indice_CodProd;
}

void calcula_total(void)
{
    p_pedido.Total = 0;

    for (int i = 0; i < indice_l; ++i)
        p_pedido.Total += l_pedido[i].SubTotal;
}

void finalizar_pedido(void)
{
    void tela_inicial(void);

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



