/*
    TRABALHO COMPILADORES - ANALISADOR LÉXICO
    NOME: MARIA ANGELICA KRÜGER MIRANDA
    RA: 201027691
    SO: WINDOWS 11
    COMPILADOR: g++ (Visual Studio Code ou Code Blocks)
*/

#include <bits/stdc++.h>

using namespace std;

char proximo;
int num_linha;
vector<string> tokens_gerados;
map<char,string> simbolos_especiais;
map<string,string> simbolos_compostos;

//Map que contem o vocabulário do Pascal simplificado
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

//Função que pega o próximo caractere da cadeia de entrada
bool PROXIMO(){
    char temp;

    //Avançar pelo arquivo pegando caractere por caractere
    //Ao chegar ao final do arquivo, fechar e mostrar os tokens gerados
    if( (scanf("%c",&temp)) == EOF ){
        cout << "Analisador lexico finalizado com sucesso." << endl;
        cout << "Tokens gerados apos a leitura do programa: " << endl;
        for(int i=0; i<=tokens_gerados.size(); i++)
            cout << tokens_gerados[i] << endl;
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

//Função que gera tokens
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

//Indica possivel erro do compilador e finaliza o programa
void ERRO(){
    cout << "Erro na linha " << num_linha + 1 << "." << endl;
    exit(1);
}

//Verifica se o caractere é um símbolo especial
bool SIMB_ESP(){
    if( simbolos_especiais.count(proximo) )
        return true;
    else
        return false;
}

//Verifica se o caractere é letra
bool LETRA(){
    if(proximo>= 'A' && proximo <= 'Z')
        return true;
    return false;
}

//Verifica se o caractere é dígito
bool DIGITO(){
    if( proximo >= '0' && proximo <= '9' )
        return true;
    return false;
}

//Verifica se o caractere é dígito ou letra
bool LETRA_DIGITO(){
    if( (proximo >= 'A' && proximo <= 'Z') || (proximo >= '0' && proximo <= '9') )
        return true;
    return false;
}

//Função que realiza a análise léxica do programa recebido
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
             ERRO();
             return false;
        }
        simbolo = CODIGO(atomo, 3);
    } else
        ERRO();

    return true;
}

//----------------PROGRAMA PRINCIPAL----------------
int main(){
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

    return 0;
}
