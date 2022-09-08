/* BIBLIOTECAS */
#include<stdio.h>
#include<system.h>

/* Cada processo deverá ter um descritor de processo associado  */
/* Esta estrutura de dados comportará como BCP dos processos */
typedef struct desc_p{
    char nome[35];                              /*nome do processo - id*/
    enum{ativo, bloq_P, terminado} estado;       /*estado em que o processo se encontra*/
    PTR_DESC contexto;                          /*contexto em que o processo se encontra*/
    struct desc_p *prox_desc;                   /*ponteiro que indica qual é o próximo BCP -> ajuda na criação da fila de BCP's*/
    struct desc_p *fila_sem;                    /*Fila de processos bloqueados por semaforo*/
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

/*VARIÁVEIS GLOBAIS*/
PTR_DESC_PROC prim;
PTR_DESC d_esc;
APONTA_REG_CRIT a;

/*----------------------------------------------------------------------------------------*/
/*                          Rotinas básicas do núcleo                                    */
/*--------------------------------------------------------------------------------------*/

/*O controle do sistema retorma para o DOS */
void far volta_dos(){
    disable();                          /*Desativar interrupções*/
    setvect(8, p_est->int_anterior);    /*Retormar o estados das interrupções para o padrão*/
    enable();                           /*Ativar as interrupções*/
    exit(0);                            /*Voltar para o DOS*/
}

/*Inicializa a fila dos processos prontos como vazia*/
void far inicia_fila_prontos(){
    prim = NULL;
}

void far cria_processo(char nome[35], void far (*end_proc)()){
    /*variável local para receber o endereço da estrutura do BCP + alocação dinâmica desta variável */
    PTR_DESC_PROC p_aux = (PTR_DESC_PROC)malloc(sizeof(struct desc_p));

    /*Insercao de informacoes*/
    strcpy(p_aux->nome, nome);
    p_aux->estado=ativo;
    p_aux->contexto=cria_desc();

    newprocess(end_proc, p_aux->contexto);

    p_aux->prox_desc=NULL;

    /*Inserção dos processos ativos no final da fila*/
    if(prim == NULL){
        p_aux->prox_desc=p_aux;
        prim=p_aux;
    }else{
        PTR_DESC_PROC aux = prim;
        while(aux->prox_desc != prim)
            aux = aux->prox_desc;
        aux->prox_desc = p_aux;
        p_aux->prox_desc = prim;
    }
}

PTR_DESC_PROC procura_proximo_ativo(){
    /*Auxiliar para percorrer a fila, deve ser iniciado com o proximo elemento*/
    PTR_DESC_PROC proximo_ativo = prim->prox_desc;

    while(proximo_ativo != prim){
        if(proximo_ativo->estado == ativo)
            return proximo_ativo;
        proximo_ativo = proximo_ativo->prox_desc;
    }

    return NULL;
}

void far escalador(){
    p_est->p_origem = d_esc;
    p_est->p_destino=prim->contexto;
    p_est->num_vetor=8;

    /*Iniciar ponteiro para região crítica do DOS*/
    _AH=0x34;
    _AL=0x00;
    geninterrupt(0x21);
    a.x.bx1 = _BX;
	a.x.es1 = _ES;

    while(1){
        iotransfer();
        disable();
		if(!*a.y){ /*se não está na RC, troca o processo*/
			if((prim = procura_proximo_ativo()) == NULL) /*se não houver mais processos ativos*/
				volta_dos();
			p_est->p_destino = prim->contexto;
		}
        enable();
    }
}

void Dispara_Sistema(){
    /*Criacao dos descritores*/
    PTR_DESC desc_dispara;
    desc_dispara=cria_desc();
    d_esc=cria_desc();

    /*Inicia o processo do escalador*/
    newprocess(escalador, d_esc);

    /*Transferir para o escalador o controle atual*/
    transfer(desc_dispara, d_esc);
}

void far Finaliza_Processo(){
    /*Desabilitar as interrupções*/
    disable();
    /*Como o processo terminou, deve marcar no seu BCP 'terminado' o seu estado*/
    prim->estado = terminado;
    /*Habilitar as interrupções*/
    enable();
    while(1);
}

/*----------------------------------------------------------------------------------------*/
/*                 Funcoes para Semaforos                                                 */
/*----------------------------------------------------------------------------------------*/

void far inicia_semaforo(semaforo *sem,int n){
	sem->s = n; /*iniciação da variável inteira com um valor n*/
	sem->Q = NULL; /*fila começa vazia*/
}

void far insere_fila_semaforo(semaforo *sem){
    PTR_DESC_PROC aux;
    if(sem->Q == NULL)
        sem->Q = prim;
    else{
        aux = sem->Q;
        while(aux->fila_sem!=NULL)
            aux=aux->fila_sem;
        aux->fila_sem=prim;
    }
    prim->fila_sem=NULL;
}

/*primitiva da operação P (ou down)*/
void far P(semaforo *sem){
	PTR_DESC_PROC aux; /*BCP auxiliar*/

	disable(); /*desabilitar interrupções*/
	if (sem->s > 0){
		sem->s--; /*decrementa a variável inteira do semáforo*/
		enable(); /*habilita interrupções*/
	}

		else{ /*bloqueia processo na fila sem->Q*/
			insere_fila_semaforo(sem); /*insere descritor do processo na fila sem->Q*/
			(sem->Q)->estado = bloq_P;

			/*achar próximo processo pronto*/
			aux = prim;

			if((prim = procura_proximo_ativo()) == NULL)
				volta_dos();

			transfer(aux->contexto,prim->contexto);
		}
}

void far V(semaforo *sem){
	PTR_DESC_PROC aux; /*declarar descritor de processo auxiliar*/

	disable(); /*desabilitar interrupções*/
	if (sem->Q == NULL) /*se a fila do semáforo estiver vazia*/
		sem->s++; /*acrescenta 1 na parte inteira do semáforo*/

		else{
			aux = sem->Q; /*auxiliar recebe o topo da fila do semáforo*/
			sem->Q = aux->fila_sem; /*fila do semáforo recebe o próximo descritor*/
			aux->fila_sem = NULL; /*aux não aponta para mais ninguém na fila de bloqueados*/
			aux->estado = ativo; /*processo do BCP aux passa a estar ativo*/
		}

	enable();
}
