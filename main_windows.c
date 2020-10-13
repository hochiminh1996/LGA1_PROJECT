#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
#include <conio.h> // para utilizar 'getch'
#include <unistd.h> // para utilizar 'getch'
#include <locale.h>

#define N 5
/**Gostei da sua ideia de pr�-definir alguns valores.
   Criei essa parte no programa no campo chamado 'CONSTANTES DO PROGRAMA'.
   Por�m, eu costumo cri�-las de outra forma.
   Se voc� achar que tem problema podemos mudar.**/



float valor_total; // (GLOBAL) TOTAL DOS PRODUTOS SELECIONADOS
/**Essa vari�vel consta na vari�vel de estrutura 'p_pedido'**/



//DEFINI��O DE UMA VARI�VEL COMPOSTA HETEROG�NEA
typedef struct{
	int cod_produto;
	char nome_produto[40];
	float custo_produto;
	int indice_exibicao;//
	int indice_unidade;
}cadastro;
/**Eu costumo criar um tipo de vari�vel de estrutura.
   Eu apenas crio a estrutura e dou nome pra ela.
   Da� com esse tipo de estrutura eu crio diversas vari�veis.
   No meu ver funciona da mesma forma, mas se vc achar que h� algum
   problema me fala pra gente mudar.**/



//ELABORADO UM VETOR DO TIPO CADASTRO, 5PO
cadastro lista[N] = {
	{0,"HAMB�RGUER",40.20},
	{1,"COCA COLA",25.30},
	{2,"BATATA FRITA",4.34},
	{3,"CERVEJA",3.20},
	{4,"CAF�",14.20}
};
/**Essa parte que cont�m os produtos dispon�veis eu j� coloquei em
   um arquvio .DAT e criei fun��es para chamar os dados quando se
   executa o programa.**/



//TOPO DA P�GINA
void topo(){
	printf("----------------------------------------------------------");
	printf("\n|C�DIGO_DO_PRODUTO | NOME_DO_PRODUTO  | CUSTO_DO_PRODUTO |\n");
	printf("----------------------------------------------------------");
	printf("\n");
}
/**J� inseri essa fun��o no programa, alterei o nome para ficar mais
   espec�fico j� que poder�amos ter o topo do menu tamb�m.
   Tamb�m reduzi o tamanho dele pois come�ou a ocupar muito espa�o
   de tela.**/



//EXIBICA��O DOS PRODUTOS / VALORES / C�DIGO CADASTRADOS NO VETOR LISTA.
void exibicao(){
	int i;
	printf("\n");
	for(i=0;i<N;i++){
		printf("%i%30s  %17.2f \n\n",lista[i].cod_produto,lista[i].nome_produto,lista[i].custo_produto);
	}
	printf("----------------------------------------------------------\n");
}
/**Tamb�m inseri essa fun��o no programa, acrescentei algumas linhas para
   para que evite problemas de formata��o na impress�o das informa��es.
   E tamb�m separei o rodap� para uma outra fun��o pois eu chamo a fun��o
   de impress�o de lan�amento de duas maneiras diferentes: todas ou um
   espec�fico.**/



//TELA INICIAL.
/**Achei o nome 'tela_inicial' mais apropriado, ent�o coloquei l�**/
int main (void)
{
	setlocale(LC_ALL,"PORTUGUESE");
   // void vendas(void);

    principal();

    return 0;
}


//FUN��O LAN�AMENTOS
void reg_lancamento(void)
{
    topo();
    exibicao();
   	calculo_produto();
}
/**Acrescentei tamb�m algumas linhas para dar um UP da visualiza��o
   do usu�rio.**/



// PRINCIPAL � O MESMO QUE A FUN��O VENDAS QUE FOI COMENTADA POR APRESENTAR ERROS NO WINDOWS.
int principal(int pedidos){

	char opcao;
    void limpar_pedido(void);
    void reg_lancamento(void);
    void rm_lancamento(void);

	if(valor_total != 0){
		//IR� CHAMAR A LISTA DE PEDIDOS APENAS SE HOUVER ALGUM VALOR ARMAZENADO NA VARI�VEL GLOBAL
		exibe_pedidos(pedidos);
	}


	do
    {
        printf("\nOp��es:\n(0) - Cancelar pedido\n(1) - Registrar lan�amentos\n(2) - Remover lan�amentos\n(3) - Confirmar pedido\n\n");

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
                confirmar_pedido(); // ainda � necess�rio criar um verificador de n� de lan�amentos. S� poder� continuar o processo se n� de lan�amentos > 1.
                break;
            default:
                system("clear || cls");
                printf("\nOp��o inv�lida.\n\nPressione qualquer tecla para voltar...");
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


        printf("\nOp��es:\n(0) - Cancelar pedido\n(1) - Registrar lan�amentos\n(2) - Remover lan�amentos\n(3) - Confirmar pedido\n\n");

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
                confirmar_pedido(); // ainda � necess�rio criar um verificador de n� de lan�amentos. S� poder� continuar o processo se n� de lan�amentos > 1.
                break;
            default:
                system("clear || cls");
                printf("\nOp��o inv�lida.\n\nPressione qualquer tecla para voltar...");
                getch();
                system("clear || cls");
                break;
        }
    } while (opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3');
}
*/


//ESTA FUN��O � REFERENTE AOS PRODUTOS QUE DEVER�O SER ESCOLHIDOS PELO CLIENTE
int calculo_produto(){
	int i=0,indice=0,unit=0,pedidos=0,contador=0;
	char opcao,letra;
	float total=0,soma_total=0;


	printf("\n");

	printf("QUANTIDADE DE PEDIDOS : ");
	scanf("%i",&pedidos);
	/**Eu preferi tirar essa parte porque olhando na experi�ncia do usu�rio
	   � bem dif�cil a pessoa j� ter em mente quantos pedidos ela far�.
	   Para resolver o problema eu coloquei um limite de 100 lan�amentos e
	   tamb�m criei um mecanismo para ela informar que n�o quer mais
	   lan�ar pedidos.
	   Eu n�o criei um bloqueio caso ela atinja 100 lan�amentos, se quiser,
	   podemos inserir.**/



    system("clear || cls");

	if(pedidos ==0){//IR� CHAMAR A PR�PRIA FUN��O SE FOR DIGITADO O VALOR 0
		topo();
		exibicao();
		calculo_produto();
	}

	for(i=0;i<pedidos;i++){
		total = 0;
		indice = 0;
		/**Fiz uma diferencia��o do �ndice (N que se encontra no vetor)
		   para o CodProd para que os CodProd sejam valores que
		   organizam os pr�prios produtos.**/
		unit = 0;
		contador++;// APENAS PARA ENUMERAR OS PEDIDOS.
        /**Como os produtos s�o gravados uma vari�vel de estrutura vetor,
           � poss�vel acompanhar o N do lan�amento pelo �ndice dele.**/



		topo();
		exibicao();
		if(soma_total!=0){
			printf("PEDIDOS : %i\nSUBTOTAL : %.2f\n",pedidos,soma_total);
			printf("----------------------------------------------------------\n");
		}
		/**Eu adicionei aqui a apresenta��o de todos os lan�amentos at�
		   o momento. Assim, o usu�rio poder� acompanhar com detalhes o
		   que ele j� inseriu.**/

		printf("\nPEDIDO %i\n",contador);
		printf("SELECIONE O C�DIGO DO PRODUTO : ");
		scanf("%i",&indice);
		/**Inseri essa parte j� e acrescentei nessa parte um meio dele
		   encerrar o registro de lan�amentos.
		   Se ele digitar -1 volta para a TELA PRINCIPAL.
		   E como o �ndice n�o � o pr�prio c�digo, fiz uma fun��o que
		   busca qual � o �ndice do produto no 'menu' pelo c�digo.
		   Por isso aqui ele j� valida se o c�digo � v�lido.**/


		if((indice >= 0) && (indice<=4)){


			printf("DIGITE O N�MERO DE UNIDADES : ");
			scanf("%i",&unit);
			/**Tamb�m j� inseri essa parte no progrma.
			   A diferen�a � que ela registra diretamente na linha do
               lan�amento que est� sendo realizado.**/

			lista[i].indice_exibicao = indice;
			lista[i].indice_unidade = unit;
			/**Essa parte de registro tamb�m inseri.**/


			if(unit == 0){
				unit = 1;
            /**A valida��o que usei aqui � que o usu�rio s� pode inserir
               de 1 a 100 na quantidade.**/
			}

			total = lista[indice].custo_produto * unit;
			/**Essa parte de registro tamb�m inseri.**/



			printf("\nNOME DO PRODUTO : %s\n",lista[indice].nome_produto);
		    printf("PRE�O UNIT�RIO : R$ %.2f\n",lista[indice].custo_produto);
		    printf("UNIDADES ADQUIRIDA : %i\n",unit);
			printf("SUBTOTAL TOTAL : R$ %.2f\n",total);
			/**Como disse anteriormente, criei a fun��o que sempre traz na
			   tela para o usu�rio o que ele pediu.
			   E no final de um lan�amento � perguntado se ele deseja
			   confirmar.**/


			getch();
			system("clear || cls");

			}else{
				printf("\n***OP��O INV�LIDA***\n");
				system("pause");
            	system("clear || cls");

				/*
					FOI CHAMADO A ESTRUTURA B�SICA DA P�GINA POR CAUSA DO CLS
				*/
				indice = 0;
				i = 0;
				pedidos = 0;
				/**Como h� diversos pontos onde o usu�rio pode querer cancelar ou
				   errar o lan�amento, eu criei uma fun��o que fizesse essa
				   'renova��o' dos valores.
				   � um pouco diferente as vari�veis que ele 'renova', mas
				   mas utilizei essa ideia sua.**/
				/*
					VALORES ZERADOS PARA N�O INTERFERIREM NO LA�O
				*/
				topo();
				exibicao();
				calculo_produto();//CHAMA A PR�PRIA FUN��O SE SELECIONAR O C�DIGO ERRADO
			}

			soma_total += total;// calculando o valor total
			valor_total = soma_total; // GLOBAL QUE ARMAZENA O VALOR COMPLETO.
			/**Inseri essa soma l� tamb�m, mas j� coloquei que juntasse direto.**/
	}

	//S� IR� APRESENTAR O RESULTADO SE A VARI�VEL CONTER ALGUM VALOR.
	if(soma_total!=0){
		topo();
		exibicao();
		printf("TOTAL : %.2f\n",soma_total);
		printf("----------------------------------------------------------\n");
	}
	/**Dentro da apresenta��o do pedido que aparece diversas vezes j� consta tamb�m
	   o valor Total do pedido.**/
	getche();
	system("clear || cls");

	principal(pedidos);
	/*
		EST� ENVIADO A QUANTIDADE DE PEDIDOS PARA PRINCIPAL(VENDAS) QUE SER� USADA EM EXIBIR PRODUTOS(exibe_pedidos)
	*/

}


//IR� EXIBIR OS PEDIDOS SELECIONADOS PELO CLIENTE | EST� SENDO USADO NO MENU INICIAR
int exibe_pedidos(int quant){
	int i,cont;
	// QUANT � A QUANTIDADE DE PEDIDOS ESCOLHIDOS.

	printf("----------------------------------------------------------");
	printf("\n\t\t\t|PEDIDOS|\n");
	printf("----------------------------------------------------------");
	printf("\n");


	for(i=0;i<quant;i++){
		cont++;// APENAS IR� ENUMERAR OS VALORES
		printf("%i� : ",cont);


		/*
			IR� RECUPERAR APENAS OS C�DIGOS DOS PRODUTOS ARMAZENADOS NA V.H INDICE_UNIDADE
			COM ESTE VALORES, A CONDI��O VERIFICA QUAL TIPO DE PRODUTO, AL�M DA QUANTIDADE DE UNIDADES DIGITADAS.
		*/
		if(lista[i].indice_exibicao==0){
			printf("PRODUTO : HAMB�RGUER\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
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
			printf("PRODUTO : CAF�\nQUANTIDADE : %i UNIDADES\n",lista[i].indice_unidade);
			printf("----------------------------------------------------------\n");
		}
	}
	printf("PRE�O TOTAL : %.2f\n",valor_total);
	printf("----------------------------------------------------------\n");

	return 0;
}
/**A disposi��o dos itens eu me baseei da sua, mas reduzi o espa�o como disse l� em cima.**/


//M�DULO DE PAGAMENTO | INCOMPLETO
int confirmar_pedido(){
	int pagamento=0,parcela=0;

	system("cls");
	printf("\n>>>MODO DE PAGAMENTO<<<\n\n");
	printf("1 ) DINHEIRO\n2 ) CART�O DE CR�DITO\n3 ) CHEQUE\n\n");
	printf("SELECIONA UMA OP��O V�LIDA : ");
	scanf("%i",&pagamento);


	switch(pagamento){
		case 1 : printf("DINHEIRO");
		break;

		case 2 : printf("CART�O DE CR�DITO");
		break;

		case 3 : printf("CHEQUE");
		break;
		default : printf("OP��O INV�LIDA");
		getche();
		confirmar_pedido(0);
	}

}
/**Sobre essa parte relaxa que eu ia montar mesmo. Precisava dela para fazer a parte da 'cobranca'**/

void mostrar_pedido(void)
{
    printf("Mostrar pedido na tela\n");
    /*
    Aqui devemos fazer com que na tela apare�a as informa��es dos lan�amentos feitos e o n� do pedido.
    */
}

void limpar_pedido(void)
{
    printf("\nVoc� escolheu CANCELAR pedido.\n");
    /*
    Aqui devemos fazer com que todos os lan�amentos sejam apagados.
    */
}


void rm_lancamento(void)
{
    printf("\nVoc� escolheu REMOVER lan�amento.\n");
    /*
    Este seria o sub-m�dulo 'rm_lancamento' que tem no mapa l�gico.
    Creio que deva ser montado com base nele.
    */
}



