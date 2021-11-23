#ifndef __EXPAND_H__
#define __EXPAND_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<string>
#include "macroni.h"


struct TabNode {
public : 
	int n ; 
	std::string tx;
	TabNode * h;
	TabNode * v;
	TabNode() {
		h=NULL;
		v=NULL;
	}

	TabNode(const TabNode & tn) 
		:n(tn.n),tx(tn.tx) {
		h=NULL;
		v=NULL;
	}

	TabNode(const int n_in , const std::string & tx_in)
		: n(n_in) , tx(tx_in){
		h=NULL;
		v=NULL;
	}

	int getROpN(TabNode * st = NULL){
		TabNode * it = (NULL ==st)?(this):(st);
		int maxInt=0;
		while(it != NULL) {
			if(it->v!=NULL) {
				int des = getROpN(it->v);
				maxInt=((des > maxInt)?(des):(maxInt));
			}
			maxInt=(( it->n> maxInt)?(it->n):(maxInt));
			it = it->h;
		}
		return maxInt;
	}

	~TabNode(){
		//printf("free: %s \n" , tx.c_str());
	}

		
} ; 


class TabList {
public:
	/*	since it works in a stack module
	 *	it is going to be great if it grows the stack by the th as the
	 *	top of the stack
	 * */
	TabNode * th ;
	TabNode * tve ; // vertical end fot the th	
	TabList() {
		th = NULL ; 
		tve = NULL ; 
	}

	TabList( const TabList & tl) {
		/*
		 * Here need to copy all of them to make it work
		 * */
		th = copy(tl.th) ; 
		tve = getVEnd(); 
			
	}

	TabList(const std::vector<TabNode>  & ts_in){
		th = NULL ; 
		tve = NULL ; 
		pullH(ts_in);	
	}


	void pullH(const std::vector<TabNode>  & ts_in){
		/*
		 * This function mainly just pull up the stack 
		 */
		TabNode * tmph = NULL;
		TabNode * tmpe = NULL;
		for(auto it=ts_in.begin() ; it!=ts_in.end() ; it++) {
			if(NULL == tmph) {
				tmpe = new TabNode(it->n , it->tx);
				tmph = tmpe ;
			} else {
				tmpe->h = new TabNode(it->n , it->tx);
				tmpe = tmpe->h;
			}
		}
		if(NULL != tmph) {
			if(NULL !=th ){ 
				tmph->v = th;
				//printf("th is not NULL\n");
			}
			else { 
				tve = tmph;
				//printf("th is NULL\n");
			}
			th = tmph;
		}
	}

	// this is only used in the stack mode
	// this need to be remebered to delete
	void topH (TabNode * t){
		// this t is added to the top of the th without copy
		if(NULL==t) return ;
		t->v = th;
		if(NULL==th) tve = t ;
		th = t;
	}

	TabNode* popH(){
		if(NULL==th) return NULL ; 
		TabNode * tbl = th;
		if(NULL==th->v) tve = NULL;
		th = th->v;
		tbl->v = NULL;
		return tbl;
	}

	void appendH(const int n_in , const std::string & tx_in){
			//printf("pushing it\n");
			TabNode * the = getHEnd();
			//printf("\tget the  hEnd\n");
			if(NULL == the) {
				th = new TabNode(n_in,tx_in);
				tve = th;
			}else {
				the->h = new TabNode(n_in,tx_in);
			}
	}

	TabNode * getHEnd(TabNode * st = NULL){
		TabNode * it = (NULL ==st)?(th):(st);
		while(it!=NULL && it->h!=NULL) it = it->h  ;
		return it;
	}

	TabNode * getVEnd(TabNode * st = NULL){
		TabNode * it = (NULL ==st)?(th):(st);
		while(it!=NULL && it->v!=NULL) it = it->v  ;
		return it;
	}

	void printAll( TabNode * st = NULL) {
		TabNode * it = (NULL ==st)?(th):(st);
		//printf("F:%s L:%i N:%s\n",fileName.c_str() , lineno , tname.c_str());	
		while(it != NULL) {
			//the node wich is a down spawn is not printed
			if(it->v!=NULL) {
				printAll(it->v);
			} 
			if(0==it->n) {
				printf("%s\n" , it->tx.c_str());
			}else{
				printf("#%i\n" , it->n);
			}
			it = it->h;
		}
	}

	void printARow( TabNode * st = NULL) {
		TabNode * it = (NULL ==st)?(th):(st);
		//printf("F:%s L:%i N:%s\n",fileName.c_str() , lineno , tname.c_str());	
		while(it != NULL) {
			//the node wich is a down spawn is not printed
			if(it->v!=NULL) {
				printAll(it->v);
			} 
			if(0==it->n) {
				printf("%s " , it->tx.c_str());
			}else{
				printf("#%i " , it->n);
			}
			it = it->h;
		}
		printf("\n");
	}
	// this is used in the initalizer;
	TabNode * copy( TabNode * t = NULL) { // This is recursive ; 
		TabNode * cph = NULL ; // the final header 
		TabNode * cpt = NULL ; //the copy iterator 
		TabNode * it = (NULL==t)?(th):(t) ; 
		while(it != NULL) {
		
			// first copy horizontally :
			if(cph==NULL) {
				cph = new TabNode(*it);
				cpt = cph ;
			} else {
				cpt->h = new TabNode(*it);
			}
			// vertical go
			if(it->v!=NULL) {
				cpt->v = copy(it->v);
			} 

			// move cpt if there is new added
			if(cpt->h!=NULL)cpt = cpt->h;
			
			// horizontally iterate
			it = it->h;
		}
		return cph; // for the end could use getVEnd()  to decide it;
	}

	std::string cat(){
		std::string s="";

		TabNode * it = th ;
		while(it!=NULL) {
			if(0!=it->n) {
				s+=" \\#";
				s+=std::to_string(it->n);
				s+=" ";
			} else { 
				s+=it->tx;
			}
			it = it->h;
		}
		return s ; 

	}

	~TabList(){
		//printf("Here deletes Tablist\n");
		del();
	}
private:

	void del( TabNode * st = NULL) {
		TabNode * ith = (NULL ==st)?(th):(st);
		TabNode * ithh = NULL ; 
		//printf("F:%s L:%i N:%s\n",fileName.c_str() , lineno , tname.c_str());	
		while(ith != NULL) {
			if(ith->v!=NULL) {
				del(ith->v);
			}
			ithh = ith->h;
			delete ith;
			ith =ithh;
		}
	}
};


class Expd {
public:
	std::string fileName;
	std::string name;
	int lineno;
	TabList tl ; 

	Expd() {
	
	}
	// this is a really normal copy in action
	Expd( const Expd & E): fileName(E.fileName) , name(E.name) , lineno(E.lineno) ,tl(E.tl) {
		
	}
	Expd(std::string const & fileName_in , std::string const & name_in 
		,const int lineno_in , const std::vector<TabNode>  & ts_in )
		: fileName(fileName_in) , name(name_in) , lineno(lineno_in) ,tl(ts_in){
		// this is OK initialized
	}

	Expd(std::string const & fileName_in , std::string const & name_in 
		,const int lineno_in , const TabList  & ts_in )
		: fileName(fileName_in) , name(name_in) , lineno(lineno_in) ,tl(ts_in){
		// this is OK initialized
	}
	~Expd() {
	}
};



class ExpdList{
/* This works as an
 * manager to manage all the look up table ,
 * it is legit only one per shoot.
 * it is important
 * */
public:
	std::vector<Expd> L ;
	ExpdList() {
	}
	
	int is(const std::string & name) {
		const char * name_c = name.c_str();
		int idx = -1 ;
		for(auto it = L.begin() ; it!=L.end();it++) {
			idx++;
			if(0==strcmp(name_c , it->name.c_str())) return idx; 
		}
		return -1;
	}

	// This might run little faster somehow
	// since new defined are more welcome in general
/*	int isFromBack(const std::string & name) {
		const char * name_c = name.c_str();
		int idx = L.size() ;
		for(auto it = L.end() ; it>=L.begin();it--) {
			idx--;
			if(0==strcmp(name_c , it->name.c_str())) break; 
		}
		return (idx==L.size())?(-1):(idx);
	}
*/
	void erase(const int idx) {
		//L.erase(L.begin() + idx ) ;
		L[idx].name = "%%"+L[idx].name;
	}

	void listAll() {
		for(auto it = L.begin() ; it!=L.end();it++) {
			printf("F : %s N : %s L : %i  <-- " , it->fileName.c_str() ,
				 it->name.c_str(),it->lineno );
			it->tl.printARow(); 
		}
	}

	void printToFile(const char * fn="log.txt") {
		FILE * f = fopen(fn , "w");
		if(NULL ==f ) {
			printf("%s expd - the file %s cant be opened to log%s" , RED , fn , RESET);
			exit(-1);
		}
		for(auto it = L.begin() ; it!=L.end() ; it++ ) {
			fprintf(f, "%s <-- %s\n", it->name.c_str() , it->tl.cat().c_str()) ; 
		}
		fclose(f);		
	}
	
	~ExpdList(){
	}

};


class CharNode {
public:
	char c ; // the char
	CharNode*s; // sibling
	CharNode*d; // child or decendent
	TabList * z ;  // valued part :  
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

	//CharNode * add(const TabList){
	//}

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
			printf("%s\n",z);
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
		cn->z = new TabList(ts_in) ; 
		return cn  ;
	}
	
	CharNode * add(const char * name , const TabList  & tl) {
		CharNode * cn = mkPath(name); 
		if(cn->z!=NULL) {
			delete cn->z; 
		}
		cn->z = new TabList(tl) ; 
		return cn  ;
	}

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
	~StrMap() { 
		// better if this can be deleted in dfs mode : 
		//printf("here delete sm\n");
		del();
		r=NULL;
	}
} ; 





#endif
