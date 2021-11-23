#ifndef __EXPAND_H__
#define __EXPAND_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<string>
#include "macroni.h"

class TabNode {
public : 
	int k ; 
	std::string s;
	TabNode * n;
	TabNode() {
		n=NULL;
	}
	TabNode(const TabNode & tn) 
		:k(tn.k),s(tn.s) {
		n=NULL;
	}
	TabNode(const int k_in , const std::string & s_in)
		: k(k_in) , s(s_in){
		n=NULL;
	}

	~TabNode(){
	}

} ; 

class TabHead{
public:
	TabNode * h;
	TabHead * v;
	TabHead (){
		h = NULL ; 
		v = NULL ; 
	}
	TabHead ( const TabHead * th ){
		h = NULL ; 
		v = NULL ; 
		TabNode * it = th->h;
		TabNode * e = NULL ; 
		while(it!=NULL){
			TabNode * p = new TabNode(*it);
			if(NULL==e) {
				h = p ; 
			} else {
				e->n = p ; 
			}
			e = p ; 
			it=it->n;	
		}
	}

	TabHead ( const std::vector<TabNode> & ts ){
		h = NULL ; 
		v = NULL ; 
		TabNode * e = NULL ; 
		for( auto it = ts.begin(); it!=ts.end();it++) {
			TabNode * p = new TabNode( *it );
			if(NULL==e) {
				h = p ; 
			} else {
				e->n = p ; 
			}
			e = p ; 
		}
	}
	
	void copy ( TabNode **reth , TabNode **rete ) {
		(*reth) = NULL;
		(*rete) = NULL ; 
		TabNode * it = h;
		while(it!=NULL) {
			TabNode * p = new TabNode(*it);
			if(NULL==(*rete)) {
				(*reth) = p ; 
			} else {
				(*rete)->n = p ; 
			}
			(*rete) = p ; 
			it=it->n;	
		}
	}
	void append( const int k, const std::string & s){
		TabNode * e = getEnd();
		TabNode * p = new TabNode(k,s);
		if(NULL==e) {
			h = p ; 
		} else {
			e->n = p ; 
		}
	}
	
	void append( const std::vector<TabNode> & ts){
		TabNode * e = getEnd();
		for(auto it = ts.begin();it!=ts.end();it++) {
			TabNode * p = new TabNode(*it);
			if(NULL==e) {
				h = p ; 
			} else {
				e->n = p ; 
			}
			e = p ; 
		}
	}
	TabNode * getEnd(){
		TabNode * it =  h  ;
		while(it!=NULL && it->n!=NULL) it=it->n; 
		return it ; 	
	}
	void printRow() {
		TabNode * it =  h  ;
		while(it!=NULL ) {
			printf("(%i,%s) " , it->k,it->s.c_str());	
			it=it->n;
		} printf("\n");
	}
	std::string genStr() {
		TabNode * it =  h  ;
		std::string ret = "" ; 
		while(it!=NULL ) {
			ret+=(it->k==0)?(it->s):("\\#"+std::to_string(it->k));	
			it=it->n;
		} 
		return ret ; 
	}
	int opCNT(){
		int x= 0 ;
		TabNode * it =  h  ;
		while(it!=NULL ) {
			x = (x > it->k)?x:(it->k);	
			it=it->n;
		} 
		return x ;
	}
	int frontOp(){
		TabNode * it =  h  ;
		while(it!=NULL ) {
			if(it->k == -1 ) return -1; 	
			it=it->n;
		} 
		return 0;
	}
	~TabHead (){
		TabNode * it =  h  ;
		TabNode * itt =  NULL  ;
		while(it!=NULL ) {
			itt = it ; 
			it = it->n;
			delete itt;
		}
	}
} ; 





class CharNode {
public:
	char c ; // the char
	CharNode*s; // sibling
	CharNode*d; // child or decendent
	TabHead * z ;  // valued part :  
	CharNode(){
		s = NULL ; 
		c = 0 ; 
		d = NULL ; 
		z = NULL ; 
	}

	~CharNode(){
		//printf("here delete CharNode %c\n" , c);
	}

}; 

/*an implement of string tree map*/
class StrMap{
public:
	CharNode * r ; // the root of the string
	StrMap(){
		r = NULL;
	}

	CharNode * mkPath(const char * s ){
		/* basically just create or find a position */
		if(NULL==s || '\0'==s[0]) return NULL;

		CharNode * it = r; 
		CharNode * itps = NULL; // the sibling pointing it
		CharNode * itpd = NULL; // the father desend to be it

		for(int stp=0 ; s[stp]!='\0' ; stp++) { 
			char c = s[stp];
			bool sf = false ; /* flag indicates found at sibling */
			/* sibling iteration , they are sorted */
			while(it!=NULL && c >= it->c ) { 
				if(c == it->c) {
					sf=true;
					break ;
				}
				itps = it ; 
				it = it->s;
			}
			/* siblings not found c */	
			if(!sf) {
				CharNode * itt  = it ; 	
				it = new CharNode();
				it->c = c ; 
				it->s = itt;
				
				if(NULL!=itps)itps->s = it ; 
				if(NULL!=itpd && (NULL==itpd->d || itt == itpd->d ))itpd->d = it ; 
				if(NULL==r || itt == r) r = it ; 
			}
			/* desend down */
			itps = NULL;
			itpd = it ; 
			it = it->d;
		}
		return itpd ; 
	}


	CharNode * fdPath(const char * s ){
		/* basically just create or find a position */
		if(NULL==s || '\0'==s[0]) return NULL;

		CharNode * it = r; 
		CharNode * itpd = NULL ;
		for(int stp=0 ; s[stp]!='\0' ; stp++) { 
			char c = s[stp] ;
			/* sibling iteration , they are sorted */
			bool sf = false ; 
			while(it!=NULL && c >= it->c ) { 
				if(c == it->c) {
					itpd = it ; 
					it = it->d;
					sf = true ; 
					break ;
				}
				it = it->s;
			} 
			if(!sf)return NULL ;
		}
		return itpd; 
	}
	
	void printDFS( const char *  s , CharNode  * cnn=NULL){
		CharNode * cn = (NULL==cnn)?(r):(cnn);	
		int sl = strlen(s); 
		char * z = (char *) malloc( sl+2) ; 
		strcpy(z,s) ; 
		z[sl] = cn->c;
		z[sl+1] = '\0' ; 
		/* desendent check */
		if(cn->d == NULL) {
			printf("%s <-- ",z);
			if(cn->z!=NULL)cn->z->printRow();
		} else {
			printDFS( z , cn->d ) ; 		
		}
		/* sibling check */
		if(cn->s != NULL) {
			printDFS(s,cn->s);
		}
		delete z;
	}

	CharNode * add(const char * name , const std::vector<TabNode>  & ts_in) {
		CharNode * cn = mkPath(name); 
		if(cn->z!=NULL) {
			delete cn->z; 
		}
		cn->z = new TabHead(ts_in) ; 
		//cn->z->append(ts_in);
		return cn  ;
	}
	
	CharNode * add(const char * name , TabHead * th) {
		/* the tabHead is directly used */
		CharNode * cn = mkPath(name); 
		if(cn->z!=NULL) {
			delete cn->z; 
		}
		//cn->z = new TabHead(th) ; 
		cn->z = th ; 
		return cn  ;
	}

	~StrMap() { 
		//printf("here delete sm\n");
		del();
		r=NULL;
	}
protected:
	void del( CharNode  * cnn=NULL){
		CharNode * cn = (NULL==cnn)?(r):(cnn);	
		if(NULL == cn) return ; 
		/* desendent check */
		if(cn->d != NULL) {
			del(  cn->d ) ; 		
		}
		/* sibling check */
		if(cn->s != NULL) {
			del(cn->s);
		}
		if(NULL!=cn->z) delete cn->z;
		delete cn ; 
	}
} ; 



#endif
