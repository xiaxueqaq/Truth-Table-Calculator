#include<stdio.h>
#include<string.h>

char expr[255];
int  id[255];

int  op_pvl[255],op_cnt;
char op_stk[255];
int  el_stk[255],el_cnt;
int  vis[255];
int  var_cnt;
int  ns[255];  //namespace started with 1
int  var[255]; //boolean variable storage

int is_op(char c){
  //printf("calling is_op(%c) ret=%d\n",c,0||(c=='v')||(c=='^')||(c=='-')||(c=='=')||(c=='>')||(c=='(')||(c==')'));
  return 0||(c=='v')||(c=='^')||(c=='-')||(c=='=')||(c=='>')||(c=='(')||(c==')');
}

void assign_name(char* s){
  int i;
  for(i=0;i<strlen(s);i++){
    //printf("assign_name @ %d char:%c\n",i,s[i]);
    if(!is_op(s[i]))
      if(!vis[(int)(s[i])]){
	var_cnt++;
	ns[var_cnt]=(int)(s[i]);
	vis[(int)(s[i])]=1;
	id[i]=-(int)(s[i]);
      }
      else{
	id[i]=-(int)(s[i]);
      }
  }
}

void assign_pvl(char* s){
  int layer,i;
  layer=0;
  for(i=0;i<strlen(s);i++)
    if (is_op(s[i])){
      if (s[i]=='('){
	layer++;
	continue;
      }
      if (s[i]==')'){
	layer--;
	continue;	
      }
      if (s[i]=='-'){
	id[i]=2*layer+2;
	continue;
      }
      id[i]=2*layer+1;
    }
}

void examine(){

  if (op_stk[op_cnt]=='-'){
    el_stk[el_cnt]=1-el_stk[el_cnt];
    op_cnt--;
    return;
  }
  if (op_stk[op_cnt]=='>'){
    if ((el_stk[el_cnt-1]==1)&&(el_stk[el_cnt]==0))
      el_stk[el_cnt-1]=0;
    else
      el_stk[el_cnt-1]=1;
    el_cnt--;
    op_cnt--;
    return;
  }
  if (op_stk[op_cnt]=='='){
    if(el_stk[el_cnt-1]==el_stk[el_cnt])
      el_stk[el_cnt-1]=1;
    else
      el_stk[el_cnt-1]=0;
    el_cnt--;
    op_cnt--;
    return;
  }
  if (op_stk[op_cnt]=='^'){
    if((el_stk[el_cnt-1]==1)&&(el_stk[el_cnt]==1))
      el_stk[el_cnt-1]=1;
    else
      el_stk[el_cnt-1]=0;
    el_cnt--;
    op_cnt--;
    return;
  }
  if (op_stk[op_cnt]=='v'){
    if((el_stk[el_cnt-1]==0)&&(el_stk[el_cnt]==0))
      el_stk[el_cnt-1]=0;
    else
      el_stk[el_cnt-1]=1;
    el_cnt--;
    op_cnt--;
    return;
  }
}

int judge(void){
  int i;
  for(i=0;i<strlen(expr);i++){
    if (id[i]==0)
      continue;
    if (id[i]<0){
      el_cnt++;
      el_stk[el_cnt]=var[(int)(expr[i])];
    }
    else{
      if (id[i]>op_pvl[op_cnt]){
	op_cnt++;
	op_stk[op_cnt]=expr[i];
	op_pvl[op_cnt]=id[i];
      }      
      else {
	while(id[i]<=op_pvl[op_cnt])
	  examine();
	op_cnt++;
	op_stk[op_cnt]=expr[i];
	op_pvl[op_cnt]=id[i];
      }
    }
  }
  while(op_cnt>0)
    examine();
  //printf("judge done: op_cnt=%d, el_cnt=%d\n",op_cnt,el_cnt);
  return el_stk[1];
}

void dfs(int depth){ //start with 1
  if (depth==var_cnt+1){
    int i,res;
    op_cnt=0;
    el_cnt=0;
    res=judge();
    for(i=1;i<=var_cnt;i++)
      if(var[ns[i]])
	printf("T|");
      else
	printf("F|");
    if (res)
      printf(" =T\n");
    else
      printf(" =F\n");
    return;
  }
  var[ns[depth]]=1;
  dfs(depth+1);
  var[ns[depth]]=0;
  dfs(depth+1);
  
}

int main(void){

  printf("Copyright (c) 2019 xiaxueqaq @ BUAA 1709(SMS)/ 1723(SAE), All Rights Reserved\n");
  printf("Type \"help\" for usage, or input the boolean expression directly\n");
  while(1){
    scanf("%s",expr);
    if (strcmp(expr,"help")==0){
      printf("For Negation, input \"-\",\nFor Conjunction( i.e. and), input \"^\",\n");
      printf("For Disjunction( i.e. or), input letter \"v\",\nFor Conditional( i.e. ->), just input \">\",\n");
      printf("For Biconditional( i.e. <->), just input \"=\".\n");
    }
    else
      break;
  }
  assign_name(expr);
  assign_pvl(expr);

  int i;
  printf("Expression Parsed, %d Variables Found\n",var_cnt);
  //for(i=0;i<strlen(expr);i++)
  //  printf("id[%d]=%d, ",i,id[i]);
  //printf("\n");
  for(i=1;i<=var_cnt;i++){
    printf("%c ",(char)(ns[i]));    
  }
  printf(" result\n");
  dfs(1);

  return 0;
}
