//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: controle de cardapio

Descricao:
Programa que ira consultar, adicionar e remover os produtos de um cardapio
que esta armazenado num arquivo .DAT. Desenvolvido dentro de um sistema
oprecional linux.

Desenvolvedor(es):
Leandro Paiva Higa  github: leandrophiga

Última atualizacao 21-out.-2020 as 14h56
*/
//--------------------------------------
/**BIBLIOTECAS UTILIZADAS**/
//--------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // Linux (criacao de getch)
#include <unistd.h> // Linux (criacao de getch)
//#include <conio.h> // Windows (getch)
#include <string.h>

//--------------------------------------
/**CONSTANTES DO PROGRAMA**/
//--------------------------------------

const int   t_nome_prod    = 15;     // tamanho da string 'nome_prod'
const int   max_menu       = 100;    // maximo de itens no da variavel 'menu'
const int   min_cod_prod   = 10;     // valor minimo de 'cod_prod'
const int   max_cod_prod   = 99;     // valor maximo de 'cod_prod'
const float min_custo_prod = 0.01;   // valor minimo de 'custo_prod'
const float max_custo_prod = 100.00; // valor maximo de 'custo_prod'
const int   min_tempo_prod = 1;      // valor minimo de 'tempo_prod' em minutos
const int   max_tempo_prod = 60;     // valor maximo de 'tempo_prod' em minutos
const char  menu_dat[]     = {"MENU.DAT"}; // nome do arquivo que se encontra dos dados sobre o menu

//--------------------------------------
/**ESTRUTURAS UTILIZADAS**/
//--------------------------------------

struct produto // estrutura para formar a lista de produtos disponiveis 'menu'
{
    int   cod_prod; // codigo do produto (de 10 a 99)
    char  nome_prod[t_nome_prod]; // nome do produto (max caracteres: 14 / 15 = '\0') nao pode haver espacos
    float custo_prod; // custo do produto (de 0.01 a 100.00)
    int   tempo_prod; // tempo de preparo do produto (de 1 a 60)
};

//--------------------------------------
/**FORMACAO DAS VARIAVEIS GLOBAIS**/
//--------------------------------------

struct produto menu[max_menu] = {}; // variavel de estrutura que guardara os produtos disponiveis

int qtd_produtos;

struct produto novo_produto; // variavel que guardara os dados sobre o novo produto que esta sendo inserido

//--------------------------------------
/**FUNCAO PRINCIPAL**/
//--------------------------------------

int main (void)
/*Objetivo da funcao
Funcao principal que sera acionada no inicio do programa.
*/
{
    //funcoes utilizadas
    void import_menu(void);
    void controle_menu(void);

    import_menu(); // fara o download do menu atual no arquivo .DAT

    controle_menu(); // iniciara a tela inicial do programa

    return 0;
}

//--------------------------------------
/**MODULOS**/
//--------------------------------------

void controle_menu (void)
/*Objetivo da funcao
Modulo principal do programa que inicia a TELA INICIAL, apresenta e encaminha
o usuario para as funcionalidades desejadas.*/
{
    //funcoes utilizadas
    int getch(void);
    void ver_menu(void);
    void inserir_produto(void);
    void remover_produto(void);

    //variaveis locais
    char opcao;

    do
    {
        system("clear || cls"); // limpa a tela para sensacao de nova pagina

        // apresentacao das opcoes ao usuario
        printf("\nBem-vindo ao controle de cardapio.\n");
        printf("\nDigite a opcao desejada.\n");
        printf("\n(0) - Ver o menu atual\n");
        printf(  "(1) - Inserir um novo produto\n");
        printf(  "(2) - Remover um produto existente\n");
        printf(  "ESC - Sair\n");

        opcao = getch(); // capta qual eh a opcao escolhida pelo usuario

        switch (opcao) // encaminha o usuario para opcao escolhida
        {
            case '0': // opcao de ver o menu atual
                ver_menu(); // mostra o menu atual na tela
                opcao = 0; // limpa a opcao escolhida
                break;

            case '1': // opcao de inserir um novo produto ao menu
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nDirecionando para INSERIR produto.\n");
                printf("\nPressione qualquer tecla para continuar...");
                opcao = 0; // limpa a opcao escolhida
                getch(); // pausa a tela para leitura do usuario
                inserir_produto(); // direciona para o submodulo INSERIR PRODUTO
                break;

            case '2':
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nDirecionando para REMOVER produto.\n");
                printf("\nPressione qualquer tecla para continuar...");
                opcao = 0; // limpa a opcao escolhida
                getch(); // pausa a tela para leitura do usuario
                remover_produto(); // direciona para o submodulo REMOVER PRODUTO
                break;

            case 27:
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nSaindo do sistema.\n");
                printf("\nPressione qualquer tecla para continuar...");
                getch(); // pausa a tela para leitura do usuario
                break;

            default:
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nOpcao invalida.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa a tela para leitura do usuario
                break;
        }

    } while (opcao != 27);
    /*Enquanto o usuario nao apertar ESC para sair do programa
    ele continuara em execucao*/
}

//--------------------------------------
/**SUBMODULOS - CONTROLE DE CARDAPIO**/
//--------------------------------------

void ver_menu (void)
/*Objetivo da funcao
Imprimir na tela o menu atual com os dados sobre cada um dos produtos.
*/
{
    // funcoes utilizadas
    int getch(void);
    void mostrar_menu(void);

    system("clear || cls"); // limpa a tela para sensacao de nova pagina

    mostrar_menu(); // imprime o menu atual na tela

    printf("\nPressione qualquer tecla para voltar...");
    getch(); // pausa a tela para leitura do usuario
}

void inserir_produto (void)
/*Objetivo da funcao
Funcao para permitir ao usuario registrar um novo produto ao menu.*/
{
    //funcoes utilizadas
    void mostrar_menu(void);
    void limpa_buffer(void);
    void limpar_novo_produto(void);
    int  getch(void);
    bool validar_cod_prod(int cod_prod);
    void get_string(char linha[]);
    bool validar_nome_prod(char nome_prod[]);
    void formatar_string(char linha[]);
    bool validar_custo_prod(float custo_prod);
    bool validar_tempo_prod(int tempo_prod);
    void registrar_produto(void);
    void export_menu(void);

    //variaveis locais
    bool voltar;
    char confirmar, continuar;

    do
    {
        system("clear || cls"); // limpa a tela para sensacao de nova pagina
        printf("\n===INSERIR PRODUTO===\n"); // cabecalho do submodulo

        mostrar_menu(); // imprime o menu atual na tela

        voltar = false; // por padrao ele nao permite voltar para a tela inicial

        /*Intercao com o usuario*/
        //cod_prod do novo_produto
        printf("\nDigite o codigo do novo produto [0 para voltar]: ");
        scanf("%i", &novo_produto.cod_prod);
        limpa_buffer(); // limpa o ENTER deixado pelo 'scanf'

        if (novo_produto.cod_prod == 0) // se o usuario quiser retornar a tela inicial
        {
            limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
            system("clear || cls"); // limpa a tela para sensacao de pop-up
            printf("\nVoltando para TELA PRINCIPAL.\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; // fara com que termine a execucao do submodulo  volte a tela inicial
            getch(); // pausa a tela para leitura do usuario
        }
        /*Sera verificado se o cod_prod digitado eh valido*/
        else if (! validar_cod_prod(novo_produto.cod_prod)) // caso nao seja valido
        {
            limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
            system("clear || cls"); // limpa a tela para sensacao de pop-up
            printf("\nCODIGO de produto invalido.\n");
            printf("\nVerifique se:\n");
            printf("\n- Nao esta utilizando um codigo ja utilizado;");
            printf("\n- O valor do codigo esta entre %i a %i;\n", min_cod_prod, max_cod_prod);
            printf("\nPressione qualquer tecla para voltar...");
            getch(); // pausa a tela para leitura do usuario
        }
        else
        {
            /*Interacao com o usuario*/
            //nome_prod do novo_produto
            printf("\nDigite o nome do novo produto [0 para voltar]: ");
            get_string(novo_produto.nome_prod);

            if (novo_produto.nome_prod[0] == '0') // se o usuario quiser retornar a tela inicial
            {
                limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nVoltando para TELA PRINCIPAL.\n");
                printf("\nPressione qualquer tecla para voltar...");
                voltar = true; // fara com que termine a execucao do submodulo  volte a tela inicial
                getch(); // pausa a tela para leitura do usuario
            }
            else
            {
                formatar_string(novo_produto.nome_prod); // remove espacos e transforma para letras maiusculas

                /*Sera verificado se o nome_prod digitado eh valido*/
                if (! validar_nome_prod(novo_produto.nome_prod)) // caso nao seja valido
                {
                    limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                    system("clear || cls"); // limpa a tela para sensacao de pop-up
                    printf("\nNOME de produto invalido.\n");
                    printf("\nVerifique se:\n");
                    printf("\n- Nao esta utilizando um nome ja utilizado;");
                    printf("\n- Nao ha uso de caracteres especiais;");
                    printf("\n- O nome nao passa de %i caracteres;\n", t_nome_prod - 1);
                    printf("\nPressione qualquer tecla para voltar...");
                    getch(); // pausa a tela para leitura do usuario
                }
                else
                {
                    /*Intercao com o usuario*/
                    //custo_prod do novo_produto
                    printf("\nDigite o custo do novo produto [0 para voltar]: ");
                    scanf("%f", &novo_produto.custo_prod);
                    limpa_buffer(); // limpa o ENTER deixado pelo 'scanf'

                    if (novo_produto.custo_prod == 0) // se o usuario quiser retornar a tela inicial
                    {
                        limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                        system("clear || cls"); // limpa a tela para sensacao de pop-up
                        printf("\nVoltando para TELA PRINCIPAL.\n");
                        printf("\nPressione qualquer tecla para voltar...");
                        voltar = true; // fara com que termine a execucao do submodulo  volte a tela inicial
                        getch(); // pausa a tela para leitura do usuario
                    }
                    /*Sera verificado se o custo_prod digitado eh valido*/
                    else if (! validar_custo_prod(novo_produto.custo_prod)) // caso nao seja valido
                    {
                        limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                        system("clear || cls"); // limpa a tela para sensacao de pop-up
                        printf("\nValor de CUSTO invalido.\n");
                        printf("\nValor minimo: %.2f\n", min_custo_prod);
                        printf(  "Valor maximo: %.2f\n", max_custo_prod);
                        printf("\nPressione qualquer tecla para voltar...");
                        getch(); // pausa a tela para leitura do usuario
                    }
                    else
                    {
                        /*Intercao com o usuario*/
                        //tempo_prod do novo_produto
                        printf("\nDigite o tempo de preparo do novo produto em minutos [0 para voltar]: ");
                        scanf("%i", &novo_produto.tempo_prod);
                        limpa_buffer(); // limpa o ENTER deixado pelo 'scanf'

                        if (novo_produto.tempo_prod == 0) // se o usuario quiser retornar a tela inicial
                        {
                            limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                            system("clear || cls"); // limpa a tela para sensacao de pop-up
                            printf("\nVoltando para TELA PRINCIPAL.\n");
                            printf("\nPressione qualquer tecla para voltar...");
                            voltar = true; // fara com que termine a execucao do submodulo  volte a tela inicial
                            getch(); // pausa a tela para leitura do usuario
                        }
                        /*Sera verificado se o tempo_prod digitado eh valido*/
                        else if (! validar_tempo_prod(novo_produto.tempo_prod)) // caso nao seja valido
                        {
                            limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                            system("clear || cls"); // limpa a tela para sensacao de pop-up
                            printf("\nValor de TEMPO invalido.\n");
                            printf("\nValor minimo: %i\n", min_tempo_prod);
                            printf(  "Valor maximo: %i\n", max_tempo_prod);
                            printf("\nPressione qualquer tecla para voltar...");
                            getch(); // pausa a tela para leitura do usuario
                        }
                        else
                        {
                            do
                            {
                                /*Intercao com o usuario*/
                                //confirmacao do registro do novo produto
                                printf("\nConfirmar o registro do produto [0 - nao/ 1 - sim]: ");
                                scanf("%c", &confirmar);
                                limpa_buffer(); // limpa o ENTER deixado pelo 'scanf'
                            } while (confirmar != '0' && confirmar != '1'); // permanece a execucao ate que se obtenha uma resposta valida

                            if (confirmar == '0')
                            {
                                limpar_novo_produto(); // limpa o que ele tiver registrado ate o momento
                                system("clear || cls"); // limpa a tela para sensacao de pop-up
                                printf("\nPressione qualquer tecla para voltar...");
                                getch(); // pausa a tela para leitura do usuario
                            }
                            else
                            {
                                registrar_produto(); // registra no menu o novo produto definitivamente
                                limpar_novo_produto(); // limpa o que ele tiver registrado realizado para um novo registro
                                system("clear || cls"); // limpa a tela para sensacao de pop-up
                                printf("\nProduto REGISTRADO.\n");
                                printf("\nPressione qualquer tecla para voltar...");
                                getch(); // pausa a tela para leitura do usuario
                                export_menu(); // faz o upload do menu atual
                            }
                        }
                    }
                }
            }
        }
    } while (! voltar);
    /*o submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true.*/
}

void remover_produto (void)
/*Objetivo da funcao
Funcao para permitir ao usuario remover um produto existente no menu.*/
{
    // funcoes utilizadas
    void mostrar_menu(void);
    void limpa_buffer(void);
    int  getch(void);
    int  buscar_cod_prod(int cod_prod);
    void mostrar_produto(int cod_prod);
    void limpar_produto(int cod_prod);
    void export_menu(void);

    // variaveis locais
    int cod_prod_digitado;
    char conf_rm;
    bool voltar;

    do
    {
        system("clear || cls"); // limpa a tela para sensacao de nova pagina
        printf("\n===REMOVER PRODUTO===\n"); // cabecalho do submodulo

        mostrar_menu(); // imprime o menu atual na tela

        voltar = false; // por padrao ele nao permite voltar para a tela inicial

        /*Intercao com o usuario*/
        //cod_prod do novo_produto
        printf("\nDigite o codigo do novo produto [0 para voltar]: ");
        scanf("%i", &cod_prod_digitado);
        limpa_buffer(); // limpa o ENTER deixado pelo 'scanf'

        if (cod_prod_digitado == 0)
        {
            cod_prod_digitado = '\0';
            system("clear || cls"); // limpa a tela para sensacao de pop-up
            printf("\nVoltando para TELA PRINCIPAL.\n");
            printf("\nPressione qualquer tecla para voltar...");
            voltar = true; // fara com que termine a execucao do submodulo  volte a tela inicial
            getch(); // pausa a tela para leitura do usuario
        }
        else if (buscar_cod_prod(cod_prod_digitado) == -1)
        {
            cod_prod_digitado = '\0';
            system("clear || cls"); // limpa a tela para sensacao de pop-up
            printf("\nNao ha produtos com o CODIGO digitado.\n");
            printf("\nPressione qualquer tecla para voltar...");
            getch(); // pausa a tela para leitura do usuario
        }
        else
        {
            do
            {
                system("clear || cls");
                printf("\nO seguinte produto sera REMOVIDO:\n");
                mostrar_produto(cod_prod_digitado);
                printf("\nDeseja continuar (0 - nao / 1 - sim)?\n");
                conf_rm = getch();
            } while (conf_rm != '0' && conf_rm != '1');

            if (conf_rm == '0')
            {
                cod_prod_digitado = '\0';
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nProcedimento CANCELADO.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa a tela para leitura do usuario
            }
            else
            {
                limpar_produto(cod_prod_digitado);
                system("clear || cls"); // limpa a tela para sensacao de pop-up
                printf("\nProduto REMOVIDO.\n");
                printf("\nPressione qualquer tecla para voltar...");
                getch(); // pausa a tela para leitura do usuario
                export_menu();
            }
        }
    } while (! voltar);
    /*o submodulo continuara a executar ate que uma das opcoes escolhidas pelo usuario
    faca-o terminar a execucao com voltar = true.*/
}

//--------------------------------------
/**FUNCOES AUXILIARES**/
//--------------------------------------

/// Manipulacao de arquivos

void import_menu (void)
/*Objetivo da funcao
*/
{
    // funcoes utilizadas
    int getch(void);

    // variaveis locais
    FILE *pont_menu;

    pont_menu = fopen (menu_dat, "rb");

    if (pont_menu == NULL)
    {
        system("clear || cls");
        printf("\nArquivo nao encontrado.\n");
        getch();
    }
    else
    {
        qtd_produtos = fread(menu, sizeof(struct produto), max_menu, pont_menu);

        if (qtd_produtos == 0)
        {
            system("clear || cls");
            printf("\nNao ha produtos listados no arquivo.\n");
            getch();
        }
        else
        {
            system("clear || cls");
            printf("\nForam IMPORTADOS %i produtos para o menu.\n", qtd_produtos);
            getch();
        }
    }
    fclose(pont_menu);
}

void export_menu (void)
/*Objetivo da funcao
*/
{
    // funcoes utilizadas
    int getch(void);

    // variaveis locais
    FILE *pont_menu;
    int conf_qtd;

    pont_menu = fopen (menu_dat, "wb");

    if (pont_menu == NULL)
    {
        system("clear || cls");
        printf("\nNao foi possivel exportar a lista de produtos.\n");
        getch();
    }
    else
    {
        conf_qtd = fwrite(menu, sizeof(struct produto), qtd_produtos, pont_menu);

        if (conf_qtd == 0)
        {
            system("clear || cls");
            printf("\nNao ha produtos listados no arquivo.\n");
            getch();
        }
        else
        {
            system("clear || cls");
            printf("\nForam EXPORTADOS %i produtos para arquivo.\n", qtd_produtos);
            getch();
        }
    }
    fclose(pont_menu);
}

/// Manipulacao de dados

void limpar_novo_produto (void)
/*Objetivo da funcao
A funcao deve limpar qualquer registro realizado na variavel 'novo_produto'*/
{
    novo_produto.cod_prod = '\0'; // preenche '\0' (vazio) em cod_prod

    for (int i = 0; i <= t_nome_prod; ++i) // passa por todos os caracteres de nome_prod
        novo_produto.nome_prod[i] = '\0'; // preenche '\0' (vazio) em todos os caracteres de nome_prod

    novo_produto.custo_prod = '\0'; // preenche '\0' (vazio) em custo_prod

    novo_produto.tempo_prod = '\0'; // preenche '\0' (vazio) em tempo_prod
}

void registrar_produto (void)
/*Objetivo da funcao
Guarda os valores inseridos pelo usuario sobre o novo produto dentro do menu*/
{
    // funcoes utilizadas
    void ordernar_produtos(void);

    menu[qtd_produtos].cod_prod = novo_produto.cod_prod; // guarda o cod_prod no menu

    strcpy(menu[qtd_produtos].nome_prod, novo_produto.nome_prod); // guarda o nome_prod no menu

    menu[qtd_produtos].custo_prod = novo_produto.custo_prod; // guarda o custo_prod no menu

    menu[qtd_produtos].tempo_prod = novo_produto.tempo_prod; // guarda o tempo_prod no menu

    ++qtd_produtos; // passa para o proximo indice em que podera ser registrado um novo produto

    ordernar_produtos();
}

void ordernar_produtos (void)
/*Objetivo da funcao
Funcao fara a ordenacao os produtos no menu pelo codigo do produto*/
{
    // variaveis locais
    struct produto auxiliar;

    for (int i = 0; menu[i].cod_prod != '\0'; ++i)
    {
        for (int ii = i + 1; menu[ii].cod_prod != '\0'; ++ii)
        {
            if (menu[i].cod_prod > menu[ii].cod_prod)
            {
                // guardando o cod_prod maior no variavel auxiliar
                auxiliar.cod_prod = menu[i].cod_prod;
                for (int iii = 0; iii <= t_nome_prod; ++iii)
                    auxiliar.nome_prod[iii] = menu[i].nome_prod[iii];
                auxiliar.custo_prod = menu[i].custo_prod;
                auxiliar.tempo_prod = menu[i].tempo_prod;

                // colocando o cod_prod menor no lugar do maior
                menu[i].cod_prod = menu[ii].cod_prod;
                for (int iii = 0; iii <= t_nome_prod; ++iii)
                    menu[i].nome_prod[iii] = menu[ii].nome_prod[iii];
                menu[i].custo_prod = menu[ii].custo_prod;
                menu[i].tempo_prod = menu[ii].tempo_prod;

                // colocando o cod_prod que esta no auxiliar no lugar do cod_prod menor
                menu[ii].cod_prod = auxiliar.cod_prod;
                for (int iii = 0; iii <= t_nome_prod; ++iii)
                    menu[ii].nome_prod[iii] = auxiliar.nome_prod[iii];
                menu[ii].custo_prod = auxiliar.custo_prod;
                menu[ii].tempo_prod = auxiliar.tempo_prod;
            }
        }
    }
}

void limpar_produto (int cod_prod)
/*Objetivo da funcao
Remover o produto indicado pelo codigo do produto referido*/
{
    // funcoes utilizadas
    int buscar_cod_prod(int cod_prod);
    int getch(void);

    //variaveis locais
    int i; // indice do produto selecionado

    i = buscar_cod_prod(cod_prod);

    if (i == -1)
    {
        system("clear || cls"); // limpara a tela para sensacao de pop-up
        printf("\nNao foi possivel encontrar o produto.\n");
        printf("\nPressione qualquer tecla para voltar...");
        getch(); // pausa para leitura da mensagem pelo usuario
    }
    else
    {
        menu[i].cod_prod = '\0'; // preenche '\0' (vazio) em cod_prod
        for (int ii = 0; ii <= t_nome_prod; ++ii) // passa por todos os caracteres de nome_prod
            menu[i].nome_prod[ii] = '\0'; // preenche '\0' (vazio) em todos os caracteres de nome_prod
        menu[i].custo_prod = '\0'; // preenche '\0' (vazio) em custo_prod
        menu[i].tempo_prod = '\0'; // preenche '\0' (vazio) em tempo_prod

        while (i < qtd_produtos)
        {
            menu[i].cod_prod = menu[i + 1].cod_prod; // sobe o proximo cod_prod
            for (int ii = 0; ii <= t_nome_prod; ++ii) // passa por todos os caracteres de nome_prod
                menu[i].nome_prod[ii] = menu[i + 1].nome_prod[ii]; // sobe todos os caracteres de nome_prod
            menu[i].custo_prod = menu[i + 1].custo_prod; // sobe o proximo custo_prod
            menu[i].tempo_prod = menu[i + 1].tempo_prod; // sobe o proximo tempo_prod
            ++i; // vai para o proximo produto
        }
        --qtd_produtos;
    }
}

/// Visualizacao de dados

void mostrar_menu (void)
/*Objetivo da funcao
Imprimir na tela o menu atual*/
{
    if (qtd_produtos == 0) // se o primeiro cod_prod for '\0' (vazio), entao nao ha itens no 'menu'
        printf("\nNao ha itens no menu.\n");
    else // apresentacao das informacoes do 'menu' caso haja produtos disponiveis
    {
        printf("\n==================MENU=================="); // titulo da quadro
        printf("\nCodigo Nome do Produto  Custo Tempo(min)\n"); // cabecalho da tabela
        for (int i = 0; menu[i].cod_prod != '\0'; ++i) // ciclo que fara a impressao dos produtos disponiveis
        {
            // cod_prod
            printf("%i     ", menu[i].cod_prod); // imprime cod_prod (como tem sempre dois caracteres nao eh necessario quantidade de espacos diferentes

            // nome_prod
            for (int ii = 0; ii < t_nome_prod; ++ii) // ciclo que passa por todos os caracteres de nome_prod
            {
                if (menu[i].nome_prod[ii] != '\0') // se o caractere de nome_prod for diferente de vazio ele imprime na tela
                    printf("%c", menu[i].nome_prod[ii]);
                else // se for vazio ele imprime um espaco
                    printf(" ");
            }
            printf(" "); // um espaco para separacao da proxima informacao

            // custo_prod
            // dependendo do valor de custo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
            if (menu[i].custo_prod < 10)
                printf("  %.2f ", menu[i].custo_prod);
            else if (menu[i].custo_prod < 100)
                printf(" %.2f " , menu[i].custo_prod);
            else
                printf("%.2f "  , menu[i].custo_prod); // valor maximo de custo_prod

            // tempo_prod
            //dependendo do valor de tempo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
            if (menu[i].tempo_prod < 10)
                printf("         %i\n", menu[i].tempo_prod);
            else
                printf("        %i\n", menu[i].tempo_prod);
        }
        printf("========================================\n"); // rodape do quadro
    }
}

void mostrar_produto (int cod_prod)
/*Objetivo da funcao
Imprimir na tela as informacoes de um produto referido pelo codigo dado.*/
{
    // funcoes utilizadas
    int buscar_cod_prod(int cod_prod);

    //variaveis locais
    int i; // indice do produto selecionado

    i = buscar_cod_prod(cod_prod);

    if (i == -1)
    {
        printf("\nProduto nao encontrado.\n");
    }
    else
    {
        printf("\nCodigo Nome do Produto  Custo Tempo(min)\n");

        // cod_prod
        printf("%i     ", menu[i].cod_prod); // imprime cod_prod (como tem sempre dois caracteres nao eh necessario quantidade de espacos diferentes

        // nome_prod
        for (int ii = 0; ii < t_nome_prod; ++ii) // ciclo que passa por todos os caracteres de nome_prod
        {
            if (menu[i].nome_prod[ii] != '\0') // se o caractere de nome_prod for diferente de vazio ele imprime na tela
                printf("%c", menu[i].nome_prod[ii]);
            else // se for vazio ele imprime um espaco
                printf(" ");
        }
        printf(" "); // um espaco para separacao da proxima informacao

        // custo_prod
        // dependendo do valor de custo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
        if (menu[i].custo_prod < 10)
            printf("  %.2f ", menu[i].custo_prod);
        else if (menu[i].custo_prod < 100)
            printf(" %.2f " , menu[i].custo_prod);
        else
            printf("%.2f "  , menu[i].custo_prod); // valor maximo de custo_prod

        // tempo_prod
        //dependendo do valor de tempo_prod eh necessario uma quantidade de espacos para alinhar ao quadro
        if (menu[i].tempo_prod < 10)
            printf("         %i\n", menu[i].tempo_prod);
        else
            printf("        %i\n", menu[i].tempo_prod);
    }
}

/// Validacao de dados

bool validar_cod_prod (int cod_prod)
/*Objetivo da funcao
Verifica se o cod_prod referido pode ser utilizado*/
{
    // funcoes utilizadas
    int buscar_cod_prod(int cod_prod);

    /*retornara 'true', e portanto valido, caso o valor de cod_prod
    esteja no intervalo definido e nao seja um codigo ja utilizado.
    Para esta segunda condicao eh usado a funcao 'buscar'. Śe esta
    retorna -1 eh porque o cod_prod nao foi utilizado ainda*/
    if(cod_prod >= min_cod_prod && cod_prod <= max_cod_prod &&
       buscar_cod_prod(cod_prod) == -1)
        return true;
    else
        return false;
}

bool validar_nome_prod (char nome_prod[])
/*Objetivo da funcao
Verifica se o nome_prod referido pode ser utilizado*/
{
    // funcoes utilizadas
    int buscar_nome_prod(char nome_prod[]);

    // variaveis locais
    int cont_c_especial = 0;

    // ciclo que faz uma contagem de caracteres especiais no nome_prod
    for(int i = 0; i < t_nome_prod && nome_prod[i] != '\0'; ++i)
    {
        if((nome_prod[i] >= 48 && nome_prod[i] <= 57)  || // algarismos de 0 a 9
           (nome_prod[i] >= 65 && nome_prod[i] <= 90)  || // letras maiusculas
           (nome_prod[i] >= 97 && nome_prod[i] <= 122) || // letras minusculas
            nome_prod[i] == 95 || nome_prod[i] == 32) // espaco e underline (sao permitidos)
        {
        }
        else
            ++cont_c_especial; // somara 1 caso o caracter comparado nao seja de nenhum dos grupos acima
    }

    /*retornara 'true', e portanto valido, caso o nome_prod nao
    contenha caracteres especiais e nao seja um nome ja utilizado.
    Para esta segunda condicao eh usado a funcao 'buscar'. Śe esta
    retorna -1 eh porque o cod_prod nao foi utilizado ainda*/
    if(buscar_nome_prod(nome_prod) == -1 && cont_c_especial == 0 &&
       nome_prod[t_nome_prod - 1] == '\0')
        return true;
    else
        return false;
}

bool validar_custo_prod (float custo_prod)
/*Objetivo da funcao
Verifica se o custo_prod referido pode ser utilizado*/
{
    /*retornara 'true', e portanto valido, caso o valor de cod_prod
    esteja no intervalo definido.*/
    if(custo_prod >= min_custo_prod && custo_prod <= max_custo_prod)
        return true;
    else
        return false;
}

bool validar_tempo_prod (int tempo_prod)
/*Objetivo da funcao
Verifica se o custo_prod referido pode ser utilizado*/
{
    /*retornara 'true', e portanto valido, caso o valor de cod_prod
    esteja no intervalo definido.*/
    if(tempo_prod >= min_tempo_prod && tempo_prod <= max_tempo_prod)
        return true;
    else
        return false;
}

/// Busca de dados

int buscar_cod_prod (int cod_prod)
/*Objetivo da funcao
Retorna o valor do indice de um cod_prod especifico, caso nao encontre sera retornado -1*/
{
    int indice_cod_prod = -1; // variavel que guardara a o indice do produto procurado, ele ja eh -1 para caso nao seja encontrado o produto
    int i = 0; // indice que seja procurado, inicia em 0

    while(indice_cod_prod == -1 && menu[i].cod_prod != '\0') // ciclo que estara buscando o indice ate que se encontre ou passe por todos os itens
    {
        if(cod_prod != menu[i].cod_prod) // caso o cod_prod do 'menu' seja diferente do CodProd procurado
            ++i; // vai para o proximo indice
        else
            indice_cod_prod = i; // copia o indice na variavel que sera retornada
    }

    return indice_cod_prod; // retorna o indice do cod_prod procurado ou -1 se nao tiver sido encontrado
}

int buscar_nome_prod (char nome_prod[])
/*Objetivo da funcao
Retorna o valor do indice de um nome_prod especifico, caso nao encontre sera retornado -1*/
{
    int indice_nome_prod = -1; // variavel que guardara a o indice do produto procurado, ele ja eh -1 para caso nao seja encontrado o produto
    int i = 0; // indice que seja procurado, inicia em 0

    while(indice_nome_prod == -1 && menu[i].cod_prod != '\0') // ciclo que estara buscando o indice ate que se encontre ou passe por todos os itens
    {
        if(strcmp(nome_prod, menu[i].nome_prod) != 0) // caso o nome_prod do 'menu' seja diferente do CodProd procurado
            ++i; // vai para o proximo indice
        else
            indice_nome_prod = i; // copia o indice na variavel que sera retornada
    }

    return indice_nome_prod; // retorna o indice do nome_prod procurado ou -1 se nao tiver sido encontrado
}

//--------------------------------------
/**FUNCOES BASICAS**/
//--------------------------------------

int getch (void) // Linux - criacao da funcao 'getch' pois o sistema nao tem a biblioteca 'conio.h'
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

void limpa_buffer (void) // funcao criada para limpar o buffer do teclado que funciona em WINDOWS e LINUX
/*Objetivo da funcao
Limpar qualquer "sujeira" que tiver no buffer do teclado. So funciona se realmente houver algo no buffer
Deve-se utilizar apos a funcao 'scanf'*/
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_string (char linha[]) // funcao que funciona melhor que gets
/*Objetivo da funcao
Guarda em uma variavel do tipo string (char com vetor) o que o usuario digitar ate ele apertar ENTER*/
{
    // variaveis locais
    char c; // caractere que ira compor a string
    int i = 0; // indice do caractere

    do {
        c = getchar (); // guarda o caractere digitado na variavel 'c'
        linha [i] = c; // guarda o valor da variavel 'c' na posicao da string indicada
        ++i; // vai para a proxima posicao da string
    } while (c != '\n'); // ciclo rodara enquanto a variavel 'c' nao for ENTER

    linha [i - 1] = '\0'; // substitui o ENTER por '\0' (vazio) para finalizar a string
}

void formatar_string (char linha[])
/*Objetivo da funcao
Substitui da string referida os espacos por underline e as letras minusculas por maiusculas*/
{
    // variaveis locais
    int i = 0;

    while (linha[i] != '\0')
    {
        if (linha[i] == 32)
            linha[i] = 95; // substitui espaco por underline

        if (linha[i] >= 97 && linha[i] <= 122)
            linha[i] -= 32; // substitui a letra minuscula por sua maiuscula
        ++i; // parte para o proximo caractere
    }
}

//--------------------------------------
/**INFORMACOES BASICAS**/
//--------------------------------------
/*
Nome do projeto: Controle de cardapio

Descricao:
Programa que ira consultar, adionar e remover os produtos de um cardapio
que esta armazenado num arquivo .DAT. Desenvolvido num dentro de um sistema
oprecional linux.

Desenvolvedor(es):
Leandro Paiva Higa  github: leandrophiga

Última atualizacao 21-out.-2020 as 14h56
*/
