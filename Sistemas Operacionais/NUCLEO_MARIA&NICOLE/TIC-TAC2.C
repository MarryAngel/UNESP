#include<system.h>
#include<stdio.h>

PTR_DESC co1, co2, principal;
int contagem = 0;

/*--------------------------------------*/
/*              COROTINAS               */
/*--------------------------------------*/
void far corotina1(){
    while(contagem < 100){
        contagem=contagem+1;
        printf("tic - ");
        transfer(co1, co2);
    }
    transfer(co1, principal);
}

void far corotina2(){
    while(1){
        printf("tac ");
        transfer(co2, co1);
    }
}

/*-------------------------------------*/

/*--------------------------------------*/
/*         PROGRAMA  PRINCIPAL          */
/*--------------------------------------*/
main (){
    /*Criacao dos descritores de corotinas*/
    co1=cria_desc();
    co2=cria_desc();
    principal=cria_desc();

    /*Criacao dos processos - associar descritores com as corotinas*/
    newprocess(corotina1, co1);
    newprocess(corotina2, co2);

    /*Passar o comando para a corotina1*/
    transfer(principal, co1);
}
