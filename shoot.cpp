#include <stdio.h>
#include <unistd.h>
#include <string>
#include "tokenString.h"
#include "expand.h"
#include "macroni.h"
#include "shoot.h"
// global definition expaned tabList
StrMap stm ; 


// resolve the ~
std::string expandPath(const char * s ){
	std::string r="";
	char * homedir = getenv("HOME");
	if(homedir!=NULL) {
		for(int i =0 ; s[i]!='\0' ; i++) {
			if('~'==s[i]) r+=homedir;
			else r+=s[i] ; 
		}
	}
	else {
		r = s ; 
	}
	return r ;	
}

void init_ex() {
	/*initialize some symbols that might not be typabl*/
	stm.add(  "spc" 	 ,{ {0 , " "}}  ) ;
	//stm.add(  "mspc"  ,{ {0 , "\\ "}}  ) ;
	stm.add(  "rp" 	,{   {0 , "{"} , {1 , ""},{ 0 , "}"}  } ) ;
	stm.add(  "mrp" 	,{  {0 , "\\{"} , {1 , ""} ,{ 0 , "\\}"}  } ) ;
	stm.add(  "clmx" ,{ {0 , "! "}  } ) ;
	stm.add(  "alt" 	,{ {0 , "@ "}  } ) ;
	stm.add(  "void" ,{ {0,""}  });
	//stm.add(  "dfrac" ,{ {0,"\\dfrac"} , { 0, "{"} , {1 , ""},{0 , "}"} ,{ 0, "{"} , {2 , ""},{0 , "}"}  });
	stm.add(  "NL" ,{ {0,"\n"}   });
}



void printStack(TabHead * st) {
	TabHead * it = st ;
	while(it!=NULL) {
		it-> printRow() ; 
		it = it -> v ;
	}
}

extern "C" {
 int yyrestart(FILE * );
 int yylex();
}

void errExit(const char * fn , const int ln ,const char *wd,  const char * msg) {
	printf("%sF:%s L:%i W: %s M: %s %s\n" ,RED,fn,ln,wd,msg,RESET);
	exit(-1);
}

void warnPrint(const char * fn , const int ln ,const char *wd,  const char * msg) {
	printf("%sF:%s L:%i W: %s M: %s %s\n" ,YELLOW,fn,ln,wd,msg,RESET);
}


TabHead * popNoneVoidTop(TabHead** st ){
	//printf("%sPOPNONE %s\n", GREEN , RESET) ; 
	TabHead * top ; 
	while(NULL!=(*st) && NULL==(*st)->h) {
		TabHead * stt = (*st); 
		(*st) = (*st)->v;
		delete stt ; // clean out the voids
	}
	top =  (*st) ; 
	if(NULL != (*st)) { 
		(*st) = (*st)->v; 
		top->v = NULL ; 
	}	
	return top ; 
}

int abs(const int z ){
	return (z<0)?(-z):z ; 
}
int replacement(TabHead * tbn , TabHead ** stPOP){
	TabNode * it = tbn->h;
	TabNode * itt = NULL;
	while(it!=NULL) {
		if(it->k > 0){
			/* replacement */
			TabNode * reth ; TabNode * rete ;
			if(stPOP[abs((it->k) - 1)] == NULL) return  - (it->k) ; 
			stPOP[abs((it->k) - 1 )]->copy(&reth, &rete);
			if(NULL!=itt) itt->n = reth;
			else tbn->h = reth;
			rete->n = it->n ; 
			it->n =NULL ; 
			delete it; 
			itt = rete;
			it = rete->n;
		} else {
			/* Normal iteration */
			itt = it ; 
			it =it->n;
		}
	}
	return 0;
}

int m1Replacement(TabHead * tbn , TabHead * front){
	if(front == NULL) return -1 ; 
	TabNode * it = tbn->h;
	TabNode * itt = NULL;
	while(it!=NULL) {
		if(-1==it->k ){
			/* replacement */
			TabNode * reth ; TabNode * rete ;
			front->copy(&reth, &rete);
			
			if(NULL!=itt) itt->n = reth;
			else tbn->h = reth;
			rete->n = it->n ; 
			it->n =NULL ; 
			delete it; 
			itt = rete;
			it = rete->n;
		} else {
			/* Normal iteration */
			itt = it ; 
			it =it->n;
		}
	}
	return 0;
}


TabHead * serializeStack( TabHead * st , const int flag = 0 ){ 
	//printf("%sSTACK%s\n", GREEN , RESET);
	 /* and only return a TabHead
		* if flag is 0 do the #-1 replacement 
		* */	 

	/* Clear all voids and left over brackets */
	TabHead * it = st ; 
	TabHead * itt = NULL ; // itt is the non void front 
	while(it!=NULL  ) {
		if(	(it->h!=NULL) && (
				it->h->k == -100 - L_RP
				|| it->h->k == -100 - L_RB
				|| it->h->k == -100 - L_RC )
		) {
			printf("%s BRAKET IS LEFT OPEN  %s\n " , RED , RESET) ; 
			return NULL ; 
		} else if(it->h==NULL ) { 
			/* cleansing void */
			TabHead * itv = it->v ; 
			if( itt != NULL )   {
				itt->v = it->v ;  
				delete it ; 
				it = itt->v ; 
			}else {
				TabHead * tmp = it->v ; 
				delete it ; 
				it = tmp;
				st = tmp ; 
			}
		} else {
			/* Normal iteration once Nomral iterated itt wont be NULL */
			itt  = it  ; 
			it = it -> v ;
		}
	}
	 

	/*TODO need to add #-1 replacement for the bellow ones */
	if(0==flag) {
		//printf("%sREP%s\n", GREEN , RESET);
		//printStack(st);		
		it = st;  
		itt = NULL ; 
		TabHead * ittt = NULL ; 
		while(it!=NULL) {
			if(-1 ==  it->frontOp() && itt!=NULL) {
				/* try to do an front replacement  */		
				m1Replacement(it , itt) ; 		
				//printf("%sREProw%s\n", GREEN , RESET);
				//it->printRow();
				delete itt ; 
				if(NULL == ittt) {
					st = it ; 
				} else {
					ittt = ittt ; 
					ittt -> v = it ; 
				}
			} else {
				ittt = itt ; 
			}
			itt = it ; 
			it = it->v; 
		}
	}	

	//printf("%sREP ended%s\n" , RED , RESET) ; 
	//printStack(st);		
	//printf("%sptr ended%s\n" , RED , RESET) ; 
	/* serialize & destroy the stack */

	TabHead * ret = new TabHead();
	ret->h = st->h;
	it = st ; 
	itt = NULL ;  
	TabNode * e = NULL ;
	while(it!=NULL) {
		if(it->h!=NULL) {
			if(e!=NULL) e->n = it->h;
			e = it->getEnd() ; 
		}
		itt = it;
		it = it->v; 
		itt->h = NULL ; 
		itt->v = NULL ; 
		delete itt; 
	}
	//printf("ser ended\n") ; 
	return ret ;
}

TabHead *  nonVoidSerialize( TabHead  * st ,const int f = 0 ){
	/*seperator*/
	TabHead * it = st ; 
	TabHead * itt =NULL ; 
	while(  it!=NULL ) {
		if(it->h==NULL || 
			((it->h!=NULL) && (
				it->h->k == -100 - L_RP
				|| it->h->k == -100 - L_RB
				|| it->h->k == -100 - L_RC
			))
		) break ;  
		itt = it ; 
		it = it->v ;
	}
	if(itt!=NULL) { /*Yes there is a stack*/
		itt->v = NULL ; 
		st  =  serializeStack (st, f );
		if(st==NULL)return NULL;  
		st->v = it ; 
		TabHead * x = new TabHead() ; x->v = st; st =x ;
	} 						
	return st  ; 
}

std::string shoot( const char * fileName) {
	
	/* yylex related:	*/
	std::string fileNameSTR = expandPath(fileName) ; 
	FILE * f = fopen(fileNameSTR.c_str() , "r");
	if(!f){
		printf("%sshoot - file : %s is NULL%s\n" ,RED ,fileName,RESET) ;
		exit(-1);
	}
	yyrestart(f);
	initGV();
	yylex();
	fclose(f);
	TokenList TL;
	getGV(&TL);

	std::string lout = "" ; /* output latex */
	
	int stkid = 0 ;  
	int sent_len = TL.senp.size();
	for( int si = 0 ; si < sent_len ; si++ ) {
		/* This big fat for loop iterates all so called "sentences" */	
		int etkid  =  TL.senp[si] - 1  ; // this is token id index
		int itself = TL.senp[si] ; 	
		if(laclimax== TL.tokid[itself].t) {
			lout+=TL.getToken(itself);
			lout+="\n"; // ! climiax things are regarded as a seperate sentence
			stkid  =  itself + 1  ; // this is the start for the next sentence		 
			continue;
		}
		/*clean out the start of sentence starting VOID*/
		while(TL.tokid[stkid].t == L_VOID && stkid <= etkid)  { 
			stkid++;	
		}


		int sent_type = 0 ; // this recordes the sentence type 
		TokenID tid 	; 
		TabHead * st = NULL  ; /*the stack*/ 
		bool breakForFlag = false  ; 
		//printf("\n");

		/*create a flag to tell is this a definition or a rep*/

		int isDef = 1; 
		if(TL.tokid[stkid].t == L_DOL 
			||TL.tokid[stkid].t == L_DDOL  
			||TL.tokid[stkid].t == L_DALT  ) isDef  = 0 ; 
		for(int idx = etkid ;  (idx >= stkid)  && !breakForFlag ; idx--) {
			/* this big fat for loop iterates though all tokens in a sentence from rear to front 
			 * but sub structure in it is just a "switch case"*/	

			std::string tn = TL.getToken(idx);
			tid 	 = TL.tokid[idx];
			//printf("%s%s%s\n",GREEN,tn.c_str()  , RESET) ; 
			switch(tid.t) {
				 case(L_ID): {
					CharNode * cn = stm.fdPath(tn.c_str());
					// printf("%sCHarNode 0x%x%s\n",GREEN,cn , RESET) ; 
					if(cn!=NULL && cn->z!=NULL && idx != stkid) { /* a call to replacement */
						/* copy the expand tab */
						TabHead * tbn = new TabHead(cn->z); 
						if(NULL==tbn) errExit(fileName,tid.ln,tn.c_str(),"expand tab found empty");

						int sN = tbn->opCNT();
						//printf("sN : %i\n",sN);
						if(sN == 0 ) {
							/* just pile up if there is no need arguments*/	
							tbn->v = st;
							st = tbn;
							break ;  
						}
						TabHead * stPOP[sN];
						/* pop the valid arguments from stack */
						int i = 0 ; 
						while(i < sN ) {
							TabHead * top =  popNoneVoidTop( &st ) ;
							if(NULL == top ) errExit(fileName,tid.ln,tn.c_str(),"stack empty");
							stPOP[i] = top ; 
							i++;
						}

						int retCode = replacement(tbn , stPOP);
						if(retCode < 0 ) errExit(fileName,tid.ln,tn.c_str(),"got Null stPOP");

						/* deleteing the old arguments */	
						for(int  i =0 ; i < sN ; i++){
							delete stPOP[i]; // they are pointers	
						}
						
						/* push the tbn as the header*/
						tbn->v = st;
						st = tbn;

					} else if(idx==stkid){ /* a call to defintion the ID in stm*/
						//printf("a call to defintion the ID in stm \n") ;
						if(NULL!=cn && NULL!=cn->z) {
							warnPrint(fileName,tid.ln , tn.c_str()," : is going to be redefined") ; 
						}
						TabHead * th = serializeStack(st , 1 );
						if(th == NULL ) errExit(fileName,tid.ln,tn.c_str(),"stack empty for definition");
						stm.add(tn.c_str(), th ); 
						st = NULL ; 
						//printf("END OF a call to defintion the ID in stm \n") ;
					} else { /* the ID is not defined */
						std::string t = "[\\text{Escaped}\\ "+tn+"]" ; 
						//printf("%s\n" , t.c_str()); 
						TabHead *x =  new TabHead ({{0, t}}); 	
						x->v = st; st = x;
					}
					break;
				} case(L_INT) :  {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_PAR) : {
					//printf("%sPAR %i%s\n" , RED,std::stoi(tn),RESET);
					int num = std::stoi(tn) ; 
					if(num < -1 || num > 9 ) errExit(fileName,tid.ln,tn.c_str(),"extreme par");
					TabHead * x = new TabHead({{num, " PAR "}});
					x->v = st; st = x ; 
					break;
				} case(L_FLOAT) :  {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_COLM 		) : { 
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_SEMI 		) : { 
						printf("%sFile : %s L: %i, There should not be semi here%s\n",
							RED , fileName, tid.ln,RESET
						);
						exit(-1);
					break;
				} case(L_COMMA 		) : {  
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_ASSIGNOP	) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_RELOP 		) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_PLUS 		) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_MINUS 		) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_STAR 		) : {
					TabHead * x = new TabHead({{0,"\\cdot "}});
					x->v = st; st = x ; 
					break;
				} case(L_DSTAR 		) : {
					TabHead * x = new TabHead({{0,"\\times "}});
					x->v = st; st = x ; 
					break;
				} case(L_SUP 			) : {
					/*TODO must functionize them here*/
					//TabHead * x = new TabHead({{-100 - L_SUP,""}});
					//x->v = st; st = x ; 
					break;
				} case(L_SUB 			) : {
					///*TODO must functionize them here*/
					//if(NULL == st ) errExit(fileName,tid.ln,tn.c_str(),"stack empty at L_SUB");
					//TabHead * x = new TabHead({ {-1,""},{0,"_{"}}) ; 
					////printf("%sL_SUB%s\n", GREEN , RESET);
					//TabHead * top =  popNoneVoidTop( &st) ; 
					//if(NULL == top ) errExit(fileName,tid.ln,tn.c_str(),"stack empty at L_SUB");
					//(x->getEnd())->n = top->h ; 
					//(top->getEnd())->n = new TabNode(0,"}");
					//top->h = NULL ; delete top ; 
					//x->v = st ; st = x ;	
					break;
				} case(L_DIV 			) : {
					/*TODO must functionize them here*/
					//TabHead * x = new TabHead({{-100 - L_DIV,""}});
					//x->v = st; st = x ; 
					break;
				} case(L_DDIV 			) : {
					/*TODO must functionize them here*/
					//TabHead * x = new TabHead({{-100 - L_DDIV,""}});
					//x->v = st; st = x ; 
					break;
				} case(L_ALG 			) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_BAR 			) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_DOT 			) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_ALT 			) : {
					// here need to clean the stack rs up for the output
					// same for the DALT
					break;
				} case(L_DALT 		) : {
					breakForFlag = true;
					if(st==NULL) break ; 	
					TabHead * disp =  serializeStack(st,isDef);
					if(disp==NULL) errExit(fileName,tid.ln,tn.c_str(),"disp got NULL");
					lout+="(L:"+std::to_string(tid.ln) +")\n";
					lout+="\\begin{equation}\n";
					lout+="\\begin{split}\n";
					lout+= disp->genStr();
					lout+="\\end{split}\n";
					lout+="\\end{equation}\n";
					delete disp ;
					break;
				} /*case(L_NUM 			) : {
					// do nothing
					break;
				} case(L_DNUM 		) : {
					break;
				} case(L_WHT 			) : {
					rs.pullH({{0 , tn }});
					break;
				} */case(L_BSL 			) : {
					TabHead * x = new TabHead({{0,"\\\\\n"}});
					x->v = st; st = x ; 
					break;
				} case(L_PLA 			) : {
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_DOL 			) : {
					breakForFlag = true;
					if(st==NULL) break ; 	
					TabHead * disp =  serializeStack(st,isDef);
					if(disp==NULL) errExit(fileName,tid.ln,tn.c_str(),"disp ss got NULL");
					lout+=" $ ";
					lout+= disp->genStr();
					lout+=" $ ";
					delete disp ;
					break;
				} case(L_DDOL 		) : {
					breakForFlag = true;
					if(st==NULL) break ; 	
					TabHead * disp =  serializeStack(st,isDef);
					if(disp==NULL) errExit(fileName,tid.ln,tn.c_str(),"disp ss got NULL");
					lout+="(L:"+std::to_string(tid.ln) +")\n";
					lout+="\\begin{equation}\n";
					lout+= disp->genStr();
					lout+="\\end{equation}\n";
					delete disp ;
					break;
				} case(L_LP 			) : {
					TabHead * it = st ; 
					while(  it!=NULL ) {
						if(it->h!=NULL && it->h->k ==(-100 - L_RP)) break ;  
						if(it->h!=NULL && (it->h->k ==(-100 - L_RC) || it->h->k ==(-100 - L_RB))) 
							errExit(fileName,tid.ln,tn.c_str()," is crossed");
						it=it->v ;
					}
					if(NULL == it ) errExit(fileName,tid.ln,tn.c_str()," is not closed");
					TabHead * stt = st ; 
					st = it->v ; 
					it->h->k=0;
					it->h->s=")";
					it->v =NULL ; 
					TabHead * stx = serializeStack(stt,isDef);
					if(stx==NULL) errExit(fileName,tid.ln,tn.c_str(),"LP stack serialize got NULL");
					TabNode * stxr = new TabNode(0,"(");
					stxr->n = stx->h;
					stx->h = stxr;
					stx -> v = st;
					st = stx ; 
					break;
				} case(L_RP 			) : {
					TabHead * x = new TabHead({{-100 - L_RP,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_LB 			) : {
					TabHead * it = st ; 
					while(  it!=NULL ) {
						if(it->h!=NULL && it->h->k ==(-100 - L_RB)) break ;  
						if(it->h!=NULL && (it->h->k ==(-100 - L_RC) || it->h->k ==(-100 - L_RP))) 
							errExit(fileName,tid.ln,tn.c_str()," is crossed");
						it=it->v ;
					}
					if(NULL == it ) errExit(fileName,tid.ln,tn.c_str()," is not closed");
					TabHead * stt = st ; 
					st = it->v ; 
					it->h->k=0;
					it->h->s="]";
					it->v =NULL ; 
					TabHead * stx = serializeStack(stt,isDef);
					if(stx==NULL) errExit(fileName,tid.ln,tn.c_str(),"LB stack serialize got NULL");
					TabNode * stxr = new TabNode(0,"[");
					stxr->n = stx->h;
					stx->h = stxr;
					stx -> v =st;
					st = stx;
					break;
				} case(L_RB 			) : {
					TabHead * x = new TabHead({{-100 - L_RB,""}});
					x->v = st; st = x ; 
					break;
				} case(L_LC 			) : {
					TabHead * it = st ; 
					while(  it!=NULL ) {
						if(it->h!=NULL && it->h->k ==(-100 - L_RC)) break ;  
						if(it->h!=NULL && (it->h->k ==(-100 - L_RP) || it->h->k ==(-100 - L_RB))) 
							errExit(fileName,tid.ln,tn.c_str()," is crossed");
						it=it->v ;
					}
					if(NULL == it ) errExit(fileName,tid.ln,tn.c_str()," is not closed");
					TabHead * stt = st ; 
					st = it->v ; 
					it->h->k=0;
					it->h->s="";
					it->v =NULL ; 
					TabHead * stx = serializeStack(stt,isDef);
					if(stx==NULL) errExit(fileName,tid.ln,tn.c_str(),"LC ss got NULL");
					stx -> v = st;
					st = stx ; 
					break;
				} case(L_RC 			) : { 
					TabHead * x = new TabHead({{-100 - L_RC,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_CHR 			) : { 
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_STR 			) : { 
					TabHead * x = new TabHead({{0,tn}});
					x->v = st; st = x ; 
					break;
				} case(L_INC 			) : { 
					printf("\n%s L_INC %s %s\n" , GREEN , tn.c_str(), RESET);
					lout+=shoot(tn.c_str());
					break;
				} case(L_VOID 	) : { 
					//printf("void\n");
				 	st = nonVoidSerialize(st,isDef) ; 
					break;
				}
				default : {
					printf("%s Error there is no such token type %s\n",RED,RESET);
					break;
				}
			} // end of switch
		} // end of one sentence
		stkid  =  TL.senp[si] + 1  ; /*iterate for the sentence */		 
	} // end of all the sentences

	return lout;
}


