#include "extension.h"

int main(int argc, char **argv)
{
   Prog p;
   p.c=0;
   checkCount(argc);
   p.fp=openFile(argv[1]);
   main_(&p);
   getWordCount(&p);
   detectRep(&p);
   redundancyCheck(&p);
   freeProg(&p);
   return 0;
}


void checkCount(int c)
{
   if(c<NUMALLOWEDARGS){
      ERROR("Please type in a filename!\n");
   }
   else if(c>NUMALLOWEDARGS){
      ERROR("Please type in only 1 filename!\n");
   }
}


FILE *openFile(char *filename)
{
   FILE *fp;
   fp=fopen(filename,"r");
   if(fp==NULL){
      ERROR("Failed to open file.\n");
   }
   return fp;
}


void getWordCount(Prog* p)
{
   fseek(p->fp,0,SEEK_SET);
   while(READNEXT!=EOF){
      p->c++;
   }
}


/* This function detects repetition by going through the input program, 
looking for repetition and consequently rewriting the program until it
cannot be condensed any further*/
void detectRep(Prog* p)
{
   Bool running=TRUE, reprint=FALSE;
   readIn(p); /* Read in all the arguments into a dynamic array */
   while(running){
      scanRep(p); /* Scan the sets of strings for repetition */
      if(p->loop.rep>=MINLOOPS){
         reWrite(p); /* Rewrite the program in a condensed way */
         updateProg(p);
         reprint=TRUE;
      }
      else{
         running=FALSE;
      }
   }
   if(reprint){
      printOptiProg(p); /* Print the new prog */
   }
   else{
      printf("No working loops detected in your code. Well done!\n");
   }
}


/* This function rewrites the program in a condensed way if possible */
void updateProg(Prog* p)
{
   int i;
   p->prog=(char**)realloc(p->prog,p->oc*sizeof(char*));
   if(p->prog==NULL){
      ERROR("Dynamic memory allocation failed");
   }
   for(i=0;i<p->oc;i++){
      p->prog[i]=(char*)malloc((strlen(p->optiprog[i])+1)*sizeof(char));
      if(p->prog[i]==NULL){
         ERROR("Dynamic memory allocation failed");
      }
      strcpy(p->prog[i],p->optiprog[i]);
   }
   p->c=p->oc;
}


void readIn(Prog* p)
{
   int wordcount=0,i;
   fseek(p->fp,0,SEEK_SET);
   while(READNEXT!=EOF){
      wordcount++;
   }
   p->prog=(char**)malloc(wordcount*sizeof(char*));
   fseek(p->fp,0,SEEK_SET);
   for(i=0;i<wordcount;i++){
      READNEXT;
      p->prog[i]=(char*)malloc((strlen(p->cw)+1)*sizeof(char));
      strcpy(p->prog[i],p->cw);
   }
}


void scanRep(Prog* p)
{
   int i,j;
   p->loop.rep=0;
   for(i=MINLOOPSIZE;i<p->c/2;i++){ /* length wordlist */
      for(j=0;j<p->c-i;j++){ /* start index wordlist */
         if(grammarCheck(p,j)){
            scanFrame(p,j,i);
         }
      }
   }
}


/* Counts amount of repetition */
void scanFrame(Prog* p, int start, int length)
{
   int n,offset,same,rep=1;
   for(offset=start+length;offset<p->c-length;offset=offset+length){ /* pos==offset */
      same=0;
      for(n=0;n<length;n++){ /* check for all prog */
         if(offset+n<p->c){
            if(strsame(p->prog[start+n],p->prog[offset+n])){
               same++;
            }
            else{ /* stop this iteration */
               offset=p->c-length;
               n=length;
            }
         }
      }
      if(same==length){
         rep++;
      }
   }
   if(rep>p->loop.rep){
      storeLoop(p,start,length,rep);	
   }
}


Bool grammarCheck(Prog* p, int start)
{
   if(isNum(p->prog[start])){
      return FALSE;
   }
   return TRUE;
}


void storeLoop(Prog* p, int start, int length, int rep)
{
   p->loop.start=start;
   p->loop.length=length;
   p->loop.rep=rep;
}


void reWrite(Prog* p)
{
   int i=0,j;
   char endloop[BUFF];
   p->oc=p->c-p->loop.length*p->loop.rep+LOOPARGS+p->loop.length;
   p->optiprog=(char**)malloc(p->oc*sizeof(char*));
   for(j=0;j<p->loop.start;j++){
      insert(p,p->prog[j],i++);
   }
   insert(p,"DO",i++);
   insert(p,"Z",i++);
   insert(p,"FROM",i++);
   insert(p,"1",i++);
   insert(p,"TO",i++);
   sprintf(endloop,"%d",p->loop.rep);
   insert(p,endloop,i++);
   insert(p,"{",i++);
   for(j=p->loop.start; j< p->loop.start+p->loop.length ;j++){
      insert(p,p->prog[j],i++);
   }
   insert(p,"}",i++);
   j+=p->loop.length*(p->loop.rep-1);
   while(j<p->c){
      insert(p,p->prog[j],i++);
      j++;
   }
}


void insert(Prog* p, char* string, int i)
{
   p->optiprog[i]=(char*)malloc((strlen(string)+1)*sizeof(char));
   if(p->optiprog[i]==NULL){
      ERROR("Memory allocation failed");
   }
   strcpy(p->optiprog[i],string);
}


void printOptiProg(Prog* p)
{
   int i,c=0,newline=0;
   printf("Yo! You can also write your code like this:\n\n");
   for(i=0;i<p->oc;i++){
      if(strsame(p->optiprog[i],"{")){
         printf("{");
         c++;
         newline=1;
      }
      else if(strsame(p->optiprog[i],"}")){
         c--;
         printf("\n%*s}",c*INDENT," ");
         newline=1;
      }
      else{
         if(strsame(p->optiprog[i],"DO")){
            newline=1;
         }
         if(newline){
            printf("\n%*s%s ",c*INDENT," ",p->optiprog[i]);
            newline=0;
         }
         else{
            printf("%s ",p->optiprog[i]);
         }
      }
   }
   printf("\n\n");
}


Bool nextLine(Prog* p, int i)
{
   if((strsame(p->optiprog[i],"FD") ||
   strsame(p->optiprog[i],"RT") ||
   strsame(p->optiprog[i],"LT")) &&
   isNum(p->optiprog[i+1])){
      return TRUE;
   }
   return FALSE;
}


void freeProg(Prog* p)
{
   int i;
   for(i=0;i<p->oc;i++){
      free(p->prog[i]);
   }
   free(p->prog);
   for(i=0;i<p->c;i++){
      free(p->optiprog[i]);
   }
   free(p->optiprog);
}


void main_(Prog* p)
{
	if(!(strSame(p,0,"{"))){
		ERROR("No begin statement?\n");
	}
	instruc(p);
}


void instruc(Prog* p)
{
	if(strSame(p,0,"}")){ /* Base-case */
		return;
	}
	else if(strSame(p,strlen(p->cw),"FD") || strSame(p,strlen(p->cw),"RT") || strSame(p,strlen(p->cw),"LT")){
		READNEXT;
		if(!(isVarNum(p->cw))){
   		ERROR("Expected a (set) Var or a Num!\n");
   	}
	}
	else if(strSame(p,strlen(p->cw),"DO")){
		loop(p);
	}
	else if(strSame(p,strlen(p->cw),"SET")){
		set(p);
	}	
	else{
      ERROR("Expected a statement or a closing bracket!\n");
	}
	instruc(p);
}


void loop(Prog* p)
{
	if(READNEXT && !isVar(p->cw)){
		ERROR("Expected a VAR of 1 uppercase char after the DO!\n");
	}
	if(!strSame(p,0,"FROM")){
		ERROR("You missed the FROM in the do loop after the variable\n");
	}
	if(READNEXT && !isVarNum(p->cw)){ 
		ERROR("You missed the VARNUM in the do loop after the FROM\n");
	}
	if(!strSame(p,0,"TO")){
		ERROR("You missed the TO in the do loop after the variable\n");
	}
	if(READNEXT && !isVarNum(p->cw)){
		ERROR("You missed the VARNUM in the do loop after the TO\n");
	}
	main_(p);
}


void set(Prog* p)
{
	if(READNEXT && !isVar(p->cw)){
		ERROR("Expected a VAR after the set!\n");
	}
	if(!strSame(p,0,":=")){
		ERROR("Expected a := after the VAR!\n");
	}
	rpn(p);
}


void rpn(Prog* p)
{
	if(strSame(p,0,";")){ /* Base-case */
	}
	else if(isVarNum(p->cw) || isOp(p->cw[0])){
		rpn(p);
	}
	else{
		ERROR("Argument is invalid.\n");
	}
}


Bool strSame(Prog* p, int offset, char* test)
{
	fseek(p->fp, -offset, SEEK_CUR);
	READNEXT;
	if(strcmp(p->cw,test)==0){
		return TRUE;
	}
	return FALSE;
}


Bool isOp(char a)
{
	if(a=='+'|| a=='-' || a=='*' || a=='/'){
		return TRUE;
	}
	return FALSE;
}


Bool isVarNum(char* var)
{
	if(isNum(var)){
		return TRUE;
	}
	else if(isVar(var)){
		return TRUE;
	}
	return FALSE;
}


Bool isVar(char* var)
{
	if(isupper(var[0]) && isalpha(var[0]) && var[1]=='\0'){
		return TRUE;
	}
	return FALSE;
}


/* This function is inspired on this code:
https://rosettacode.org/wiki/Determine_if_a_string_is_numeric */
Bool isNum(char* var)
{
   char* p;
   if(var== NULL|| *var=='\0' || isspace(*var)){
      return FALSE;
   }
   strtod(var,&p);
   return *p=='\0';
}


/* This func checks for statements outside the brackets.
It is unclear how to deal with this in the
formal grammar, but I think it is a useful addition. */
void redundancyCheck(Prog* p)
{
	if(READNEXT!=EOF){
		printf("There are statements outside the brackets.\n");
	}
}
