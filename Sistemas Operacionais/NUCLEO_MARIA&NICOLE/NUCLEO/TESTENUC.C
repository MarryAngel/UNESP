#include <nucleo.h>
#include <stdio.h>
#include <stdlib.h>

void far processo1(){ /*co-rotina do processo 1*/
	int i = 0;

	while (i < 10000){
		printf("1");
		i++;
	}

	Finaliza_Processo();
}

void far processo2(){ /*co-rotina do processo 2*/
	int i = 0;

	while (i < 10000){
		printf("2");
		i++;
	}

	Finaliza_Processo();
}

main(){
	/* cria fila dos prontos vazia */
	inicia_fila_prontos();

	/*criação dos processos*/
	cria_processo("proc1",processo1);
	cria_processo("proc2",processo2);

	/*transfere controle para o escalador*/
	Dispara_Sistema();
}



