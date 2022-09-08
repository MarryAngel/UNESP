#include<stdio.h>
#include<stdlib.h>
#include<nucleo.h>

/*definir tamanho máximo do buffer*/
#define TAM_MAX 100

/*declaração do arquivo*/
FILE *arquivo;

/*valor máximo de loops de produção e consumo*/
#define LIMITE 1000

/*declaração dos semáforos necessário*/
semaforo mutex;
semaforo vazio;
semaforo cheio;

/*variáveis que indicam as posições e valores do buffer*/
int posProdutor = 0, posConsumidor = 0, buffer[TAM_MAX];
int valorColocado, valorRetirado;

/*variáveis para a produção de valores*/
int i = 1, res = 1, aux;
int xx;
int y;

void far produzirValor(){
	/*produção de sequência de fatoriais, com início em i = 1*/

    res +=3;
	valorColocado = res;
}

 void far inserirBuffer(){
 	while(buffer[posProdutor] != 0){ /*caso a posição do buffer analisada não esteja vazia*/
 		posProdutor++;	/*passa para a próxima posição do buffer*/

 		if (posProdutor == TAM_MAX) /*se a próxima posição ultrapassar o máximo*/
			posProdutor = 0; /*posição volta a zero*/
	}

	/*encontrou posição vazia*/
	buffer[posProdutor] = valorColocado;
	posProdutor++; /*passar para a próxima posição do buffer*/

	if (posProdutor == TAM_MAX)
		posProdutor = 0;
 }

void far retiraBuffer(){
	while(buffer[posConsumidor] == 0){ /*enquanto a posição do buffer analisada for vazia*/
		posProdutor++; /*passar para a próxima posição*/

		if(posConsumidor == TAM_MAX) /*caso atinja a posição máxima*/
			posConsumidor = 0; /*voltar para o início do buffer*/
	}

	valorRetirado = buffer[posConsumidor];
	buffer[posConsumidor] = 0; /*como o valor foi retirado dessa posição do buffer, esta passa a ter o valor 0*/
	posConsumidor++; /*passa para a próxima posição*/

	if (posConsumidor == TAM_MAX)
		posConsumidor = 0;
}

void far produtor(){
	posProdutor = 0;
    xx=0;
	while(xx < LIMITE){
		produzirValor(); /*produção do resultado a ser inserido no buffer*/
		P(&vazio); /*chamada da primitiva down para o semáforo vazio*/
		P(&mutex); /*chamada da primitiva down para o semáforo mutex*/
		inserirBuffer(); /*inserir elemento no buffer*/
		fprintf(arquivo,"Produtor colocou no buffer[%d] = %d\n", posProdutor, valorColocado);
		V(&mutex); /*chamada da primitiva up para o semáforo mutex*/
		V(&cheio); /*chamada da primitiva up para o semáforo cheio*/

		xx++;
	}

	Finaliza_Processo();
}

void far consumidor(){
	posConsumidor = 0;
	y = 0;

	while(y < LIMITE){
		P(&cheio); /*chamada da primitiva down para o semáforo cheio*/
		P(&mutex); /*chamada da primitiva down para o semáforo mutex*/
		retiraBuffer(); /*retira elemento do buffer*/
		fprintf(arquivo,"Consumidor retirou do buffer[%d] o valor: %d\n", posConsumidor, valorRetirado);
		V(&mutex); /*chamada da primitiva up para o semáforo mutex*/
		V(&vazio); /*chamada da primitiva V para o semáforo vazio*/

		y++;
	}

	Finaliza_Processo();
}

main(){
	/*abrir arquivo texto*/
	arquivo = fopen ("PC.txt","w");
	if (!arquivo) {
  		printf ("Erro na abertura do arquivo.");
  		exit (1);
 	}

	/*criar processos*/
	cria_processo("produtor",produtor);
	cria_processo("consumidor", consumidor);

	/*iniciar fila prontos*/
	inicia_fila_prontos();

	/*iniciar semáforos*/
	inicia_semaforo(&vazio,TAM_MAX);
	inicia_semaforo(&cheio,0);
	inicia_semaforo(&mutex,1);

	/*colocar buffer com 0 em todas as posições*/
	for (aux = 0; aux <= TAM_MAX; aux++)
		buffer[aux] = 0;

	Dispara_Sistema();

}

