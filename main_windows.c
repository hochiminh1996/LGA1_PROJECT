#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
#include <conio.h> // para utilizar 'getch'
#include <unistd.h> // para utilizar 'getch'
#include <locale.h>

#define N 5
/**Gostei da sua ideia de pré-definir alguns valores.
   Criei essa parte no programa no campo chamado 'CONSTANTES DO PROGRAMA'.
   Porém, eu costumo criá-las de outra forma.
   Se você achar que tem problema podemos mudar.**/



float valor_total; // (GLOBAL) TOTAL DOS PRODUTOS SELECIONADOS
/**Essa variável consta na variável de estrutura 'p_pedido'**/



//DEFINIÇÃO DE UMA VARIÁVEL COMPOSTA HETEROGÊNEA
typedef struct{
	int cod_produto;
	char nome_produto[40];
	float custo_produto;
	int indice_exibicao;//
	int indice_unidade;
}cadastro;
/**Eu costumo criar um tipo de variável de estrutura.
   Eu apenas crio a estrutura e dou nome pra ela.
   Daí com esse tipo de estrutura eu crio diversas variáveis.
   No meu ver funciona da mesma forma, mas se vc achar que há algum
   problema me fala pra gente mudar.**/



//ELABORADO UM VETOR DO TIPO CADASTRO, 5PO
cadastro lista[N] = {
	{0,"HAMBÚRGUER",40.20},
	{1,"COCA COLA",25.30},
	{2,"BATATA FRITA",4.34},
	{3,"CERVEJA",3.20},
	{4,"CAFÉ",14.20}
};
/**Essa parte que contém os produtos disponíveis eu já coloquei em
   um arquvio .DAT e criei funções para chamar os dados quando se
   executa o programa.**/



//TOPO DA PÁGINA
void topo(){
	printf("----------------------------------------------------------");
	printf("\n|CÓDIGO_DO_PRODUTO | NOME_DO_PRODUTO  | CUSTO_DO_PRODUTO |\n");
	printf("----------------------------------------------------------");
	printf("\n");
}
/**Já inseri essa função no programa, alterei o nome para ficar mais
   específico já que poderíamos ter o topo do menu também.
   Também reduzi o tamanho dele pois começou a ocupar muito espaço
   de tela.**/



//EXIBICAÇÃO DOS PRODUTOS / VALORES / CÓDIGO CADASTRADOS NO VETOR LISTA.
void exibicao(){
	int i;
	printf("\n");
	for(i=0;i<N;i++){
		printf("%i%30s  %17.2f \n\n",lista[i].cod_produto,lista[i].nome_produto,lista[i].custo_produto);
	}
	printf("----------------------------------------------------------\n");
}
/**Também inseri essa função no programa, acrescentei algumas linhas para
   para que evite problemas de formatação na impressão das informações.
   E também separei o rodapé para uma outra função pois eu chamo a função
   de impressão de lançamento de duas maneiras diferentes: todas ou um
   específico.**/



//TELA INICIAL.
/**Achei o nome 'tela_inicial' mais apropriado, então coloquei lá**/
int main (void)
{
	setlocale(LC_ALL,"PORTUGUESE");
   // void vendas(void);

    principal();

    return 0;
}


//FUNÇÃO LANÇAMENTOS
void reg_lancamento(void)
{
    topo();
    exibicao();
   	calculo_produto();
}
/**Acrescentei também algumas linhas para dar um UP da visualização
   do usuário.**/



// PRINCIPAL É O MESMO QUE A FUNÇÃO VENDAS QUE FOI COMENTADA POR APRESENTAR ERROS NO WINDOWS.
int principal(int pedidos){

	char opcao;
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);

	if(valor_total != 0){
		//IRÁ CHAMAR A LISTA DE PEDIDOS APENAS SE HOUVER ALGUM VALOR ARMAZENADO NA VARIÁVEL GLOBAL
		exibe_pedidos(pedidos);
	}


	do
    {
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

/*void vendas(void)
{
    void mostrar_pedido(void);
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);
    void confirmar_pedido(void);
    char opcao;
        	exibe_pedidos();

    do
    {
       // mostrar_pedido();


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
*/


//ESTA FUNÇÃO É REFERENTE AOS PRODUTOS QUE DEVERÃO SER ESCOLHIDOS PELO CLIENTE
int calculo_produto(){
	int i=0,indice=0,unit=0,pedidos=0,contador=0;
	char opcao,letra;
	float total=0,soma_total=0;


	printf("\n");

	printf("QUANTIDADE DE PEDIDOS : ");
	scanf("%i",&pedidos);
	/**Eu preferi tirar essa parte porque olhando na experiência do usuário
	   é bem difícil a pessoa já ter em mente quantos pedidos ela fará.
	   Para resolver o problema eu coloquei um limite de 100 lançamentos e
	   também criei um mecanismo para ela informar que não quer mais
	   lançar pedidos.
	   Eu não criei um bloqueio caso ela atinja 100 lançamentos, se quiser,
	   podemos inserir.**/



    system("clear || cls");

	if(pedidos ==0){//IRÁ CHAMAR A PRÓPRIA FUNÇÃO SE FOR DIGITADO O VALOR 0
		topo();
		exibicao();
		calculo_produto();
	}

	for(i=0;i<pedidos;i++){
		total = 0;
		indice = 0;
		/**Fiz uma diferenciação do índice (N que se encontra no vetor)
		   para o CodProd para que os CodProd sejam valores que
		   organizam os próprios produtos.**/
		unit = 0;
		contador++;// APENAS PARA ENUMERAR OS PEDIDOS.
        /**Como os produtos são gravados uma variável de estrutura vetor,
           é possível acompanhar o N do lançamento pelo índice dele.**/



		topo();
		exibicao();
		if(soma_total!=0){
			printf("PEDIDOS : %i\nSUBTOTAL : %.2f\n",pedidos,soma_total);
			printf("----------------------------------------------------------\n");
		}
		/**Eu adicionei aqui a apresentação de todos os lançamentos até
		   o momento. Assim, o usuário poderá acompanhar com detalhes o
		   que ele já inseriu.**/

		printf("\nPEDIDO %i\n",contador);
		printf("SELECIONE O CÓDIGO DO PRODUTO : ");
		scanf("%i",&indice);
		/**Inseri essa parte já e acrescentei nessa parte um meio dele
		   encerrar o registro de lançamentos.
		   Se ele digitar -1 volta para a TELA PRINCIPAL.
		   E como o índice não é o próprio código, fiz uma função que
		   busca qual é o índice do produto no 'menu' pelo código.
		   Por isso aqui ele já valida se o código é válido.**/


		if((indice >= 0) && (indice<=4)){


			printf("DIGITE O NÚMERO DE UNIDADES : ");
			scanf("%i",&unit);
			/**Também já inseri essa parte no progrma.
			   A diferença é que ela registra diretamente na linha do
               lançamento que está sendo realizado.**/

			lista[i].indice_exibicao = indice;
			lista[i].indice_unidade = unit;
			/**Essa parte de registro também inseri.**/


			if(unit == 0){
				unit = 1;
            /**A validação que usei aqui é que o usuário só pode inserir
               de 1 a 100 na quantidade.**/
			}

			total = lista[indice].custo_produto * unit;
			/**Essa parte de registro também inseri.**/



			printf("\nNOME DO PRODUTO : %s\n",lista[indice].nome_produto);
		    printf("PREÇO UNITÁRIO : R$ %.2f\n",lista[indice].custo_produto);
		    printf("UNIDADES ADQUIRIDA : %i\n",unit);
			printf("SUBTOTAL TOTAL : R$ %.2f\n",total);
			/**Como disse anteriormente, criei a função que sempre traz na
			   tela para o usuário o que ele pediu.
			   E no final de um lançamento é perguntado se ele deseja
			   confirmar.**/


			getch();
			system("clear || cls");

			}else{
				printf("\n***OPÇÃO INVÁLIDA***\n");
				system("pause");
            	system("clear || cls");

				/*
					FOI CHAMADO A ESTRUTURA BÁSICA DA PÁGINA POR CAUSA DO CLS
				*/
				indice = 0;
				i = 0;
				pedidos = 0;
				/**Como há diversos pontos onde o usuário pode querer cancelar ou
				   errar o lançamento, eu criei uma função que fizesse essa
				   'renovação' dos valores.
				   É um pouco diferente as variáveis que ele 'renova', mas
				   mas utilizei essa ideia sua.**/
				/*
					VALORES ZERADOS PARA NÃO INTERFERIREM NO LAÇO
				*/
				topo();
				exibicao();
				calculo_produto();//CHAMA A PRÓPRIA FUNÇÃO SE SELECIONAR O CÓDIGO ERRADO
			}

			soma_total += total;// calculando o valor total
			valor_total = soma_total; // GLOBAL QUE ARMAZENA O VALOR COMPLETO.
			/**Inseri essa soma lá também, mas já coloquei que juntasse direto.**/
	}

	//SÓ IRÁ APRESENTAR O RESULTADO SE A VARIÁVEL CONTER ALGUM VALOR.
	if(soma_total!=0){
		topo();
		exibicao();
		printf("TOTAL : %.2f\n",soma_total);
		printf("----------------------------------------------------------\n");
	}
	/**Dentro da apresentação do pedido que aparece diversas vezes já consta também
	   o valor Total do pedido.**/
	getche();
	system("clear || cls");

	principal(pedidos);
	/*
		ESTÁ ENVIADO A QUANTIDADE DE PEDIDOS PARA PRINCIPAL(VENDAS) QUE SERÁ USADA EM EXIBIR PRODUTOS(exibe_pedidos)
	*/

}


//IRÁ EXIBIR OS PEDIDOS SELECIONADOS PELO CLIENTE | ESTÁ SENDO USADO NO MENU INICIAR
int exibe_pedidos(int quant){
	int i,cont;
	// QUANT É A QUANTIDADE DE PEDIDOS ESCOLHIDOS.

	printf("----------------------------------------------------------");
	printf("\n\t\t\t|PEDIDOS|\n");
	printf("----------------------------------------------------------");
	printf("\n");


	for(i=0;i<quant;i++){
		cont++;// APENAS IRÁ ENUMERAR OS VALORES
		printf("%iº : ",cont);


		/*
			IRÁ RECUPERAR APENAS OS CÓDIGOS DOS PRODUTOS ARMAZENADOS NA V.H INDICE_UNIDADE
			COM ESTE VALORES, A CONDIÇÃO VERIFICA QUAL TIPO DE PRODUTO, ALÉM DA QUANTIDADE DE UNIDADES DIGITADAS.
		*/
		if(lista[i].indice_exibicao==0){
			printf("PRODUTO : HAMBÚRGUER\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}else if(lista[i].indice_exibicao==1){
			printf("PRODUTO : COCA COLA\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}else if(lista[i].indice_exibicao==2){
			printf("PRODUTO : BATATA FRITA\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}else if(lista[i].indice_exibicao==3){
			printf("PRODUTO : CERVEJA\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}else if(lista[i].indice_exibicao==4){
			printf("PRODUTO : CAFÉ\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}
	}
	printf("PREÇO TOTAL : %.2f\n",valor_total);
	printf("----------------------------------------------------------\n");

	return 0;
}
/**A disposição dos itens eu me baseei da sua, mas reduzi o espaço como disse lá em cima.**/


//MÓDULO DE PAGAMENTO | INCOMPLETO
int confirmar_pedido(){
	int pagamento=0,parcela=0;

	system("cls");
	printf("\n>>>MODO DE PAGAMENTO<<<\n\n");
	printf("1 ) DINHEIRO\n2 ) CARTÃO DE CRÉDITO\n3 ) CHEQUE\n\n");
	printf("SELECIONA UMA OPÇÃO VÁLIDA : ");
	scanf("%i",&pagamento);


	switch(pagamento){
		case 1 : printf("DINHEIRO");
		break;

		case 2 : printf("CARTÃO DE CRÉDITO");
		break;

		case 3 : printf("CHEQUE");
		break;
		default : printf("OPÇÃO INVÁLIDA");
		getche();
		confirmar_pedido(0);
	}

}
/**Sobre essa parte relaxa que eu ia montar mesmo. Precisava dela para fazer a parte da 'cobranca'**/

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


void rm_lancamento(void)
{
    printf("\nVocê escolheu REMOVER lançamento.\n");
    /*
    Este seria o sub-módulo 'rm_lancamento' que tem no mapa lógico.
    Creio que deva ser montado com base nele.
    */
}



