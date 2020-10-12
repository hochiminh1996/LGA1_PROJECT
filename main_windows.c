/*
Leandro Higa:
>> Criei essa interface baseado no mapa l�gico que tinha montado j�.
>> Perceba que todas as fun��es que foram criadas est�o abaixo de 'main' e se eu preciso de uma fun��o qualquer
   dentro de outra fun��o ou na pr�pria 'main', eu declaro ela antes de tudo. Vi pela internet que esse � um
   padr�o recomendado.
>> Criei a fun��o 'getch' pois n�o h� no LINUX, creio que deva funcionar normal no WINDOWS.
>> Perceba que coloquei no que usei 'clear || cls'. Isso porque no LINUX o comando de limpar a tela � 'clear'.
   No formato escrito ela funcionar� em ambos os sistemas.
>> Tamb�m fiz com que a fun��o 'main' chame o m�dulo 'vendas'. Acho que os m�dulos n�o funcionam de maneira
   linear. Logo, na main dever� ser poss�vel escolher o m�dulo que voc� quer entrar.
>> Pode dar play pois, em teoria, deve funcionar.
*/

#include <stdio.h>
#include <stdlib.h> // para utilizar 'system'
#include <conio.h> // para utilizar 'getch'
#include <unistd.h> // para utilizar 'getch'
#include <locale.h>
#define N 5
float valor_total; // (GLOBAL) TOTAL DOS PRODUTOS SELECIONADOS
/*
Eu identifiquei 3 estruturas diferentes que precisamos trabalhar:
- Produto [CodProd / NomeProd / CustoProd] >> Eu vi que voc� j� montou ela e da� ter�amos nossa base de produtos a partir dela.
- Pedido_lancamentos [Npedido / CodProd / NomeProd / CustoProd / Qtd / Valor] >> Ser� muito usada no m�dulo 'Vendas'.
- Pedido_pagamentos [Npedido / ValorTotal / FormaPgto] >> Ser� muito usada no m�dulo 'Cobran�a'.
*/

//DEFINI��O DE UMA VARI�VEL COMPOSTA HETEROG�NEA 
typedef struct{
	int cod_produto;
	char nome_produto[40];
	float custo_produto;
	int indice_exibicao;//
	int indice_unidade;
}cadastro;


//ELABORADO UM VETOR DO TIPO CADASTRO, 5PO
cadastro lista[N] = {
	{0,"HAMB�RGUER",40.20},
	{1,"COCA COLA",25.30},
	{2,"BATATA FRITA",4.34},
	{3,"CERVEJA",3.20},
	{4,"CAF�",14.20}
};


//TOPO DA P�GINA 
void topo(){
	printf("----------------------------------------------------------");
	printf("\n|C�DIGO_DO_PRODUTO | NOME_DO_PRODUTO  | CUSTO_DO_PRODUTO |\n");
	printf("----------------------------------------------------------");
	printf("\n");
}


//EXIBICA��O DOS PRODUTOS / VALORES / C�DIGO CADASTRADOS NO VETOR LISTA.
void exibicao(){
	int i;	
	printf("\n");
	for(i=0;i<N;i++){
		printf("%i%30s  %17.2f \n\n",lista[i].cod_produto,lista[i].nome_produto,lista[i].custo_produto);
	}
	printf("----------------------------------------------------------\n");
}

//TELA INICIAL.
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
	
	
	
    system("clear || cls");
	
	if(pedidos ==0){//IR� CHAMAR A PR�PRIA FUN��O SE FOR DIGITADO O VALOR 0
		topo();
		exibicao();
		calculo_produto();
	}

	for(i=0;i<pedidos;i++){
		total = 0;
		indice = 0;
		unit = 0;
		contador++;// APENAS PARA ENUMERAR OS PEDIDOS.
		
		topo();
		exibicao();		

		
		if(soma_total!=0){
			printf("PEDIDOS : %i\nSUBTOTAL : %.2f\n",pedidos,soma_total);
			printf("----------------------------------------------------------\n");
		}
		
		printf("\nPEDIDO %i\n",contador);
		printf("SELECIONE O C�DIGO DO PRODUTO : ");
		scanf("%i",&indice);
	
		if((indice >= 0) && (indice<=4)){
			
	
			printf("DIGITE O N�MERO DE UNIDADES : ");
			scanf("%i",&unit);
			
			lista[i].indice_exibicao = indice;
			lista[i].indice_unidade = unit;

			
			if(unit == 0){
				unit = 1;
			}
				
			total = lista[indice].custo_produto * unit;
			
				
			printf("\nNOME DO PRODUTO : %s\n",lista[indice].nome_produto);
		    printf("PRE�O UNIT�RIO : R$ %.2f\n",lista[indice].custo_produto);
		    printf("UNIDADES ADQUIRIDA : %i\n",unit);
			printf("SUBTOTAL TOTAL : R$ %.2f\n",total);
			
			
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
				/*
					VALORES ZERADOS PARA N�O INTERFERIREM NO LA�O 
				*/
				topo();
				exibicao();
				calculo_produto();//CHAMA A PR�PRIA FUN��O SE SELECIONAR O C�DIGO ERRADO
			}
			soma_total += total;// calculando o valor total
		
		
			valor_total = soma_total; // GLOBAL QUE ARMAZENA O VALOR COMPLETO.
	}
	
	//S� IR� APRESENTAR O RESULTADO SE A VARI�VEL CONTER ALGUM VALOR.
	if(soma_total!=0){
		topo();
		exibicao();
		printf("TOTAL : %.2f\n",soma_total);
		printf("----------------------------------------------------------\n");
	}
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



