/* BIBLIOTECAS */
#include<system.h>
#include<stdio.h>

/* CRIACAO DAS ESTRUTURAS DE DADOS USADAS */
typedef struct desc_p{
    char nome[35];                                  /*nome do processo*/
    enum{ativo, bloq_P, terminado} estado;       /*estado em que o processo se encontra*/
    PTR_DESC contexto;                              /*contexto em que o processo se encontra*/
    struct desc_p *prox_desc;                       /*ponteiro que indica qual é o próximo BCP -> ajuda na criação da fila de BCP's*/
    struct desc_p *fila_sem;                        /*Fila de processos bloqueados por semaforo*/
}DESCRITOR_PROC;

typedef DESCRITOR_PROC *PTR_DESC_PROC;

/*criação do tipo semáforo*/
typedef struct{
	int s; /*parte inteira do semáforo*/
	PTR_DESC_PROC Q; /*criação da fila Q do semáforo como um ponteiro descritor de processo*/
} semaforo;

/*Estrutura de dados para a regiao critica*/
typedef struct registros{
	unsigned bx1, es1;
} regis;

typedef union k{
	regis x;
	char far *y;
} APONTA_REG_CRIT;

/* FUNCOES QUE SAO USADAS E ESTAO PRESENTES EM NUCLEO.C */

extern void far volta_dos();

extern void far inicia_fila_prontos();

extern void far cria_processo(char nome[35], void far (*end_proc)());

extern PTR_DESC_PROC procura_proximo_ativo;

extern void far escalador();

extern void Dispara_Sistema();

extern void far Finaliza_Processo();

/*FUNCOES USADAS NO SEMAFOROS*/
extern void far inicia_semaforo(semaforo *sem,int n);

extern void far P(semaforo *sem);

extern void far V(semaforo *sem);
