%{
#include<stdio.h>
#include<stdlib.h>
#include "tokenString_c.h"
//extern struct Node * add_node(const char * name , const int x); 

//extern void addToken(const char * tokenCStr , const int type) ;

extern  void addToken(const char * tokenCStr , const  int type , const int lineno);

%}

/* so here goes some of the Definitions */
letter 			[a-zA-Z]
digit 			[0-9]
lacommet 		"%"
laclimax 		"!"
nl 					"\n"|"\r\n"

L_ID 				{letter}+[a-zA-Z0-9]*
L_INT 			-?[0-9]+|0x[0-9a-fA-F]+
L_PAR 			#-?[0-9]+|0x[0-9a-fA-F]+
L_FLOAT 		([0-9]*\.?[0-9]+|[0-9]+\.)

L_COLM 			":"
L_SEMI 			";"
L_COMMA 		","
L_ASSIGNOP 	"="
L_RELOP 		">"|"<"|">="|"<="|"=="|"!="
L_PLUS 			"+"
L_MINUS 		"-"
L_STAR 			"*"
L_DSTAR 		"**"
L_SUP 			"^"
L_SUB 			"_"
L_DIV 			"/"
L_DDIV 			"//"
L_ALG 			"&"
L_BAR 			"|"
L_DOT 			"."
L_ALT 			"@"
L_DALT 			"@@"
L_NUM 			"#"
L_DNUM 			"##"
L_WHT 			"?"
L_BSL 			"\\\\"
L_PLA 			"`"
L_DOL 			"$"
L_DDOL 			"$$"
L_LP 				"("
L_RP 				")"
L_LB 				"["
L_RB 				"]"
L_LC 				"{"
L_RC 				"}"
L_CHR 			\'(\\.|[^'\\])*\'
L_STR 			\"(\\.|[^"\\])*\" 
L_INC 			#include\"(\\.|[^"\\])*\" 
L_SIM				"~"
L_VOID			[\ \t\n]*
%option yylineno
%option noyywrap


%%
{nl} { addToken(yytext , L_VOID 			,yylineno);	}

{lacommet}.* 		
{laclimax}.* 		{ addToken(yytext+1 , laclimax , yylineno); 	}

{L_ID} 					{ addToken(yytext , L_ID 			,yylineno);	}	
{L_INT} 				{ addToken(yytext , L_INT 		,yylineno);	}		
{L_PAR} 				{ addToken(yytext+1 , L_PAR 		,yylineno);	}		
{L_FLOAT} 			{ addToken(yytext , L_FLOAT 	,yylineno);	}

{L_COLM}				{ addToken(yytext , L_COLM 		,yylineno);	}
{L_SEMI}				{ addToken(yytext , L_SEMI 		,yylineno);	}	
{L_COMMA}				{ addToken(yytext , L_COMMA 	,yylineno);	}	
{L_ASSIGNOP}		{ addToken(yytext , L_ASSIGNOP,yylineno);	}
{L_RELOP}				{ addToken(yytext , L_RELOP 	,yylineno);	}		
{L_PLUS}				{ addToken(yytext , L_PLUS 		,yylineno);	}
{L_MINUS}				{ addToken(yytext , L_MINUS 	,yylineno);	}	
{L_STAR}				{ addToken(yytext , L_STAR 		,yylineno);	}
{L_DSTAR}				{ addToken(yytext , L_DSTAR 	,yylineno);	}	
{L_SUP}					{ addToken(yytext , L_ID 		,yylineno);addToken(" ", L_VOID 			,yylineno);	}
{L_SUB}					{ addToken(yytext , L_ID 		,yylineno);addToken(" ", L_VOID 			,yylineno);}
{L_DIV}					{ addToken(yytext , L_ID 		,yylineno);addToken(" ", L_VOID 			,yylineno);}
{L_DDIV}				{ addToken(yytext , L_ID 		,yylineno);addToken(" ", L_VOID 			,yylineno);}	
{L_ALG}					{ addToken(yytext , L_ALG 		,yylineno);	}
{L_BAR}					{ addToken(yytext , L_BAR 		,yylineno);	}
{L_DOT}					{ addToken(yytext , L_DOT 		,yylineno);	}
{L_ALT}					{ addToken(yytext , L_ALT 		,yylineno);	}
{L_DALT}				{ addToken(yytext , L_DALT 		,yylineno);	}	
{L_NUM}					{ addToken(yytext+1 , L_NUM 		,yylineno);	}
{L_DNUM}				{ addToken(yytext , L_DNUM 		,yylineno);	}	
{L_WHT}					{ addToken(yytext , L_WHT 		,yylineno);	}
{L_BSL}					{ addToken(yytext , L_BSL 		,yylineno);	}
{L_PLA}					{ /* addToken(yytext , L_PLA 		,yylineno);	*/}
{L_DOL}					{ addToken(yytext , L_DOL 		,yylineno);	}
{L_DDOL}				{ addToken(yytext , L_DDOL 		,yylineno);	}
{L_LP}					{ addToken(yytext , L_LP 			,yylineno);	}
{L_RP}					{ addToken(yytext , L_RP 			,yylineno);	}
{L_LB}					{ addToken(yytext , L_LB 			,yylineno);	}
{L_RB}					{ addToken(yytext , L_RB 			,yylineno);	}
{L_LC}					{ addToken(yytext , L_LC 			,yylineno);	}
{L_RC}					{ addToken(yytext , L_RC 			,yylineno);	}
{L_CHR}					{  yytext[strlen(yytext)-1]='\0';addToken(yytext+1 , L_CHR 		,yylineno);	}
{L_STR}					{  yytext[strlen(yytext)-1]='\0';addToken(yytext+1 , L_STR 		,yylineno);	}
{L_INC}					{  yytext[strlen(yytext)-1]='\0';addToken(yytext+9 , L_INC 		,yylineno);printf("\n\nHere it is included\n\n");	}
{L_SIM} 					{ addToken(yytext , L_ID 			,yylineno);	}	
{L_VOID} 					{ addToken(yytext , L_VOID 			,yylineno);	}	
              	
	            	
%%            	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
              	
