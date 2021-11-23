#include <stdio.h>
#include "tokenString.h"
#include "expand.h"
#include "shoot.h"

extern StrMap stm; 


int main(int argc , char ** argv) {
	if(argc!=3 ) { perror(" please first give an input then an output file \n"); return 1 ;} 
	init_ex();
	FILE * fo = fopen(argv[2] , "w") ;
	if(!fo){ perror("main - out file is NULL") ; return 1;}
	fprintf(fo , "%s" , shoot(argv[1]).c_str());
	fclose(fo);

	//stm.printDFS("");
	//printf("%sFinish%s\n",GREEN,RESET);

	return 0 ;
	

}
