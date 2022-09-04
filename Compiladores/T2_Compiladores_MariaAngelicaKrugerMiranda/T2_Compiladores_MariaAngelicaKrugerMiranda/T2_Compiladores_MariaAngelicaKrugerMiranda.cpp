/*
    TRABALHO COMPILADORES - ANALISADOR SINTÁTICO
    NOME: MARIA ANGELICA KRÜGER MIRANDA
    RA: 201027691
    SO: WINDOWS 11
    COMPILADOR: g++ (Code Blocks)
*/

#include <bits/stdc++.h>

using namespace std;

//VARIÀVEIS GLOBAIS
char proximo;
string token, msg;
int num_linha, conta_token;
vector<string> tokens_gerados;
map<char,string> simbolos_especiais;
map<string,string> simbolos_compostos;

//FUNÇÔES GERAIS DO COMPILADOR
void ERRO(string mensagem);        //Ao identificar erro para com a compilação e encerra
void COMPILAR();    //Inicia o processo de compilação
void VOCABULARIO(); //Map que contem o vocabulário do Pascal simplificado

//ESCOPO DAS FUNÇÔES USADAS PELO ANALEX
bool ANALISADOR_LEXICO(string &simbolo);    //Realiza a análise léxica do programa recebido
bool LETRA_DIGITO();                        //Verifica se o caractere é dígito ou letra
bool DIGITO();                              //Verifica se o caractere é dígito
bool LETRA();                               //Verifica se o caractere é letra
bool SIMB_ESP();                            //Verifica se o caractere é um símbolo especial
string CODIGO(string simbolo, int option);  //Função que gera tokens
bool PROXIMO();                             //Função que pega o próximo caractere da cadeia de entrada

//ESCOPO DAS FUNÇÔES USADAS PELA ANALISE SINTÁTICO
void ANALISADOR_SINTATICO();        //Inicia a análise sintática
void PEGAR_TOKEN();                 //Pegar o próximo token que o ANALEX produziu
void PROGRAMA();                    //program + identificador + ; + bloco
void BLOCO();                       //Analisa bloco de comandos (entre begin e end)
void PARTE_DECLARA_VARIAVEIS();     //
void DECLARA_VARS();
void LISTA_ID();                    //Sequencia de variáveis separadas por virgula
void PARTE_DECLARA_SUBROTINA();
void DECLARA_FUNCTION();
void DECLARA_PROCEDURE();
void PARAMETROS_FORMAIS();
void SECAO_PARAMETROS_FORMAIS();
void COMANDO_COMPOSTO();
bool COMANDO();
void COMANDO_SEM_ROTULO();
void ATRIBUICAO();
bool CHAMADA_PROCEDIMENTO();
void COMANDO_CONDICIONAL();
void COMANDO_REPETITIVO();
void LISTA_EXPRESSOES();
void EXPRESSAO();
bool RELACAO();
void EXPRESSAO_SIMPLES();
void TERMO();
void FATOR();
void VARIAVEL();
void CHAMADA_FUNCAO();
bool COMANDO_COMPOSTO_SINTATICO();
void TIPO();


void PEGAR_TOKEN(){
    token = tokens_gerados[conta_token];
    conta_token++;
}

void FATOR(){
    string comparar;
    if(token.substr(0,2) == "ID"){
        VARIAVEL();
        PEGAR_TOKEN();
        if(token == "C_ABREPARENTESES"){
            LISTA_ID();
            if(token == "C_FECHAPARENTESES")
                PEGAR_TOKEN();
            else
                ERRO(msg);
        }
    }else{
        if(token.substr(0,5) == "NUMERO")
            PEGAR_TOKEN();
        else{
            if(token == "C_ABREPARENTESES"){
                PEGAR_TOKEN();
                EXPRESSAO();
                if(token != "C_FECHAPARENTESES")
                    ERRO(msg);
                PEGAR_TOKEN();
            }else
                ERRO(msg);
        }
    }
}

void TERMO(){
    FATOR();
    while( (token == "C_ASTERISCO") || (token == "C_AND") ){
        PEGAR_TOKEN();
        FATOR();
    }
}

void EXPRESSAO_SIMPLES(){
    if(token == "C_SOMA" || token == "C_MENOS" || token == "C_DIV")
        PEGAR_TOKEN();
    TERMO();
    while( (token == "C_SOMA") || (token == "C_MENOS") || (token == "C_OR") || (token == "C_DIV")){
        PEGAR_TOKEN();
        TERMO();
    }
}

bool RELACAO(){
    if( token == "C_IGUAL")
        return true;
    if (token == "C_MAIOR")
        return true;
    if (token == "C_MENOR")
        return true;
    return false;
}

void EXPRESSAO(){
    EXPRESSAO_SIMPLES();
    if(RELACAO()){
        PEGAR_TOKEN();
        EXPRESSAO_SIMPLES();
    }
}

void VARIAVEL(){}

void ATRIBUICAO(){
    VARIAVEL();
    PEGAR_TOKEN();
    EXPRESSAO();
}

bool CHAMADA_PROCEDIMENTO(){
    if(token != "ABREPARENTESES")
        return false;
    LISTA_ID();
    if(token != "C_FECHAPARENTESES")
        ERRO(msg);
}

void COMANDO_CONDICIONAL(){
    EXPRESSAO();
    if(token != "C_THEN")
        ERRO(msg);
    PEGAR_TOKEN();
    COMANDO_SEM_ROTULO();
    PEGAR_TOKEN();
    if(token != "C_ELSE"){
        PEGAR_TOKEN();
        COMANDO_SEM_ROTULO();
    }
}

void COMANDO_REPETITIVO(){
    EXPRESSAO();
    if(token != "C_DO")
        ERRO(msg);
    PEGAR_TOKEN();
    COMANDO_SEM_ROTULO();
}

void COMANDO_SEM_ROTULO(){
    if(token.substr(0,2) == "ID"){
        PEGAR_TOKEN();
        if(token == "C_ATRIBUICAO")
            ATRIBUICAO();
        else{
            CHAMADA_PROCEDIMENTO();
            if(token != "C_PONTOVIRGULA")
                PEGAR_TOKEN();
        }
    }else{
        if(token == "C_IF"){ PEGAR_TOKEN(); COMANDO_CONDICIONAL();}
        else{
            if(token == "C_WHILE") { PEGAR_TOKEN(); COMANDO_REPETITIVO();}
            else
                COMANDO_COMPOSTO_SINTATICO();
        }
    }
}

bool COMANDO(){
    COMANDO_SEM_ROTULO();
    if(token != "C_PONTOVIRGULA")
        return false;
    PEGAR_TOKEN();
    return true;
}

bool COMANDO_COMPOSTO_SINTATICO(){
    if(token != "C_BEGIN")
        return false;
    PEGAR_TOKEN();
    while( COMANDO() ){
    }
    if(token != "C_END")
        ERRO(msg);
    return true;
}

void PARAMETROS_FORMAIS(){
    if(token == "C_ABREPARENTESES"){
        PEGAR_TOKEN();
        while (token == "C_VAR" || token.substr(0,2) != "ID"){
            if(token == "C_VAR"){
                PEGAR_TOKEN();
                LISTA_ID();
            }else
                LISTA_ID();
            if(token != "C_PONTOVIRGULA")
                ERRO(msg);
            PEGAR_TOKEN();
            if(token.substr(0,2) != "ID")
                ERRO(msg);
            PEGAR_TOKEN();
            if(token != "C_VIRGULA")
                ERRO(msg);
            PEGAR_TOKEN();
        }
        if (token != "C_FECHAPARENTESES")
            ERRO(msg);
        PEGAR_TOKEN();
    }
}

void DECLARA_FUNCTION(){
    PEGAR_TOKEN();
    if(token.substr(0,2) != "ID")
        ERRO(msg);
    PEGAR_TOKEN();
    PARAMETROS_FORMAIS();
    if(token != "C_DOISPONTOS")
        ERRO(msg);
    PEGAR_TOKEN();
    if(token.substr(0,2) != "ID")
        ERRO(msg);
    if(token != "C_PONTOVIRGULA")
        ERRO(msg);
    BLOCO();
}

void DECLARA_PROCEDURE(){
    PEGAR_TOKEN();
    if(token.substr(0,2) != "ID")
        ERRO(msg);
    PEGAR_TOKEN();
    PARAMETROS_FORMAIS();
    if(token != "C_PONTOVIRGULA")
        ERRO(msg);
    BLOCO();
}

void PARTE_DECLARA_SUBROTINA(){
    while(token == "C_PROCEDURE" || token == "C_FUNCTION"){
        if (token == "C_PROCEDURE")
            DECLARA_PROCEDURE();
        else
            DECLARA_FUNCTION();
        PEGAR_TOKEN();
        if(token != "C_PONTOVIRGULA" )
            ERRO(msg);
        PEGAR_TOKEN();
    }
}

void LISTA_ID(){
    while(token.substr(0,2) == "ID"){
        PEGAR_TOKEN();
        if(token != "C_VIRGULA")
            break;
        PEGAR_TOKEN();
    }
}

void TIPO(){
    //string tipo;
    //tipo = token.substr(3,10);
    //cout << "Tipo: " << tipo << endl;
    //cout << "Token: " << token << endl;
    if(token.substr(3,9) != "INTEGER" )
        ERRO(msg);
    //cout << tipo << endl;
   // if(token.substr(3,9) != "BOOLEAN")
     //  ERRO();
}

void DECLARA_VARS(){
    LISTA_ID();
    if(token != "C_DOISPONTOS") ERRO;
    PEGAR_TOKEN();
    TIPO();
}

void PARTE_DECLARA_VARIAVEIS(){
    if (token == "C_VAR"){
        PEGAR_TOKEN();
        while(token.substr(0,2) == "ID"){
            DECLARA_VARS();
            PEGAR_TOKEN();
            if(token != "C_PONTOVIRGULA" )
                break;
            PEGAR_TOKEN();
        }
    }
}

void BLOCO(){
    PEGAR_TOKEN();
    PARTE_DECLARA_VARIAVEIS();
    PARTE_DECLARA_SUBROTINA();
    if(!COMANDO_COMPOSTO_SINTATICO())
        ERRO(msg);
}

void PROGRAMA(){
    string compara, aux, msg;
    PEGAR_TOKEN();
    if (token == "C_PROGRAM"){
        PEGAR_TOKEN();
        aux = token; compara = aux.substr(0,2);
        if( compara == "ID"){
            PEGAR_TOKEN();
            if(token != "C_PONTOVIRGULA")
                ERRO((msg = "Falta ponto_virgula "));
            BLOCO();
            PEGAR_TOKEN();
            if( token != "C_END" ) ERRO;
            PEGAR_TOKEN;
            if( token != "C_PONTO") ERRO;
        }else
            ERRO( (msg = "Erro no nome do programa ") );
    }else
        ERRO(msg);
    cout << "COMPILACAO EFETUADA COM SUCESSO." << endl;
    exit(1);
}

void ANALISADOR_SINTATICO(){
    conta_token=0;
    cout << "INICIO DO SINTATICO" << endl;
    for(int i=0; i<tokens_gerados.size(); i++)
       cout << tokens_gerados[i] << endl;
    PROGRAMA();
}

void VOCABULARIO(){
    //Símbolos especiais
    simbolos_especiais['.'] = "C_PONTO";
    simbolos_especiais[';'] = "C_PONTOVIRGULA";
    simbolos_especiais[','] = "C_VIRGULA";
    simbolos_especiais['('] = "C_ABREPARENTESES";
    simbolos_especiais[')'] = "C_FECHAPARENTESES";
    simbolos_especiais[':'] = "C_DOISPONTOS";
    simbolos_especiais['='] = "C_IGUAL";
    simbolos_especiais['<'] = "C_MENOR";
    simbolos_especiais['>'] = "C_MAIOR";
    simbolos_especiais['+'] = "C_SOMA";
    simbolos_especiais['-'] = "C_MENOS";
    simbolos_especiais['*'] = "C_ASTERISCO";

    //Simbolos compostos
    simbolos_compostos[":="]        = "C_ATRIBUICAO";
    simbolos_compostos["PROGRAM"]   = "C_PROGRAM";
    simbolos_compostos["LABEL"]     = "C_LABEL";
    simbolos_compostos["VAR"]       = "C_VAR";
    simbolos_compostos["PROCEDURE"] = "C_PROCEDURE";
    simbolos_compostos["FUNCTION"]  = "C_FUNCTION";
    simbolos_compostos["BEGIN"]     = "C_BEGIN";
    simbolos_compostos["END"]       = "C_END";
    simbolos_compostos["IF"]        = "C_IF";
    simbolos_compostos["THEN"]      = "C_THEN";
    simbolos_compostos["ELSE"]      = "C_ELSE";
    simbolos_compostos["WHILE"]     = "C_WHILE";
    simbolos_compostos["DO"]        = "C_D0";
    simbolos_compostos["OR"]        = "C_OR";
    simbolos_compostos["AND"]       = "C_AND";
    simbolos_compostos["DIV"]       = "C_DIV";
    simbolos_compostos["NOT"]       = "C_NOT";
}

bool PROXIMO(){
    char temp;

    //Avançar pelo arquivo pegando caractere por caractere e passando para realizar a análise léxica
    //Ao chegar ao final do arquivo, fechar e começar a análise semântica
    if( (scanf("%c",&temp)) == EOF ){
        ANALISADOR_SINTATICO();
        fclose(stdout);
        return false;
    }

    //Contador para quebras de linha caso gere erro
    if(temp == '\n')
        num_linha++;

    //Transformar temp em mai�sculo
    proximo = toupper(temp);

    return true;
}

string CODIGO(string simbolo, int option) {
    string aux1 = "ID_";
    string aux2 = "NUMERO_";

    //Simbolos que é um simbolo composto
    if(option == 1)
        return simbolos_compostos[simbolo];
    //Simbolos que é uma variavel
    else if (option == 2)
        return simbolo=aux1+simbolo;
    //Simbolo que é um número
    return simbolo=aux2+simbolo;
}

void ERRO(string mensagem){
    cout << "Erro ao compilar." << endl;
    cout << "Erro: " << mensagem << endl;
    exit(1);
}

bool SIMB_ESP(){
    if( simbolos_especiais.count(proximo) )
        return true;
    else
        return false;
}

bool LETRA(){
    if(proximo>= 'A' && proximo <= 'Z')
        return true;
    return false;
}

bool DIGITO(){
    if( proximo >= '0' && proximo <= '9' )
        return true;
    return false;
}

bool LETRA_DIGITO(){
    if( (proximo >= 'A' && proximo <= 'Z') || (proximo >= '0' && proximo <= '9') )
        return true;
    return false;
}

bool ANALISADOR_LEXICO(string &simbolo){
    string atomo="";
    char anterior;

    //Ignorar final de linha, espaços e tabulações
    while( proximo == '\n' || proximo == ' ' || proximo == '\t'){
        if( !PROXIMO() )
            return false;
    }

    //Verificar se proximo é simbolo especial e se for deve ver se é especial composto
    if( SIMB_ESP() ){
        anterior = proximo;
        PROXIMO();
        if( anterior == ':' ){
            if ( proximo == '=' ){
                atomo+=anterior;
                atomo+=proximo;
                simbolo = CODIGO(atomo, 1);
                PROXIMO();
                return true;
            }
        }
        simbolo = simbolos_especiais[anterior];
    }
    //Verificar se forma comando, variável ou número
    else if( LETRA() ){
        do{
            atomo+=proximo;
            PROXIMO();
        }while( LETRA_DIGITO() );
        if( simbolos_compostos.count(atomo) ){
            simbolo = CODIGO(atomo, 1);
        }else{
            simbolo = CODIGO(atomo, 2);
        }
    }else if( DIGITO() ){
        do{
            atomo+=proximo;
            PROXIMO();
        }while( DIGITO() == true );
        if( LETRA() ){
             ERRO(msg);
             return false;
        }
        simbolo = CODIGO(atomo, 3);
    } else
        ERRO(msg);

    return true;
}

void COMPILAR(){
    //Abertura do arquivo que contém o programa a ser analisado.
    freopen("Trab1_Compiladores.txt", "r", stdin);

    //Montar a tabela com simbolos especiais e compostos
    VOCABULARIO();

    //Inicializar pegando o primeiro caractere do programa
    PROXIMO();

    //Rodar analisador léxico e gerar os tokens para cada átomo.
    //Se houver erros deve retornar a linha que gerou o erro.
    string simbolo;

    while( ANALISADOR_LEXICO(simbolo) ){
        tokens_gerados.push_back(simbolo);
    }
}

//----------------PROGRAMA PRINCIPAL----------------
int main(){

    COMPILAR();

    return 0;
}
