/* 
	ALGORITMO B�SICO : PROJETO MICKEY E DONALD (VERS�O TESTE)
	AUTOR : FELIPPE M
	DESCRI��O : M.VENDA
*/



#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <conio.h>

#define N 5
typedef struct{
	int cod_produto;
	char nome_produto[40];
	float custo_produto;
	float custo_total;
}cadastro;

cadastro lista[N] = {
	{0,"HAMB�RGUER",40.20},
	{1,"COCA COLA",25.30},
	{2,"BATATA FRITA",4.34},
	{3,"CERVEJA",3.20},
	{4,"CAF�",14.20}
};

void topo(){
	printf("----------------------------------------------------------");
	printf("\n|C�DIGO_DO_PRODUTO | NOME_DO_PRODUTO  | CUSTO_DO_PRODUTO |\n");
	printf("----------------------------------------------------------");
	printf("\n");
}
void exibicao(){
	int i;	
	printf("\n");
	for(i=0;i<N;i++){
		printf("%i%30s  %17.2f \n\n",lista[i].cod_produto,lista[i].nome_produto,lista[i].custo_produto);
	}
	printf("----------------------------------------------------------\n");
}

int calculo_produto(){
	FILE *ponteiro_arquivo; // vari�vel tipo arquivo.	

	int i=0,indice=0,unit=0;
	char opcao,letra;	
	float total=0,soma_total=0;
	
	
	printf("\n");
	do{
		ponteiro_arquivo = fopen("registro/registro_pagamento.txt","a"); // IR� CRIAR O ARQUIVO DENTRO DA PASTA REGISTRO (W) = escrita
		indice = 0;
		printf("SELECIONE O C�DIGO DO PRODUTO : ");
		scanf("%i",&indice);	
		
		if((indice >= 0) && (indice<=4)){
			printf("DIGITE O N�MERO DE UNIDADES : ");
			scanf("%i",&unit);
				
			if(unit == 0){
				unit = 1;
			}		
		
			total = lista[indice].custo_produto * unit;
			system("cls");
			printf("**********NOTA FISCAL**********\n");
			printf("\nRAZ�O SOCIAL : MICKEY E DONALD");
			printf("\nNOME DO PRODUTO : %s\nPRE�O UNIT�RIO : R$ %.2f\nQUANTIDADE ADQUIRIDA : %i UNIDADES \n\nSUBTOTAL TOTAL : R$ %.2f\n\n",
			lista[indice].nome_produto,lista[indice].custo_produto,unit,total);
			
			//ESCRITA NO ARQUIVO CRIADO A PARTIR DA VARI�VEL PONTEIRO_ARQUIVO
			
			
			fprintf(ponteiro_arquivo,"RAZ�O SOCIAL : MICKEY E DONALD\nC�DIGO DO PRODUTO : %i\n",lista[indice].cod_produto);
			fprintf(ponteiro_arquivo,"NOME DO PRODUTO : %s\nPRE�O UNIT�RIO : R$ %.2f\n",lista[indice].nome_produto,lista[indice].custo_produto);
			fprintf(ponteiro_arquivo,"QUANTIDADE ADQUIRIDA : %i UNIDADES\nVALOR TOTAL : R$ %.2f\n\n",unit,total);
			fprintf(ponteiro_arquivo,"\n");

			fclose(ponteiro_arquivo);
			
			////////////////////////////////////////////////////////////////////
			
			system("pause");
			system("cls");
			topo();	
			exibicao();

		}else{
			printf("\n***OP��O INV�LIDA***\n");
			system("pause");
			system("cls");
			topo();	
			exibicao();
		}
		soma_total += total;
		
		if(soma_total!=0){
			printf("TOTAL : %.2f\n",soma_total);
			printf("----------------------------------------------------------\n");
		}
		printf("\nDESEJA CONTINUAR [S/N]?");
		fflush(stdin);
		scanf("%s",&opcao);	
		//printf("\n");
		letra = toupper(opcao);
		
	}while(letra !='N');
}

int pagamento_produto(){
	int pagamento=0,parcela=0;
	do{
		system("cls");
		printf("\n>>>MODO DE PAGAMENTO<<<\n\n");
		printf("1 ) DINHEIRO\n2 ) CART�O DE CR�DITO\n3 ) CHEQUE\n\n");	
		printf("SELECIONA UMA OP��O V�LIDA : ");
		scanf("%i",&pagamento);

		switch(pagamento){
			case 1 : 
				printf("\nFORMA DE PAGAMENTO : DINHEIRO\n");
				 printf("QUANTAS PARCELAS : ");		 
				 scanf("%i",&parcela);
			break;
			case 2 : printf("\nFORMA DE PAGAMENTO : CART�O DE CR�DITO");
			
			break;
			case 3 : printf("\nFORMA DE PAGAMENTO : CHEQUE");
	
			break;
		}
	}while(pagamento>3 || pagamento==0);
}


int main(){
	setlocale(LC_ALL,"PORTUGUESE");		
	topo();
	exibicao();
	calculo_produto();
	pagamento_produto();
	
}

