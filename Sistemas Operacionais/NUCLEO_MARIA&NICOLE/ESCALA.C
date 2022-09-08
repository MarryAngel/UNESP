#include <stdio.h>
#include <system.h>

PTR_DESC dA, dB, dmain, d_esc; /*declaração dos descritores*/

void far corotina_A(){ /*co-rotina A*/
	while(1)
		printf("A");
}

void far corotina_B(){ /*co-rotina B*/
	while(1)
		printf("B");
}

void far escalador(){ /* co-rotina escalador */
	/*estabelecer parâmetros da estrtura p_est (iotransfer)*/
	p_est->p_origem = d_esc;
	p_est->p_destino = dA;
	p_est->num_vetor = 8;
	
	while(1){
		iotransfer();
		disable(); /*desabilita interrupções*/
		
		/*mudança no destino, para que as co-rotinas sejam intercaladas*/
		if(p_est->p_destino == dA) 
			p_est->p_destino = dB;
			
		else
			p_est->p_destino = dA;
		
		enable(); /*habilita interrupções*/	
	}
}

main(){
	/* criação dos descritores da co-rotina */
	dA = cria_desc();
	dB = cria_desc();
	d_esc = cria_desc();
	dmain = cria_desc();
	
	/* associação das co-rotinas com os descritores */
	newprocess(corotina_A, dA);
	newprocess(corotina_B, dB);
	newprocess(escalador, d_esc);
	
	/* transferir controle da main para o escalador */
	transfer(dmain,d_esc);
}
