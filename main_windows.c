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
#include <conio.h> // para utilizar 'getch'
#include <unistd.h> // para utilizar 'getch'
#include <locale.h>
#define N 5
float valor_total; // (GLOBAL) TOTAL DOS PRODUTOS SELECIONADOS
/*
Eu identifiquei 3 estruturas diferentes que precisamos trabalhar:
- Produto [CodProd / NomeProd / CustoProd] >> Eu vi que você já montou ela e daí teríamos nossa base de produtos a partir dela.
- Pedido_lancamentos [Npedido / CodProd / NomeProd / CustoProd / Qtd / Valor] >> Será muito usada no módulo 'Vendas'.
- Pedido_pagamentos [Npedido / ValorTotal / FormaPgto] >> Será muito usada no módulo 'Cobrança'.
*/

//DEFINIÇÃO DE UMA VARIÁVEL COMPOSTA HETEROGÊNEA 
typedef struct{
	int cod_produto;
	char nome_produto[40];
	float custo_produto;
	int indice_exibicao;//
	int indice_unidade;
}cadastro;


//ELABORADO UM VETOR DO TIPO CADASTRO, 5PO
cadastro lista[N] = {
	{0,"HAMBÚRGUER",40.20},
	{1,"COCA COLA",25.30},
	{2,"BATATA FRITA",4.34},
	{3,"CERVEJA",3.20},
	{4,"CAFÉ",14.20}
};


//TOPO DA PÁGINA 
void topo(){
	printf("----------------------------------------------------------");
	printf("\n|CÓDIGO_DO_PRODUTO | NOME_DO_PRODUTO  | CUSTO_DO_PRODUTO |\n");
	printf("----------------------------------------------------------");
	printf("\n");
}


//EXIBICAÇÃO DOS PRODUTOS / VALORES / CÓDIGO CADASTRADOS NO VETOR LISTA.
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


//FUNÇÃO LANÇAMENTOS
void reg_lancamento(void)
{
    topo();
    exibicao();
   	calculo_produto();
}


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
	
	
	
    system("clear || cls");
	
	if(pedidos ==0){//IRÁ CHAMAR A PRÓPRIA FUNÇÃO SE FOR DIGITADO O VALOR 0
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
		printf("SELECIONE O CÓDIGO DO PRODUTO : ");
		scanf("%i",&indice);
	
		if((indice >= 0) && (indice<=4)){
			
	
			printf("DIGITE O NÚMERO DE UNIDADES : ");
			scanf("%i",&unit);
			
			lista[i].indice_exibicao = indice;
			lista[i].indice_unidade = unit;

			
			if(unit == 0){
				unit = 1;
			}
				
			total = lista[indice].custo_produto * unit;
			
				
			printf("\nNOME DO PRODUTO : %s\n",lista[indice].nome_produto);
		    printf("PREÇO UNITÁRIO : R$ %.2f\n",lista[indice].custo_produto);
		    printf("UNIDADES ADQUIRIDA : %i\n",unit);
			printf("SUBTOTAL TOTAL : R$ %.2f\n",total);
			
			
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
				/*
					VALORES ZERADOS PARA NÃO INTERFERIREM NO LAÇO 
				*/
				topo();
				exibicao();
				calculo_produto();//CHAMA A PRÓPRIA FUNÇÃO SE SELECIONAR O CÓDIGO ERRADO
			}
			soma_total += total;// calculando o valor total
		
		
			valor_total = soma_total; // GLOBAL QUE ARMAZENA O VALOR COMPLETO.
	}
	
	//SÓ IRÁ APRESENTAR O RESULTADO SE A VARIÁVEL CONTER ALGUM VALOR.
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



