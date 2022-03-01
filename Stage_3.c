#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*STRUCTURE DEFINITION: temporary list*/
struct charlist{
  char c;
  struct charlist *next;
};

/*STRUCTUTE DEFINITION: the base list*/
struct baselist{
  struct baselist *next;
  char *word;
};

/*CONDITION FUNCTION: spacebar(quotation) exit status checker*/
int spbex(char chr,int qm_marker)
{
  return ((chr != ' ')||(qm_marker % 2  != 0));
}

/*CONDITION FUNCTION: amp(quotation) exit status checker*/
int ampex(char chr,int qm_marker)
{
  return ((chr != '&')||(qm_marker % 2  != 0));
}

/*CONDITION FUNCTION: EOL exit status checker*/
int eolex(char chr)
{
  return (chr != '\n');
}

/*CONDITION FUNCTION: EOF exit status checker*/
int eofex(char chr)
{
  return (chr != EOF);
}

/*CONDITION FUNCTION: Composition of previously declared functions*/
int ex_cond(char c,int q)
{
  return (spbex(c,q)&&ampex(c,q)&&eolex(c)&&eofex(c));
}

/*FUNCTION: comparing 2 strings*/
int str_comp(char* str1, char* str2)
{
  while((*str1 != '\0')&&(*str2 != '\0')){
    if( *str1 != *str2  ){
      return 0;
    }
    str1++;
    str2++;
  }
  return *str1 == *str2;
}

/*FUNCTION: temporary list deletion*/
void del_charlist(struct charlist *chr)
{
  struct charlist *tmp;
  while(chr){
    tmp=chr->next;
    free(chr);
    chr=tmp;
  }
};

/*FUNCTION: deleting base array*/
void del_barray(char** barr,int len)
{
  int i;
  for(i=0;i<len;i++)
  {
    free(barr[i]);
  }
  free(barr);
}

/*FUNCTION: deleting only baselist segments and keeping  words unfreed*/
void partial_del_baselist(struct baselist *bas)
{
  struct baselist *tmp;
  while(bas){
    tmp=bas->next;
    free(bas);
    bas=tmp;
  }
  bas=NULL;
}

/*FUNCTION: full base list and words deletion (in case of QM ERROR)*/
void full_del_baselist(struct baselist *bas)
{
  struct baselist *tmp;
  while(bas){
    tmp=bas->next;
    free(bas->word);
    free(bas);
    bas=tmp;
  }
  bas=NULL;
}

/*FUNCTION: skipping after ampersand error*/
void skip()
{
  char c='\0';
  while(c != '\n'){
    c=getchar();
  }
}

/*FUNCTION: list size*/
int list_size(struct charlist *chr)
{
  int i=0;
  struct charlist *tmp;
  tmp=chr;
  while(tmp){
    i++;
    tmp=tmp->next;
  }
  return i;
}

/*FUNCTION: list size*/
int base_list_size(struct baselist *chr)
{
  int i=0;
  struct baselist *tmp;
  tmp=chr;
  while(tmp){
    i++;
    tmp=tmp->next;
  }
  return i;
}

/*FUNCTION: list to array*/
char* list_to_arr(struct charlist *chr)
{
  int i,ls=list_size(chr);
  char *a;
  struct charlist *tmp;
  ls++;
  tmp=chr;
  a=malloc(ls);
  for(i=0; i<ls-1; i++){
    a[i]=tmp->c;
    tmp=tmp->next;
  }
  a[i]='\0';
  del_charlist(chr);
  return a;
}

/*FUNCTION: Adding block to temporary list*/
struct charlist* add_block_temp(struct charlist *fel,char chr)
{
  struct charlist *tmp=NULL;
  tmp=fel;
  if (tmp == NULL){
    tmp=malloc(sizeof(struct charlist));
    tmp->c=chr;
    tmp->next=NULL;
    return tmp;
  }else
  {
    while (tmp->next != NULL){
      tmp=tmp->next;
    }
    tmp->next=malloc(sizeof(struct charlist));
    tmp->next->c=chr;
    tmp->next->next=NULL;
    return fel;
  }
}

/*FUNCTION: Adding block to temporary list*/
struct baselist* add_block_base(struct baselist *fel,char *a)
{
  struct baselist *tmp=NULL;
  tmp=fel;
  if (tmp == NULL){
    tmp=malloc(sizeof(struct baselist));
    tmp->word=a;
    tmp->next=NULL;
    return tmp;
  }else
  {
    while (tmp->next != NULL){
      tmp=tmp->next;
    }
    tmp->next=malloc(sizeof(struct baselist));
    tmp->next->word=a;
    tmp->next->next=NULL;
    return fel;
  }
}


/*FUNCTION: Building the temporary list*/
struct charlist* build_list(char *last_char,int *amp_flag)
{
  char chr;
  struct charlist *fel=NULL;
  int qm_marker=0;
  do
  {
    chr=getchar();
    if(chr == EOF){
      *last_char=EOF;
      return NULL;
    }
  } while(chr == ' ');
  if((*amp_flag == 1)&&(chr != '\n')){
    del_charlist(fel);
    *last_char='&';
    fel=NULL;
    return fel;
  }
  while(ex_cond(chr, qm_marker)){
    if(chr == '"'){
      qm_marker++;
      chr=getchar();
    }else{
      fel=add_block_temp(fel, chr);
      chr=getchar();
    }
  }
  if(qm_marker % 2 == 1){
    del_charlist(fel);
    *last_char = '"' ;
    fel=NULL;
    return fel;
  }else{
    if (chr == '&'){
      *last_char = ' ';
      *amp_flag = 1;
    }else{
      *last_char=chr;
    }
    return fel;
  }
}

/*FUNCTION: base list to base array*/
char** blist_to_barray(struct baselist *fel,int* arr_len)
{
  char **barr;
  struct baselist* tmp=fel;
  int len=base_list_size(fel),i=0;
  barr=malloc(sizeof(char*)*(len+1));
  for(i=0;i<len;i++)
  {
    barr[i]=tmp->word;
    tmp=tmp->next;
  }
  barr[i]=NULL;
  *arr_len=len;
  partial_del_baselist(fel);
  return barr;
}

/*FUNCTION: executing in terminal*/
void execute(char** barr, int amp_flag)
{
  int t;
  if( barr[0] == NULL){
  return;
  }
  if ( str_comp(barr[0], "cd") &&  barr[1] != NULL) {
    t = chdir(barr[1]);
    if (t == -1 ){
      perror(barr[1]);
    }
  }else
  {
    pid_t pid;
    pid = fork();
    switch(pid) {
      case -1:
        perror("Fork:");
        break;
      case 0:
        execvp(barr[0],barr);
        perror(barr[0]);
        exit(0);
        break;
      default:
        if(amp_flag == 0){
          while(wait(NULL) != pid){
          }
        }
    }
  }
}


/*MAIN FUNCTION*/
int main()
{
  char** barr; 
  char *a;
  char l_chr;
  int arr_len,amp_flag=0;
  struct baselist *fel=NULL;
  struct charlist *cfel;
  do
  {
    l_chr=0;
    amp_flag = 0;
    printf(">");
    while((l_chr!='\n')&&(l_chr!='"')&&(l_chr!=EOF)&&(l_chr!='&')){
      cfel=build_list(&l_chr, &amp_flag);
      if(cfel != NULL){
        a=list_to_arr(cfel);
        fel=add_block_base(fel, a);
      }
    }
    switch(l_chr){
      case '"':
        printf("ERROR: quotation marks unbalaced \n");
        full_del_baselist(fel);
        fel=NULL;
        break;
      case '&':
        printf("ERROR: ampersand position invalid \n");
        full_del_baselist(fel);
        fel=NULL;
        skip();
        break;
      default:
        if(l_chr != EOF){
          barr=blist_to_barray(fel, &arr_len);
          execute(barr, amp_flag);
          del_barray(barr,arr_len+1);
          fel=NULL;
          break;
        }
		}
  }while(l_chr != EOF);
  return 0;
}
