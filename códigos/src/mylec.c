#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Enumeração dos tipos de tokens léxicos
typedef enum {
    PALAVRA_CHAVE, OPERADOR_LOGICO, OPERADOR_RELACIONAL, OPERADOR_ARITMETICO,
    ATRIBUICAO, DELIMITADOR, NUMERO_INTEIRO, NUMERO_REAL, IDENTIFICADOR,
    TOKEN_DESCONHECIDO
} TipoToken;




// Enumeração dos tipos de tokens sintáticos
typedef enum {
    TIPO_INDEFINIDO,
    TIPO_IDENTIFICADOR,
    TIPO_TIPO,
    TIPO_PONTO_VIRGULA,
    TIPO_ABRE_PARENTESES,
    TIPO_FECHA_PARENTESES,
    TIPO_VIRGULA,
    TIPO_ABRE_CHAVES,
    TIPO_FECHA_CHAVES,
    TIPO_SE,
    TIPO_ENTAO,
    TIPO_SENAO,
    TIPO_ENQUANTO,
    TIPO_REPITA,
    TIPO_ATE,
    TIPO_LER,
    TIPO_MOSTRAR,
    TIPO_NUMERO_INTEIRO,
    TIPO_NUMERO_REAL,
    TIPO_OPERADOR_LOGICO,
    TIPO_OPERADOR_RELACIONAL,
    TIPO_OPERADOR_ARITMETICO,
    TIPO_ATRIBUICAO
} TipoTokenS;
// Estrutura que representa um token
typedef struct {
    TipoToken tipo;
    TipoTokenS tipo_sintatico;
    char lexema[MAX_TOKEN_LENGTH];
    int linha;  
} Token;

// Varivael que armazena o código
Token *codigo;
int  contador_tokens = 0;
// Contadores para o resumo de tokens
int contador_palavras_chave = 0;
int contador_identificadores = 0;
int contador_inteiros = 0;
int contador_reais = 0;
int contador_operadores = 0;
int contador_delimitadores = 0;
int contador_token_desconhecido = 0;

// Palavras-chave da linguagem P-
const char* palavras_chave[] = {
    "inteiro", "real", "se", "entao", "senao", "enquanto", "repita", "ate", "ler", "mostrar"
};
int num_palavras_chave = 10;

// Função que verifica se o lexema é uma palavra-chave
int eh_palavra_chave(const char* lexema) {
    for (int i = 0; i < num_palavras_chave; i++) {
        if (strcmp(lexema, palavras_chave[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função que classifica o token com base no lexema
Token classificar_token(const char* lexema, int linha) {
    Token token;
    strcpy(token.lexema, lexema);
    token.linha = linha;
    if (eh_palavra_chave(lexema)) {
        token.tipo = PALAVRA_CHAVE;
        contador_palavras_chave++;
    } else if (strcmp(lexema, "&&") == 0 || strcmp(lexema, "||") == 0) {
        token.tipo = OPERADOR_LOGICO;
        contador_operadores++;
    } else if (strcmp(lexema, "<") == 0 || strcmp(lexema, "<=") == 0 ||
               strcmp(lexema, ">") == 0 || strcmp(lexema, ">=") == 0 ||
               strcmp(lexema, "==") == 0 || strcmp(lexema, "!=") == 0) {
        token.tipo = OPERADOR_RELACIONAL;
        contador_operadores++;
    } else if (strcmp(lexema, "+") == 0 || strcmp(lexema, "-") == 0 ||
               strcmp(lexema, "*") == 0 || strcmp(lexema, "/") == 0) {
        token.tipo = OPERADOR_ARITMETICO;
        contador_operadores++;
    } else if (strcmp(lexema, "=") == 0) {
        token.tipo = ATRIBUICAO;
        contador_operadores++;
    } else if (strcmp(lexema, ";") == 0 || strcmp(lexema, ",") == 0 ||
               strcmp(lexema, "(") == 0 || strcmp(lexema, ")") == 0 ||
               strcmp(lexema, "{") == 0 || strcmp(lexema, "}") == 0) {
        token.tipo = DELIMITADOR;
        contador_delimitadores++;
    } else if (isdigit(lexema[0])) {
        if (strchr(lexema, '.') != NULL) {
            token.tipo = NUMERO_REAL;
            contador_reais++;
        } else {
            token.tipo = NUMERO_INTEIRO;
            contador_inteiros++;
        }
    } else if (isalpha(lexema[0]) || lexema[0] == '_') {
        token.tipo = IDENTIFICADOR;
        contador_identificadores++;
    } else {
        token.tipo = TOKEN_DESCONHECIDO;
        fprintf(stderr, "Erro lexico: Token desconhecido '%s' encontrado na linha %d. ",
                        lexema, linha);
        exit(1);
    }

    return token;
}

//Função que imprime lista de tokens
void imprimeTokens(){
      const char* tipo_token[] = {
        "PALAVRA-CHAVE", "OPERADOR_LOGICO", "OPERADOR_RELACIONAL", "OPERADOR_ARITMETICO",
        "ATRIBUICAO", "DELIMITADOR", "INTEIRO", "REAL", "IDENTIFICADOR", "TOKEN DESCONHECIDO"
    };
   for (int i = 0; i < contador_tokens; i++) {
        printf("%-15s <===> %s\n", tipo_token[codigo[i].tipo], codigo[i].lexema);
    }

}

// Função que exibe o token e o seu tipo na formatação desejada
void inserir_token(Token token) {
    contador_tokens = contador_tokens +1;
    Token *temp = realloc(codigo, (contador_tokens + 1) * sizeof(Token));
    codigo = temp;
    strcpy(codigo[contador_tokens-1].lexema, token.lexema);
    codigo[contador_tokens-1].linha = token.linha;
    codigo[contador_tokens-1].tipo = token.tipo;
}

// Função que analisa o código de entrada e identifica os tokens
void analisar_lexico(const char* codigo, int linha) {
    int i = 0;
    char lexema[MAX_TOKEN_LENGTH];
    int pos_lexema = 0;

    while (codigo[i] != '\0') {
        // Ignora espaços em branco
        if (isspace(codigo[i])) {
            i++;
            continue;
        }

        // Identifica identificadores ou palavras-chave
        if (isalpha(codigo[i]) || codigo[i] == '_') {
            pos_lexema = 0;
            while (isalnum(codigo[i]) || codigo[i] == '_') {
                lexema[pos_lexema++] = codigo[i++];
            }
            lexema[pos_lexema] = '\0';
            Token token = classificar_token(lexema, linha);
            inserir_token(token);
        }
        // Identifica números inteiros ou reais
        else if (isdigit(codigo[i])) {
            pos_lexema = 0;
            while (isdigit(codigo[i])) {
                lexema[pos_lexema++] = codigo[i++];
            }
            if (codigo[i] == '.') {
                lexema[pos_lexema++] = codigo[i++];
                while (isdigit(codigo[i])) {
                    lexema[pos_lexema++] = codigo[i++];
                }
            }
            lexema[pos_lexema] = '\0';
            Token token = classificar_token(lexema, linha);
            inserir_token(token);
        }
        // Identifica operadores, delimitadores e símbolos
        else {
            lexema[0] = codigo[i++];
            lexema[1] = '\0';

            // Verifica operadores lógicos
            if (lexema[0] == '&' && codigo[i] == '&') {
                lexema[1] = codigo[i++];
                lexema[2] = '\0';
            } else if (lexema[0] == '|' && codigo[i] == '|') {
                lexema[1] = codigo[i++];
                lexema[2] = '\0';
            }
            // Verifica operadores relacionais de dois caracteres (==, !=, <=, >=)
            else if (lexema[0] == '=' || lexema[0] == '!' ||
                     lexema[0] == '<' || lexema[0] == '>') {
                if (codigo[i] == '=') {
                    lexema[1] = codigo[i++];
                    lexema[2] = '\0';
                }
            }

            Token token = classificar_token(lexema, linha);
            inserir_token(token);
        }
    }
}

// Função para exibir o resumo dos tokens
void exibir_resumo_tokens() {
    printf("\nResumo de Tokens:\n");
    printf("Palavras-chave    : %d\n", contador_palavras_chave);
    printf("Identificadores   : %d\n", contador_identificadores);
    printf("Números Inteiros  : %d\n", contador_inteiros);
    printf("Números Reais     : %d\n", contador_reais);
    printf("Operadores        : %d\n", contador_operadores);
    printf("Delimitadores     : %d\n", contador_delimitadores);
    printf("Tokens Desconhecidos : %d\n", contador_token_desconhecido);
}



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// ARVORE SINTÁTICA
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// Definir a árvore sintática
typedef struct ArvoreSintatica {
    char* nome;
    struct ArvoreSintatica** filhos;
    int num_filhos;
} ArvoreSintatica;

// Função para criar um nó da árvore sintática
ArvoreSintatica* criar_no(char* nome) {
    ArvoreSintatica* no = (ArvoreSintatica*)malloc(sizeof(ArvoreSintatica));
    no->nome = nome;
    no->filhos = NULL;
    no->num_filhos = 0;
    return no;
}

// Função para destruir a árvore sintática
void destruir_arvore(ArvoreSintatica* arvore) {
    if (arvore) {
        for (int i = 0; i < arvore->num_filhos; i++) {
            destruir_arvore(arvore->filhos[i]);
        }
        free(arvore->filhos);
        free(arvore);
    }
}



 

// Função principal para analisar o programa
ArvoreSintatica* analisar_programa(int *index) {
    ArvoreSintatica* programa = criar_no("programa");
    programa->filhos = realloc(programa->filhos, sizeof(ArvoreSintatica*) * (programa->num_filhos + 2));
    programa->filhos[0] = criar_no("bloco_variaveis");
    programa->filhos[1] = criar_no("corpo_programa");
    
    while ((*index) <= contador_tokens){
        switch (codigo[*index].tipo_sintatico) {

            case TIPO_TIPO:
                int indexFim = (*index);
                 programa->filhos[0]->filhos = realloc(programa->filhos[0]->filhos, sizeof(ArvoreSintatica*) * (programa->filhos[0]->num_filhos + 1));
                 
            // se começar com um tipo é um declaração
                // obter tamanho do bloco
                while (codigo[*index].tipo_sintatico != TIPO_PONTO_VIRGULA){
                    indexFim++;
                }
                indexFim++;
                programa->filhos[0]->filhos[programa->num_filhos++] = analisar_declaracao(index, indexFim);
                (*index) = indexFim+1;
                break;

        case TIPO_SE:
            int indexFim = (*index);
                 programa->filhos[1]->filhos = realloc(programa->filhos[0]->filhos, sizeof(ArvoreSintatica*) * (programa->filhos[0]->num_filhos + 1));
            while (codigo[*index].tipo_sintatico != TIPO_PONTO_VIRGULA){
                        indexFim++;
                    }
            indexFim++;
                programa->filhos[1]->filhos[programa->num_filhos++] = analisar_condicao(index, indexFim);
                (*index) = indexFim+1;
                break;

        }
    }
    // Analisa o bloco de variáveis
    programa->filhos = realloc(programa->filhos, sizeof(ArvoreSintatica*));
    programa->filhos[programa->num_filhos++] = analisar_bloco_variaveis(&index);

    // Analisa o corpo do programa
    programa->filhos = realloc(programa->filhos, sizeof(ArvoreSintatica*) * (programa->num_filhos + 1));
    programa->filhos[programa->num_filhos++] = analisar_comandos(&index);

    return programa;
}
int obterFimBloco(TipoTokenS limiteFinal,int index){
    int inicio_bloco =  index; 
        int fim_bloco = inicio_bloco;
        int nivelFechamento = 1;
        while (codigo[fim_bloco].tipo_sintatico != limiteFinal && nivelFechamento == 0){
            if (codigo[fim_bloco].tipo_sintatico == TIPO_ABRE_PARENTESES) {
              nivelFechamento++;
            }
            if (codigo[fim_bloco].tipo_sintatico == TIPO_FECHA_PARENTESES) {
                nivelFechamento--;
            }
            switch (codigo[fim_bloco].tipo_sintatico)
            {
                case TIPO_IDENTIFICADOR:
                case TIPO_NUMERO_INTEIRO:
                case TIPO_NUMERO_REAL:
                case TIPO_OPERADOR_ARITMETICO:
                case TIPO_OPERADOR_LOGICO:
                case TIPO_OPERADOR_RELACIONAL:
                break;

                default:
                    printf('Token não esperado em  int obterFimBloco(TipoTokenS limiteFinal,int index){ ');
                return NULL;
                break;
            }
            fim_bloco++;

        }
    return fim_bloco;
}
void adicionar_filho(ArvoreSintatica* no_pai, ArvoreSintatica* no_filho) {
    no_pai->num_filhos++;
    no_pai->filhos = (ArvoreSintatica**)realloc(no_pai->filhos, no_pai->num_filhos * sizeof(ArvoreSintatica*));
    no_pai->filhos[no_pai->num_filhos - 1] = no_filho;
}




ArvoreSintatica* analisar_expressao(int ini_bloco,int fim_bloco,char* nome){
    /*
    <expressao_logica> ::= <expressao_relacional> (("&&" | "||")<expressao_relacional>)*
<expressao_relacional> ::= <expressao_aritmetica> (<operador_relacional><expressao_aritmetica>)?
<operador_relacional> ::= "<" | "<=" | ">" | ">=" | "==" | "!="
<expressao_aritmetica> ::= <termo> (("+" | "-") <termo>)*
<termo> ::= <fator> (("*" | "/") <fator>)*
<fator> ::= "(" <expressao> ")" | <numero> | <identificador> | "-" <fator> | "!" <fator>

    */
    ArvoreSintatica* no_expressao = criar_no(nome);
    int qtd = 0;
     for (int i = ini_bloco; i < fim_bloco; i++) {
        //procurar sub expressões
        if (codigo[i].tipo_sintatico == TIPO_ABRE_PARENTESES) {
            int fim_bloco =  obterFimBloco(TIPO_FECHA_CHAVES, i+1);
            
            adicionar_filho(no_expressao,analisar_expressao(i+1,fim_bloco,'Expressao'));
          
            qtd++;

        }
        if (codigo[i].tipo_sintatico == TIPO_OPERADOR_RELACIONAL) {
            strcpy(no_expressao->nome,"Expressao relacional");
            ArvoreSintatica* relacao = criar_no(codigo[i].lexema);
              // montar nó expressão relcional TIPO_OPERADOR_RELACIONAL
            // é uma expressão relacional
            // se  i-2 não é TIPO_OPERADOR_aritimético
                    // se i-1 é numero ou identificador
            if (codigo[i-2].tipo_sintatico != TIPO_OPERADOR_ARITMETICO) { // Se i-2 não for um operador aritmético
    if (codigo[i-1].tipo_sintatico == TIPO_NUMERO_REAL || codigo[i-1].tipo_sintatico == TIPO_NUMERO_INTEIRO || codigo[i-1].tipo_sintatico == TIPO_IDENTIFICADOR) {
            adicionar_filho(relacao,criar_no(codigo[i-1].lexema));
     // montar filhoEsquerda com  i-1
    }           
                    //else
                         // ERRO 
        else {
        // Se i-1 não for número nem identificador, erro
        printf("Erro: Esperado número ou identificador em i-1 (esquerda).\n");
        return;
        }
        }
            //else
                //OBTER FILHO ESQUERDA DE RECURSIVIDADE
                // ArvoreSintatica* operacao_relacional2 = analisar_expressao(i+1,fim_bloco); 
    else {
    // Se i-2 for um operador aritmético, realizar a recursão para obter o filho esquerdo
    ArvoreSintatica* operacao_relacional2 = analisar_expressao(i + 1, fim_bloco,'Expressao');
    adicionar_filho(relacao, operacao_relacional2);
}
if (codigo[i+2].tipo_sintatico != TIPO_OPERADOR_ARITMETICO) {
    if (codigo[i+1].tipo_sintatico == TIPO_NUMERO || codigo[i+1].tipo_sintatico == TIPO_IDENTIFICADOR) {
        // Se i+1 for número ou identificador, adicionar à árvore como filho direito
        adicionar_filho(relacao, criar_no(codigo[i+1].lexema)); 
    } else {
        // Se i+1 não for número nem identificador, erro
        printf("Erro: Esperado número ou identificador em i+1 (direita).\n");
        return; // Interrompe a execução por erro
    }
} else {
    // Se i+2 for um operador aritmético, realizar a recursão para obter o filho direito
    ArvoreSintatica* operacao_relacional2 = analisar_expressao(i + 1, fim_bloco);
    adicionar_filho(relacao, operacao_relacional2);
}

// Adicionar o nó de expressão relacional à árvore sintática
adicionar_filho(no_expressao, relacao);
     break;
           //SAIR DO FOR

        }
          if (codigo[i].tipo_sintatico == TIPO_OPERADOR_LOGICO) {
            int fim_bloco =  obterFimBloco(TIPO_OPERADOR_LOGICO, i+1);
            ArvoreSintatica* operacao_relacional2 = analisar_expressao(i+1,fim_bloco); 
            qtd++;

        }
        // termo aritimético
        // operador aritimético
        // operador relacional
        // operador lógico

        // procurar numero
        // identificador
        

     }
     if(qtd == 0){
        printf('Nenhum operador relacional');
     }
TIPO_OPERADOR_RELACIONAL
}

ArvoreSintatica* analisar_condicao(int index_inicial, int index_fim) {
/*
<condicao> ::= "se"”(“ <expressao_logica> ")””entao””{“ <comando> “}”["senao"”{” <comando>”}”]

 */


    ArvoreSintatica* no_condicao = criar_no("Condicao");
    int index = index_inicial;

    // Verificar se o primeiro token é "se"
    if (codigo[index].tipo_sintatico == TIPO_SE) {
        adicionar_filho(no_condicao, criar_no(codigo[index].lexema));
        index++;
    } else {
        printf('Palavra chave "se" esperada na ArvoreSintatica* analisar_condicao(int index_inicial, int index_fim) 325 ');
        // Erro: início de condição esperado
        return NULL;
    }
    if (codigo[index].tipo_sintatico != TIPO_ABRE_PARENTESES) {
        printf('Inicio de bloco esperada na ArvoreSintatica* analisar_condicao(int index_inicial, int index_fim) 330 ');
        // Erro: início de condição esperado
        return NULL;
    }else{
        
        int fim_bloco =  obterFimBloco(TIPO_FECHA_PARENTESES, index++); 
       
        ArvoreSintatica* no_expressao_logica = analisar_expressao(index, fim_bloco);
        if (no_expressao_logica != NULL) {
            adicionar_filho(no_condicao, no_expressao_logica);
        } else {
            // Erro: expressão lógica inválida
            return NULL;
        }
        index  = 1+ fim_bloco;
    }
    // Verificar se o próximo token é "entao"
    if (codigo[index].tipo == TIPO_ENTAO) {
        adicionar_filho(no_condicao, criar_no(codigo[index].lexema));
        index++;
    } else {
        printf('xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 381');
        // Erro: "entao" esperado
        return NULL;
    }
    if (codigo[index].tipo_sintatico != TIPO_ABRE_CHAVES) {
        printf('Inicio de bloco esperada na ArvoreSintatica* analisar_condicao(int index_inicial, int index_fim) 385 ');
        // Erro: início de condição esperado
        return NULL;
    }else{
        int fim_bloco =  obterFimBloco(TIPO_FECHA_CHAVES, index++); 
        
        ArvoreSintatica* no_comando = analisar_comando(index, fim_bloco);
        if (no_comando != NULL) {
            adicionar_filho(no_condicao, no_comando);
        } else {
            // Erro: comando inválido
            return NULL;
        }
        index  = 1+ fim_bloco;
    }



    // Verificar se existe o token "senao" seguido de outro <comando>
    if (codigo[index].tipo == TIPO_SENAO) {
            adicionar_filho(no_condicao, criar_no(codigo[index].lexema));
            index++;
        if (codigo[index].tipo_sintatico != TIPO_ABRE_CHAVES) {
            printf('Inicio de bloco esperada na ArvoreSintatica* analisar_condicao(int index_inicial, int index_fim) 385 ');
            // Erro: início de condição esperado
            return NULL;
        }else{
            int fim_bloco =  obterFimBloco(TIPO_FECHA_CHAVES, index++); ;
         
            ArvoreSintatica* no_comando = analisar_comando(index, fim_bloco);
            if (no_comando != NULL) {
                adicionar_filho(no_condicao, no_comando);
            } else {
                // Erro: comando inválido
                return NULL;
            }
            index  = 1+ fim_bloco;
        }
            return no_condicao;
    }
}
/*
*/
ArvoreSintatica* analisar_declaracao( int index0,int index1) {
   ArvoreSintatica* declaracao = criar_no("declaracao");
   adicionar_filho(declaracao, criar_no(codigo[index0].lexema));
   
 
  int ini = index0;
  ini++;
  if (codigo[ini].tipo_sintatico != TIPO_IDENTIFICADOR){
    printf('Esperado um identificador na declaração! ArvoreSintatica* analisar_declaracao( int index0,int index1 306');
    return NULL;
  }
  while(ini >= index1){
     if (codigo[ini].tipo_sintatico == TIPO_VIRGULA || codigo[ini].tipo_sintatico == TIPO_PONTO_VIRGULA)
     {
        if (codigo[ini-1].tipo_sintatico != TIPO_IDENTIFICADOR && codigo[ini-1].tipo_sintatico != TIPO_NUMERO_INTEIRO && codigo[ini-1].tipo_sintatico != TIPO_NUMERO_REAL){
            printf('Esperado um identificador na declaração! ArvoreSintatica* analisar_declaracao( int index0,int index1 314');
            return NULL;
        }else{
                if (codigo[ini-1].tipo_sintatico == TIPO_IDENTIFICADOR){
                    adicionar_filho(declaracao, criar_no(codigo[ini-1].lexema));
                  

                } else{if (codigo[ini-2].tipo_sintatico != TIPO_ATRIBUICAO){
                        printf('Esperado uma atribuição na declaração! ArvoreSintatica* analisar_declaracao( int index0,int index1 322');
    return NULL;
                }else{
                    
                    
                //    declaracao->filhos = realloc(declaracao->filhos, sizeof(ArvoreSintatica*) * (declaracao->num_filhos + 1));
                    
                    ArvoreSintatica* atribuicao = criar_no(codigo[ini-2].lexema);
                    adicionar_filho(atribuicao, criar_no(codigo[ini-3].lexema));
                    adicionar_filho(atribuicao, criar_no(codigo[ini-1].lexema));
                    adicionar_filho(declaracao, atribuicao);
                    
                 //   atribuicao->filhos = realloc(atribuicao->filhos, sizeof(ArvoreSintatica*) * (atribuicao->num_filhos + 2));
                 //   atribuicao->filhos[0] = criar_no(codigo[ini-3].lexema);
                 //   atribuicao->filhos[1] = criar_no(codigo[ini-1].lexema);
                 //   declaracao->filhos[declaracao->num_filhos++] = atribuicao; 

                }

                }

        }

     
     }
    ini++;
  }
    return declaracao;
}


void classificacaoSintatica(){
    for (int i = 0; i < contador_tokens; i++) {
      
  
 switch (codigo[i].tipo) {

case PALAVRA_CHAVE:
            if (strcmp(codigo[i].lexema, "inteiro")|| strcmp(codigo[i].lexema, "real")){
                codigo[i].tipo_sintatico = TIPO_TIPO;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "se")){
                codigo[i].tipo_sintatico = TIPO_SE;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "entao")){
                codigo[i].tipo_sintatico = TIPO_ENTAO;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "senao")){
                codigo[i].tipo_sintatico = TIPO_SENAO;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "enquanto")){
                codigo[i].tipo_sintatico = TIPO_ENQUANTO;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "repita")){
                codigo[i].tipo_sintatico = TIPO_REPITA;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "ate")){
                codigo[i].tipo_sintatico = TIPO_ATE;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "ler")){
                codigo[i].tipo_sintatico = TIPO_LER;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "mostrar")){
                codigo[i].tipo_sintatico = TIPO_SE;
                break;  // Sai do switch
            }
            codigo[i].tipo_sintatico = TIPO_INDEFINIDO;
            printf(" Palavra chave não programada!\n");
            break;  // Sai do switch
        case DELIMITADOR:
            if (strcmp(codigo[i].lexema, ";")){
                    codigo[i].tipo_sintatico = TIPO_PONTO_VIRGULA;
                    break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "(")){
                codigo[i].tipo_sintatico = TIPO_ABRE_PARENTESES;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, ")")){
                codigo[i].tipo_sintatico = TIPO_FECHA_PARENTESES;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, ",")){
                codigo[i].tipo_sintatico = TIPO_VIRGULA;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "{")){
                codigo[i].tipo_sintatico = TIPO_ABRE_CHAVES;
                break;  // Sai do switch
            }
            if (strcmp(codigo[i].lexema, "}")){
                codigo[i].tipo_sintatico = TIPO_FECHA_CHAVES;
                break;  // Sai do switch
            }
            codigo[i].tipo_sintatico = TIPO_INDEFINIDO;
            printf(" Palavra chave não programada!\n");
            break;  // Sai do switch
  case NUMERO_INTEIRO:
        codigo[i].tipo_sintatico = TIPO_NUMERO_INTEIRO;
        break;  // Sai do switch
  case NUMERO_REAL:
        codigo[i].tipo_sintatico = TIPO_NUMERO_REAL;
        break;  // Sai do switch
  case OPERADOR_LOGICO:
        codigo[i].tipo_sintatico = TIPO_OPERADOR_LOGICO;
        break;  // Sai do switch
  case OPERADOR_RELACIONAL:
        codigo[i].tipo_sintatico = TIPO_OPERADOR_RELACIONAL;
        break;  // Sai do switcH
  case OPERADOR_ARITMETICO:
        codigo[i].tipo_sintatico = TIPO_OPERADOR_ARITMETICO;
        break;  // Sai do switch
  case ATRIBUICAO:
        codigo[i].tipo_sintatico = TIPO_ATRIBUICAO;
        break;  // Sai do switch
  case IDENTIFICADOR:
        codigo[i].tipo_sintatico = TIPO_IDENTIFICADOR;
        break;  // Sai do switch
        default:
        codigo[i].tipo_sintatico = TIPO_INDEFINIDO;
            printf(" Tipo sintático ainda não programado!\n");
            break;  // Opcional, pois é o final do switch
    }  } 
}

// Função para analisar uma declaração
ArvoreSintatica* analisar_declaracao( int index0,int index1) {
   
  
    if (codigo[index0].tipo_sintatico == TIPO_TIPO) {
        // Tipo encontrado
        (*index)++;  // Consome o tipo
        declaracao->filhos = realloc(declaracao->filhos, sizeof(ArvoreSintatica*) * (declaracao->num_filhos + 1));
        declaracao->filhos[declaracao->num_filhos++] = criar_no(codigo[*index - 1].lexema);  // Tipo

        if (codigo[*index].tipo_sintatico == TIPO_IDENTIFICADOR) {
            declaracao->filhos = realloc(declaracao->filhos, sizeof(ArvoreSintatica*) * (declaracao->num_filhos + 1));
            declaracao->filhos[declaracao->num_filhos++] = criar_no(codigo[*index].lexema);  // Identificador
            (*index)++;  // Consome o identificador
        }

        if (codigo[*index].tipo_sintatico == TIPO_PONTO_VIRGULA) {
            (*index)++;  // Consome o ponto e vírgula
        }
    }

    return declaracao;
}

// Função para analisar os comandos do programa
ArvoreSintatica* analisar_comandos(int* index) {
    ArvoreSintatica* comandos = criar_no("comandos");

    while (codigo[*index].tipo_sintatico != TIPO_FECHA_CHAVES) {
        comandos->filhos = realloc(comandos->filhos, sizeof(ArvoreSintatica*) * (comandos->num_filhos + 1));
        comandos->filhos[comandos->num_filhos++] = analisar_comando(index);
        if (codigo[*index].tipo_sintatico == TIPO_PONTO_VIRGULA) {
            (*index)++;  // Consome o ponto e vírgula
        }
    }

    return comandos;
}

// Função para analisar um comando específico
ArvoreSintatica* analisar_comando(int* index) {
    if (codigo[*index].tipo_sintatico == TIPO_SE) {
        return analisar_condicao(index);
    }
    // Adicionar outros comandos conforme necessário
    return NULL;
}

// Função para analisar uma condição
ArvoreSintatica* analisar_condicao(int* index) {
    ArvoreSintatica* condicao = criar_no("condicao");
    (*index)++;  // Consome "se"
    condicao->filhos = realloc(condicao->filhos, sizeof(ArvoreSintatica*) * (condicao->num_filhos + 1));
    condicao->filhos[condicao->num_filhos++] = analisar_expressao(index);
    // Verifica "entao" e consome
    if (codigo[*index].tipo_sintatico == TIPO_ENTAO) {
        (*index)++;
    }
    return condicao;
}

// Função para analisar expressões
ArvoreSintatica* analisar_expressao(int* index) {
    ArvoreSintatica* expressao = criar_no("expressao");

    if (codigo[*index].tipo_sintatico == TIPO_NUMERO_INTEIRO || codigo[*index].tipo_sintatico == TIPO_NUMERO_REAL || codigo[*index].tipo_sintatico == TIPO_IDENTIFICADOR) {
        expressao->filhos = realloc(expressao->filhos, sizeof(ArvoreSintatica*) * (expressao->num_filhos + 1));
        expressao->filhos[expressao->num_filhos++] = criar_no(codigo[*index].lexema);
        (*index)++;  // Consome o token
    }

    return expressao;
}

void imprimir_arvore(ArvoreSintatica* arvore, int nivel) {
    if (arvore != NULL) {
        // Imprimir o nome do nó com indentação para mostrar a hierarquia
        for (int i = 0; i < nivel; i++) {
            printf("  ");
        }
        printf("%s\n", arvore->nome);

        // Recursivamente imprimir os filhos
        for (int i = 0; i < arvore->num_filhos; i++) {
            imprimir_arvore(arvore->filhos[i], nivel + 1);
        }
    }
}


// Função principal que recebe o caminho do arquivo e chama o analisador léxico
int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos está correto
    if (argc != 3) {
        printf("Uso: %s -p <caminho_do_arquivo>\n", argv[0]);
        return 1;
    }

    // Verifica se o argumento -p foi passado
    if (strcmp(argv[1], "-p") == 0) {
        const char *filepath = argv[2];  // Caminho do arquivo passado pelo usuário
        printf("1Caminho do arquivo recebido: %s\n", filepath);

        FILE *arquivo = fopen(filepath, "r");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }

        // Lê o conteúdo do arquivo linha por linha
        char linha[256];
        int linha_atual = 1;
        printf("Lendo o arquivo %s...\n\n", filepath);
        while (fgets(linha, sizeof(linha), arquivo)) {
            analisar_lexico(linha, linha_atual);
            linha_atual++;
        }

        // Fecha o arquivo após a leitura
        fclose(arquivo);

        // Exibe o resumo dos tokens
        exibir_resumo_tokens();
        imprimeTokens();

        ArvoreSintatica *arvre = analisar_programa();
    } else {
        printf("Argumento inválido! Uso: %s -p <caminho_do_arquivo>\n", argv[0]);
        return 1;
    }

    return 0;
}
