#include <iostream>
#include <math.h>
#include <cstring>
#include "symb.h"
using namespace std;

Node :: Node (){//constructor
	type=0;
	value=NAN;
	arg1=NULL;
	arg2=NULL;
	}	
		
Node :: ~Node(){//deconstructor
	if(arg1!=NULL) delete arg1;
	if(arg2!=NULL) delete arg2;
	}
	
Node :: Node(char a){//constructor
	type=a;
	value=NAN;
	arg1=NULL;
	arg2=NULL;
	}

Node :: Node(char a,double b){//constructor
	type=a;
	value=b;
	arg1=NULL;
	arg2=NULL;
	}

Node :: Node(char a,Node* b){//constructor
	type=a;
	value=NAN;
	arg1=b;
	arg2=NULL;
	}
	
Node :: Node(char a,Node* b,Node* c){//constructor
	type=a;
	value=NAN;
	arg1=b;
	arg2=c;
	}
	
Node :: Node(char a, double x, Node* b, Node* c){//constructor
	type=a;
	value=x;
	arg1=b;
	arg2=c;
	}
	
Node :: Node(Node* s){//copy constructor
	if(s==NULL){
		cout<<"fucking hell, who passed a null pointer to the copy-constructor?"<<endl;
		exit(1);
		}
	type=s->type;
	value=s->value;
	arg1=NULL;
	arg2=NULL;
	if (s->arg1!=NULL) arg1= new Node(s->arg1);
	if (s->arg2!=NULL) arg2= new Node(s->arg2);
	}
	
Node :: Node (Node* s, double p){//shift constructor usato nel metodo del rapp incrementale
	if(s->type!='x'){
		type=s->type;
		value=s->value;
		arg1=NULL;
		arg2=NULL;
		if (s->arg1!=NULL) arg1= new Node(s->arg1,p);
		if (s->arg2!=NULL) arg2= new Node(s->arg2,p);
		}
	else{
		type='+';
		value=NAN;
		arg1= new Node('x');
		arg2= new Node('n',p);
		}
	}
	
int Node::size(){
	int a=1;
	if(arg1!=NULL){
		a=a+arg1->size();
		}
	if(arg2!=NULL){
		a=a+arg2->size();
		}
	return a;
	}
	
Node* Node::clean(){
	Node* cl=new Node(type,value,arg1==NULL?NULL:arg1->clean(), arg2==NULL?NULL:arg2->clean());
	/*
	if(arg1!=NULL){
		cl->arg1=arg1->clean();
		}
	if(arg2!=NULL){
		cl->arg2=arg2->clean();
		}
		*/
	Node* s=NULL;//swap
	if (cl->arg1!=NULL){//se non dipende da x posso ridurre un sotto albero ad un solo numero
		if (cl->arg1->type=='n'){
			if (cl->arg2!=NULL){
				if(cl->arg2->type=='n'){
					s=new Node('n',cl->eval(NAN));
					goto flag1;
					}
				}
			else{
				s=new Node('n',cl->eval(NAN));
				goto flag1;
				}
			}
		}
	switch (cl->type){
		case'+':
			if(cl->arg1->type=='-'){
				s=cl->arg1;
				cl->arg1=cl->arg2;
				cl->arg2=s;
				s=NULL;
				}
			if((cl->arg1->type=='n')&&(cl->arg1->value==0.0)){
				s=new Node(cl->arg2);
				break;
				}
			if((cl->arg2->type=='n')&&(cl->arg2->value==0.0)){
				s=new Node(cl->arg1);
				break;
				}
			if(cl->arg1->compare (cl->arg2)){
				s=new Node('*',new Node(cl->arg1),new Node('n',2.0));
				break;
				}
			if(((cl->arg1->type=='-')&&(cl->arg1->arg1->compare(cl->arg2)))||((cl->arg2->type=='-')&&(cl->arg2->arg1->compare(cl->arg1)))){
				s=new Node('n',0.0);
				break;
				}
			if((cl->arg1->type=='-')&&(cl->arg2->type=='-')){//anzi non dovri farlo anche per moltiplicativi arbitrari?
				s=new Node('-',new Node('+',new Node(cl->arg1->arg1),new Node(cl->arg2->arg1)));
				break;
				}
			goto flag0;
		case '*':
			if(((cl->arg1->type=='n')&&(cl->arg1->value==0.0))||((cl->arg2->type=='n')&&(cl->arg2->value==0.0))){
				s=new Node('n',0.0);
				break;
				}
			if((cl->arg1->type=='n')&&(cl->arg1->value==1.0)){
				s=new Node(cl->arg2);
				break;
				}
			if((cl->arg2->type=='n')&&(cl->arg2->value==1.0)){
				s=new Node(cl->arg1);
				break;
				}
			if((cl->arg1->type=='-')&&(cl->arg2->type=='-')){
				s=new Node('*',new Node(cl->arg1->arg1),new Node(cl->arg2->arg1));
				break;
				}
			if(cl->arg1->compare(cl->arg2)){
				s=new Node('p',new Node(cl->arg1),new Node('n',2.0));
				break;
				}
			if(((cl->arg1->type=='i')&&(cl->arg1->arg1->compare(cl->arg2)))||
				((cl->arg2->type=='i')&&(cl->arg2->arg1->compare(cl->arg1)))){
				s=new Node('n',1.0);
				break;
				}
			if((cl->arg1->type=='p')&&(cl->arg1->arg1->compare(cl->arg2))){
				s=new Node(
					'p',new Node(
						cl->arg1->arg1),new Node(
							'+',new Node(
								cl->arg1->arg2),new Node(
									'n',1.0)));
				break;
				}
			if((cl->arg2->type=='p')&&(cl->arg2->arg1->compare(cl->arg1))){
				s=new Node('p',new Node(cl->arg2->arg1),new Node('+',new Node(cl->arg2->arg2),new Node('n',1.0)));
				break;
				}
			if((cl->arg1->type=='p')&&(cl->arg2->type=='p')&&(cl->arg1->arg1->compare(cl->arg2->arg1))){
				s=new Node('p',new Node(cl->arg1->arg1),new Node('+',new Node(cl->arg1->arg2),new Node(cl->arg2->arg2)));
				break;
				}
			goto flag0;
		case '-':
			if(cl->arg1->type=='-'){
				s=new Node(cl->arg1->arg1);
				break;
				}
			goto flag0;
		case 'p':
			if((cl->arg2->type=='n')&&(cl->arg2->value==1.0)){
				s=new Node(cl->arg1);
				break;
				}
			if(cl->arg1->type=='p'){
				s=new Node('p',new Node(cl->arg1->arg1),new Node('*', new Node(cl->arg1->arg2),new Node(cl->arg2)));
				break;
				}
			goto flag0;
		case 'e':
			if(cl->arg1->type=='l'){
				s=new Node(cl->arg1->arg1);
				break;
				}
			goto flag0;
		case 'l':
			if(cl->arg1->type=='e'){
				s=new Node(cl->arg1->arg1);
				break;
				}
			goto flag0;
		default:
			flag0:
			s=new Node(cl);
		}
	flag1:
	delete cl;
	return s;
	}
	
Node* strton(char* s, int m){
	char* ctrl;
	double pippo=strtod(s,&ctrl);
	int j;

	if(m<1){
		goto err;
		}
	if (s[0]==' '){
		return strton(&s[1],m-1);
		}
	if (s[m-1]==' '){
		return strton(s,m-1);
		}
	if((s[0]=='(')&&(s[m-1]==')')){
		return strton(&s[1],m-2);
		}
	if((s[0]=='x')&&(m==1)){
		return new Node('x',NAN);
		}
	j=checkfor(s,m,'+');
	if(j!=(-1)){
		return new Node('+',strton(s,j),strton(&s[j+1],m-j-1));
		}
	j=checkfor(s,m,'-');
	if(j>0){
		return new Node('+',strton(s,j),strton(&s[j],m-j));
		}
	j=checkfor(s,m,'*');
	if(j!=-1){
		return new Node('*',strton(s,j),strton(&s[j+1],m-j-1));
		}
	j=checkfor(s,m,'/');
	if(j!=-1){
		return new Node('*',strton(s,j),new Node('i',strton(&s[j+1],m-j-1)));
		}
	j=checkfor(s,m,'^');
	if(j!=-1){
		return new Node('p',strton(s,j),strton(&s[j+1],m-j-1));
		}
	if(s[0]=='-'){
		return new Node('-',strton(&s[1],m-1));
		}
	if((ctrl-s)==m*sizeof(char)){
		cout<<pippo<<endl;
		return new Node('n',pippo);
		}
	
	if(m<6){
		goto err;
		}
	if((s[0]=='s')&&(s[1]=='i')&&(s[2]=='n')){
		return new Node('s',strton(&s[4],m-5));
		}
	
	if((s[0]=='c')&&(s[1]=='o')&&(s[2]=='s')){
		return new Node('c',strton(&s[4],m-5));
		}
	
	if((s[0]=='e')&&(s[1]=='x')&&(s[2]=='p')){
		return new Node('e',strton(&s[4],m-5));
		}
	
	if((s[0]=='l')&&(s[1]=='o')&&(s[2]=='g')){
		return new Node('l',strton(&s[4],m-5));
		}
		
	if(m<8){
		goto err;
		}
		
	if((s[0]=='p')&&(s[1]=='o')&&(s[2]=='w')){
		j=checkfor(&s[4],m-5,',');
		if((j<1)||(j>m-7)){
			goto err;
			}
		return new Node('p',strton(&s[4],j),strton(&s[j+5],m-6-j));
	}
	err:
	cout << "Syntax Error" << endl; //learn about throwing exeptions
	return NULL;
}
Node* Node::set_ad(){
	Node* ad=NULL;
	switch (type){
		case 'n':
			ad = new Node('n',0.0);
			break;
		case 'x':
			ad = new Node('n',1.0);
			break;
		case '-':
			ad = new Node('-');
			ad->arg1 = arg1->set_ad();
			break;
		case '+':
			ad = new Node('+');
			ad->arg1 = arg1->set_ad();
			ad->arg2 = arg2->set_ad();
			break;
		case '*':
			ad = new Node('+',NAN);
			ad->arg1 = new Node('*');
			ad->arg2 = new Node('*');
			(ad->arg1)->arg1=new Node(arg1);
			(ad->arg1)->arg2=arg2->set_ad();
			(ad->arg2)->arg1= new Node(arg2);
			(ad->arg2)->arg2=arg1->set_ad();
			break;
		case 'i':
			ad = new Node('*');
			ad->arg1 = arg1->set_ad();
			ad->arg2 = new Node('-');
			(ad->arg2)->arg1 = new Node('i');
			((ad->arg2)->arg1)->arg1 = new Node('p');
			(((ad->arg2)->arg1)->arg1)->arg1 = new Node(arg1);
			(((ad->arg2)->arg1)->arg1)->arg2 = new Node('n',2.0);
			break;
		case 'e':
			ad = new Node('*',NAN);
			ad->arg1 = arg1->set_ad();
			ad->arg2 = new Node(this);
			break;
		case 's':
			ad = new Node('*',NAN);
			ad->arg1 = arg1->set_ad();
			ad->arg2 = new Node('c',NAN);
			(ad->arg2)->arg1 = new Node(arg1);
			break;
		case 'c':
			ad = new Node('*',NAN);
			ad->arg1 = arg1->set_ad();
			ad->arg2 = new Node('-',NAN);
			(ad->arg2)->arg1 = new Node('s',NAN);
			((ad->arg2)->arg1)->arg1 = new Node(arg1);
			break;
		case 'l':
			ad = new Node('*',NAN);
			ad->arg1 = arg1->set_ad();
			ad->arg2 = new Node('i',NAN);
			(ad->arg2)->arg1 = new Node(arg1);
			break;
		case 'p':
			ad = new Node('*');
			ad->arg1 = new Node(this);
			ad->arg2 = new Node('+');
			ad->arg2->arg1 = new Node('*');
			ad->arg2->arg1->arg1 = new Node(arg2);
			ad->arg2->arg1->arg2 = new Node('*');
			ad->arg2->arg1->arg2->arg1 = arg1->set_ad();
			ad->arg2->arg1->arg2->arg2 = new Node('i');
			ad->arg2->arg1->arg2->arg2->arg1 = new Node(arg1);
			ad->arg2->arg2 = new Node('*');
			ad->arg2->arg2->arg1 = new Node('l');
			ad->arg2->arg2->arg1->arg1 = new Node(arg1);
			ad->arg2->arg2->arg2 = arg2->set_ad();
		default:;
	}
	if(ad==NULL){
			cout<<"You tried to derive a bad Node"<<endl;
			exit(1);
			}
		return ad;
	}

Node* Node::set_fd(){
	Node* fd;
	fd = new Node('*',NAN);
	fd->arg1= new Node('+',NAN);
	fd->arg2= new Node('i',NAN);
	fd->arg2->arg1 = new Node('n',h);
	fd->arg1->arg1 = new Node(this,h);
	fd->arg1->arg2 = new Node('-',NAN);
	fd->arg1->arg2->arg1 = new Node(this);
	return fd;
	}

void Node::text(){
	switch (type){
		case 'n':
			cout<<value;
			break;
		case 'x':
			cout<<'x';
			break;
		case '-':
			cout<<"-";
			arg1->text();
			break;
		case '+':
			cout<<"(";
			arg1->text();
			if(arg2->type=='-'){
				cout<<"-";
				arg2->arg1->text();
				}
			else{
				cout<<"+";
				arg2->text();
				}
			cout<<")";
			break;
		case '*':
			arg1->text();
			cout<<"*";
			arg2->text();
			break;
		case 'e':
			cout<<"exp(";
			arg1->text();
			cout<<")";
			break;
		case 'i':
			cout<<"1/(";
			arg1->text();
			cout<<")";
			break;
		case 's':
			cout<<"sin(";
			arg1->text();
			cout<<")";
			break;
		case 'c':
			cout<<"cos(";
			arg1->text();
			cout<<")";
			break;
		case 'l':
			cout<<"log(";
			arg1->text();
			cout<<")";
			break;
		case 'p'://aggiungere il caso di (^-1)=(/)
			if((arg2->type=='n')&&(arg2->value==-1.0)){//should be useless if i want to keep only positive powers
				cout<<"1/";
				arg1->text();
				}
			else{
				arg1->text();
				cout<<"^(";
				arg2->text();
				cout<<")";
				}
			break;
		default:;
		}
	}

double Node::eval(double x){
	switch (type){
		case 'n':
			return value;
		case 'x':
			return x;
		case '-':
			return -(arg1->eval(x));
		case '+':
			return arg1->eval(x)+arg2->eval(x);
		case '*':
			return (arg1->eval(x))*(arg2->eval(x));
		case 'e':
			return exp(arg1->eval(x));
		case 'i':
			return 1.0/arg1->eval(x);
		case 's':
			return sin(arg1->eval(x));
		case 'c':
			return cos(arg1->eval(x));
		case 'l':
			return log(arg1->eval(x));
		case 'p':
			return pow(arg1->eval(x),arg2->eval(x));
		default:
			return x;
		}
	}	
	
bool Node::compare(Node* a){
	if(a->type==type){
		if(a->arg2!=NULL){
			return (arg1->compare(a->arg1)&arg2->compare(a->arg2))|(arg2->compare(a->arg1)&arg1->compare(a->arg2));
			}
		else if(a->arg1!=NULL){
			return arg1->compare(a->arg1);
			}
		else if((a->type=='n')&&(a->value!=value)){
			return 0;
			}
		else return 1;
		}
	return 0;
	}
	
int checkfor(char* s, int m, char l){
	int i,k=0;
	for (i=m-1;i>=0;i--){
		if (s[i]=='(') k++;
		else if(s[i]==')') k--;
		if((s[i]==l)&&(k==0)) {
			break;
			}
		}
	return i;
	}
