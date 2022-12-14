#include<stdio.h>
#include<stdlib.h>
#include<nucleo.h>

/*definir tamanho m?ximo do buffer*/
#define TAM_MAX 100

/*declara??o do arquivo*/
FILE *arquivo;

/*valor m?ximo de loops de produ??o e consumo*/
#define LIMITE 1000

/*declara??o dos sem?foros necess?rio*/
semaforo mutex;
semaforo vazio;
semaforo cheio;

/*vari?veis que indicam as posi??es e valores do buffer*/
int posProdutor = 0, posConsumidor = 0, buffer[TAM_MAX];
int valorColocado, valorRetirado;

/*vari?veis para a produ??o de valores*/
int i = 1, res = 1, aux;
int xx;
int y;

void far produzirValor(){
	/*produ??o de sequ?ncia de fatoriais, com in?cio em i = 1*/

    res +=3;
	valorColocado = res;
}

 void far inserirBuffer(){
 	while(buffer[posProdutor] != 0){ /*caso a posi??o do buffer analisada n?o esteja vazia*/
 		posProdutor++;	/*passa para a pr?xima posi??o do buffer*/

 		if (posProdutor == TAM_MAX) /*se a pr?xima posi??o ultrapassar o m?ximo*/
			posProdutor = 0; /*posi??o volta a zero*/
	}

	/*encontrou posi??o vazia*/
	buffer[posProdutor] = valorColocado;
	posProdutor++; /*passar para a pr?xima posi??o do buffer*/

	if (posProdutor == TAM_MAX)
		posProdutor = 0;
 }

void far retiraBuffer(){
	while(buffer[posConsumidor] == 0){ /*enquanto a posi??o do buffer analisada for vazia*/
		posProdutor++; /*passar para a pr?xima posi??o*/

		if(posConsumidor == TAM_MAX) /*caso atinja a posi??o m?xima*/
			posConsumidor = 0; /*voltar para o in?cio do buffer*/
	}

	valorRetirado = buffer[posConsumidor];
	buffer[posConsumidor] = 0; /*como o valor foi retirado dessa posi??o do buffer, esta passa a ter o valor 0*/
	posConsumidor++; /*passa para a pr?xima posi??o*/

	if (posConsumidor == TAM_MAX)
		posConsumidor = 0;
}

void far produtor(){
	posProdutor = 0;
    xx=0;
	while(xx < LIMITE){
		produzirValor(); /*produ??o do resultado a ser inserido no buffer*/
		P(&vazio); /*chamada da primitiva down para o sem?foro vazio*/
		P(&mutex); /*chamada da primitiva down para o sem?foro mutex*/
		inserirBuffer(); /*inserir elemento no buffer*/
		fprintf(arquivo,"Produtor colocou no buffer[%d] = %d\n", posProdutor, valorColocado);
		V(&mutex); /*chamada da primitiva up para o sem?foro mutex*/
		V(&cheio); /*chamada da primitiva up para o sem?foro cheio*/

		xx++;
	}

	Finaliza_Processo();
}

void far consumidor(){
	posConsumidor = 0;
	y = 0;

	while(y < LIMITE){
		P(&cheio); /*chamada da primitiva down para o sem?foro cheio*/
		P(&mutex); /*chamada da primitiva down para o sem?foro mutex*/
		retiraBuffer(); /*retira elemento do buffer*/
		fprintf(arquivo,"Consumidor retirou do buffer[%d] o valor: %d\n", posConsumidor, valorRetirado);
		V(&mutex); /*chamada da primitiva up para o sem?foro mutex*/
		V(&vazio); /*chamada da primitiva V para o sem?foro vazio*/

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

	/*iniciar sem?foros*/
	inicia_semaforo(&vazio,TAM_MAX);
	inicia_semaforo(&cheio,0);
	inicia_semaforo(&mutex,1);

	/*colocar buffer com 0 em todas as posi??es*/
	for (aux = 0; aux <= TAM_MAX; aux++)
		buffer[aux] = 0;

	Dispara_Sistema();

}

