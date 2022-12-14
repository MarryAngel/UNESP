/* BIBLIOTECAS */
#include<stdio.h>
#include<system.h>

/* Cada processo dever? ter um descritor de processo associado  */
/* Esta estrutura de dados comportar? como BCP dos processos */
typedef struct desc_p{
    char nome[35];                              /*nome do processo - id*/
    enum{ativo, bloq_P, terminado} estado;       /*estado em que o processo se encontra*/
    PTR_DESC contexto;                          /*contexto em que o processo se encontra*/
    struct desc_p *prox_desc;                   /*ponteiro que indica qual ? o pr?ximo BCP -> ajuda na cria??o da fila de BCP's*/
    struct desc_p *fila_sem;                    /*Fila de processos bloqueados por semaforo*/
}DESCRITOR_PROC;

typedef DESCRITOR_PROC *PTR_DESC_PROC;

/*cria??o do tipo sem?foro*/
typedef struct{
	int s; /*parte inteira do sem?foro*/
	PTR_DESC_PROC Q; /*cria??o da fila Q do sem?foro como um ponteiro descritor de processo*/
} semaforo;

/*Estrutura de dados para a regiao critica*/
typedef struct registros{
	unsigned bx1, es1;
} regis;

typedef union k{
	regis x;
	char far *y;
} APONTA_REG_CRIT;

/*VARI?VEIS GLOBAIS*/
PTR_DESC_PROC prim;
PTR_DESC d_esc;
APONTA_REG_CRIT a;

/*----------------------------------------------------------------------------------------*/
/*                          Rotinas b?sicas do n?cleo                                    */
/*--------------------------------------------------------------------------------------*/

/*O controle do sistema retorma para o DOS */
void far volta_dos(){
    disable();                          /*Desativar interrup??es*/
    setvect(8, p_est->int_anterior);    /*Retormar o estados das interrup??es para o padr?o*/
    enable();                           /*Ativar as interrup??es*/
    exit(0);                            /*Voltar para o DOS*/
}

/*Inicializa a fila dos processos prontos como vazia*/
void far inicia_fila_prontos(){
    prim = NULL;
}

void far cria_processo(char nome[35], void far (*end_proc)()){
    /*vari?vel local para receber o endere?o da estrutura do BCP + aloca??o din?mica desta vari?vel */
    PTR_DESC_PROC p_aux = (PTR_DESC_PROC)malloc(sizeof(struct desc_p));

    /*Insercao de informacoes*/
    strcpy(p_aux->nome, nome);
    p_aux->estado=ativo;
    p_aux->contexto=cria_desc();

    newprocess(end_proc, p_aux->contexto);

    p_aux->prox_desc=NULL;

    /*Inser??o dos processos ativos no final da fila*/
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

    /*Iniciar ponteiro para regi?o cr?tica do DOS*/
    _AH=0x34;
    _AL=0x00;
    geninterrupt(0x21);
    a.x.bx1 = _BX;
	a.x.es1 = _ES;

    while(1){
        iotransfer();
        disable();
		if(!*a.y){ /*se n?o est? na RC, troca o processo*/
			if((prim = procura_proximo_ativo()) == NULL) /*se n?o houver mais processos ativos*/
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
    /*Desabilitar as interrup??es*/
    disable();
    /*Como o processo terminou, deve marcar no seu BCP 'terminado' o seu estado*/
    prim->estado = terminado;
    /*Habilitar as interrup??es*/
    enable();
    while(1);
}

/*----------------------------------------------------------------------------------------*/
/*                 Funcoes para Semaforos                                                 */
/*----------------------------------------------------------------------------------------*/

void far inicia_semaforo(semaforo *sem,int n){
	sem->s = n; /*inicia??o da vari?vel inteira com um valor n*/
	sem->Q = NULL; /*fila come?a vazia*/
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

/*primitiva da opera??o P (ou down)*/
void far P(semaforo *sem){
	PTR_DESC_PROC aux; /*BCP auxiliar*/

	disable(); /*desabilitar interrup??es*/
	if (sem->s > 0){
		sem->s--; /*decrementa a vari?vel inteira do sem?foro*/
		enable(); /*habilita interrup??es*/
	}

		else{ /*bloqueia processo na fila sem->Q*/
			insere_fila_semaforo(sem); /*insere descritor do processo na fila sem->Q*/
			(sem->Q)->estado = bloq_P;

			/*achar pr?ximo processo pronto*/
			aux = prim;

			if((prim = procura_proximo_ativo()) == NULL)
				volta_dos();

			transfer(aux->contexto,prim->contexto);
		}
}

void far V(semaforo *sem){
	PTR_DESC_PROC aux; /*declarar descritor de processo auxiliar*/

	disable(); /*desabilitar interrup??es*/
	if (sem->Q == NULL) /*se a fila do sem?foro estiver vazia*/
		sem->s++; /*acrescenta 1 na parte inteira do sem?foro*/

		else{
			aux = sem->Q; /*auxiliar recebe o topo da fila do sem?foro*/
			sem->Q = aux->fila_sem; /*fila do sem?foro recebe o pr?ximo descritor*/
			aux->fila_sem = NULL; /*aux n?o aponta para mais ningu?m na fila de bloqueados*/
			aux->estado = ativo; /*processo do BCP aux passa a estar ativo*/
		}

	enable();
}
