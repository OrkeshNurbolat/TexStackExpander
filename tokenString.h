#ifndef __TOKEN_STRING_H__
#define __TOKEN_STRING_H__

#include "tokenString_c.h"

#include <string>
#include <vector>

#define STR std::string
#define VEC std::vector

struct TokenID{
	int p; // token position
	int t; // token id
	int ln; //defined line number
};



void printTokens() ;
STR getToken(const int i ) ;
extern "C" void addToken(const char * tokenCStr , const  int type , const int lineno);
void initGV();


class TokenList{
public:
	STR toks; // these are the global defined tokens
	VEC<TokenID> tokid;  // the position and type
	VEC<int> senp;  // the sentence end position
	int tokN  ; // the cont of all tokens; 
	
	TokenList()
	{
	

	}
	TokenList( TokenList & TL):toks(TL.toks) ,tokid(TL.tokid),senp(TL.senp) , tokN(TL.tokN){
	}
	
	TokenList(STR & toks_in  , VEC<TokenID> & tokid_in , VEC<int> senp_in , int tokN_in)
		:toks(toks_in) ,tokid(tokid_in),senp(senp_in) , tokN(tokN_in)
	{
	
	}

	STR getToken(const int i ) {
			int sp = ((0==i)?(0):(tokid[i-1].p+1));
			STR tk(toks, sp , tokid[i].p - sp + 1 ) ;
			return tk;
	}
		
	void printTokens() {
		int sz = tokid.size();
		for(int i =0 ; i < sz ; i ++) {
			STR tk = getToken(i);
			printf("%i , %i  , |%s| \n" , tokid[i].p,tokid[i].t , tk.c_str());

		}
	}


	~TokenList(){
	}

};



void getGV(TokenList * tl ) ; 



#endif
