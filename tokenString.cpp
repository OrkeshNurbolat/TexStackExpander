#include "tokenString.h"
#include <string>
#include <string.h>
#include <vector>



STR ex_toks; // these are the global defined tokens
VEC<TokenID> ex_tokid;  // the position and type
VEC<int> ex_senp;  // the sentence end position
int ex_tokN = 0 ; // the cont of all tokens; 
 

void initGV(){
	ex_toks="";
	while(ex_tokid.size()!=0){
		ex_tokid.pop_back();
	}
	while(ex_senp.size()!=0){
		ex_senp.pop_back();
	}
	ex_tokN = 0 ;
}

void getGV(TokenList * tl ) {
		tl->toks = ex_toks ;
		tl->tokid=ex_tokid ;
		tl->senp = ex_senp;
		tl->tokN = ex_tokN;
}

extern "C" void addToken(const char * tokenCStr , const  int type , const int lineno) {
	if('\0'==tokenCStr[0]) return ; 
	
	ex_toks+=tokenCStr;	
	if(0==ex_tokN)  {
		ex_tokid.push_back({(int)(strlen(tokenCStr)-1) , type , lineno}) ;
	} else {
		ex_tokid.push_back( { ex_tokid[ex_tokN-1].p+(int)(strlen(tokenCStr)) ,type ,lineno} );	
	}
	ex_tokN++;
	//add it to sentence end position if its is semi coulumn
	if(L_SEMI == type || laclimax == type  ) {
		ex_senp.push_back(ex_tokN-1);
		//printf("; added \n");
	}
}

STR getToken(const int i ) {
		int sp = ((0==i)?(0):(ex_tokid[i-1].p+1));
		STR tk(ex_toks, sp , ex_tokid[i].p - sp + 1 ) ;
		return tk;
}


void printTokens() {
	int sz = ex_tokid.size();
	for(int i =0 ; i < sz ; i ++) {
		STR tk = getToken(i);
		printf("%i , %i  , |%s| \n" , ex_tokid[i].p,ex_tokid[i].t , tk.c_str());

	}
}













