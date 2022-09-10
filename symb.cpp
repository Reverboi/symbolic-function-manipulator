#include <iostream>
#include <math.h>
#include <cstring>
#include "symb.h"
using namespace std;

Node :: Node (){//constructor
	ord=0;
	arg=NULL;
	}	
	
Node :: Node(Node** a,int b){
	arg=a;
	ord=b;
	}

Node :: ~Node(){//deconstructor
	for(int i=0;i<ord;i++){
		if(arg[i]!=NULL) delete arg[i];
		}
	free(arg);
	}

Number :: Number(double b){//constructor
	value=b;
	}

Node :: Node(Node* b){//constructor
	ord=1;
	arg=(Node**)malloc(ord * sizeof(Node*));
	arg[0]=b;
	}

Node :: Node(Node* b,Node* c){//constructor
	ord=2;
	arg=(Node**)malloc(ord * sizeof(Node*));
	arg[0]=b;
	arg[1]=c;
	}
	
Node* Sum :: copy(){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->copy();
	return new Sum(a,ord);
	}
Node* Minus :: copy(){return new Minus(arg[0]->copy());}
Node* Exp:: copy(){return new Exp(arg[0]->copy());}
Node* Cos:: copy(){return new Cos(arg[0]->copy());}
Node* Sin:: copy(){return new Sin(arg[0]->copy());}
Node* Log:: copy(){return new Log(arg[0]->copy());}
Node* Pow:: copy(){return new Pow(arg[0]->copy(),arg[1]->copy());}
Node* X:: copy(){return new X();}
Node* Prod :: copy(){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->copy();
	return new Prod(a,ord);
	}
Node* Number::copy(){return new Number(value);}
	/*
Node* Node :: shift_copy(double b){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->shift_copy(b);
	return new Node(a,ord);//will this call the right derived constructor?
	}
	
Node* X::shift_copy(double b){return new Sum(copy(),new Number(b));}
*/
Node* Sum :: omit(int b){
	Node** a=(Node**)malloc((ord-1)*sizeof(Node*));
	int i=0;
	for(int j=0;j<ord;j++){
		if(j!=b){
			a[i]=arg[j]->copy();
			}
		else i--;
		i++;
		}
	if(ord==2) return a[0]->copy();
	else return new Sum(a,ord-1);
	}
	
Node* Prod::omit(int b){
	Node** a=(Node**)malloc((ord-1)*sizeof(Node*));
	int i=0;
	for(int j=0;j<ord;j++){
		if(j!=b){
			a[i]=arg[j]->copy();
			}
		else i--;
		i++;
		}
	if(ord==2) return a[0]->copy();
	else return new Prod(a,ord-1);
	}
int Node::size(){
	int r=1;
	for(int i=0;i<ord;i++) r=r+arg[1]->size();
	return r;
	}
	
bool Node::just_numbers(){
	for(int i=0;i<ord;i++){
		if(typeid(*arg[i]).name()!=typeid(Number).name()) return 0;
		}
	return 1;
	}

Node* Sum::fuse(Node* a){
	int o=ord+a->ord;
	Node** b=(Node**)malloc(o*sizeof(Node*));
	for(int i=0;i<ord;i++) b[i]=arg[i]->copy();
	for(int i=0;i<a->ord;i++) b[i]=a->arg[i+ord]->copy();
	return new Sum(b,o);
	}

Node* Prod::fuse(Node* a){
	int o=ord+a->ord-1;
	Node** b=(Node**)malloc(o*sizeof(Node*));
	for(int i=0;i<ord;i++) b[i]=arg[i]->copy();
	for(int i=0;i<a->ord;i++) b[i+ord]=a->arg[i]->copy();
	return new Prod(b,o);
	}

Node* Sum::clean(){
	Node** c=(Node**)malloc(sizeof(Node*)*ord);
	for(int i=0;i<ord;i++){
		c[i]=arg[i]->clean();
		}
	Node* cl=new Sum(c,ord);
	if(cl->just_numbers()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	else {for(int i=0;i<cl->ord;i++){
		if(typeid(*(cl->arg[i])).name()==typeid(Number).name()){
			if((static_cast<Number*>(cl->arg[i]))->value==0.0){
				Node* s=cl->omit(i);
				delete cl;
				cl=s;
				i--;
				}
			}
		}
		int k=cl->ord;
		for(int i=0;i<k;i++){
			if(typeid(cl->arg[i])==typeid(Sum*)){
				Node* s=cl->fuse(cl->arg[i]);
				delete cl;
				cl=s;
				}
			}
		}
	return cl;
	}
Node* Prod::clean(){
	Node** c=(Node**)malloc(sizeof(Node*)*ord);
	for(int i=0;i<ord;i++){
		c[i]=arg[i]->clean();
		}
	Node* cl=new Prod(c,ord);
	if(cl->just_numbers()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	else {for(int i=0;i<cl->ord;i++){
		if(typeid(*(cl->arg[i])).name()==typeid(Number).name()){
			if((static_cast<Number*>(cl->arg[i]))->value==0.0){
				Node* s=new Number(0.0);
				delete cl;
				cl=s;
				break;
				}
			if((static_cast<Number*>(cl->arg[i]))->value==1.0){
				Node* s=cl->omit(i);
				delete cl;
				cl=s;
				i--;
				}
			}
		}
		int k=cl->ord;
		for(int i=0;i<k;i++){
			if(typeid(*(cl->arg[i])).name()==typeid(Prod).name()){
				Node* s=cl->fuse(cl->arg[i]);
				delete cl;
				cl=s;
				}
			}
		}
	return cl;
	}
Node* Minus::clean(){
	Node* cl=new Minus(arg[0]->clean());
	if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){		//-0 = 0
		if((static_cast<Number*>(cl->arg[0]))->value==0.0){
			Node* s=new Number(0.0);
			delete cl;
			cl=s;
			}
		}
	else if(typeid(*(cl->arg[0])).name()==typeid(Minus).name()){	//-(-())=()
		Node* s=cl->arg[0]->arg[0]->copy();
		delete cl;
		cl=s;
		}
	return cl;
	}

Node* Cos::clean(){
	Node* cl=new Cos(arg[0]->clean());
	if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	return cl;
	}
	
Node* Sin::clean(){
	Node* cl=new Sin(arg[0]->clean());
	if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	return cl;
	}

Node* Exp::clean(){
	Node* cl=new Exp(arg[0]->clean());
	if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	return cl;
	}
	
Node* Pow::clean(){
	Node* cl=new Pow(arg[0]->clean(),arg[1]->clean());
	if(cl->just_numbers()){
		Node* s = new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	else if(typeid(*(cl->arg[1])).name()==typeid(Number).name()){
		if((static_cast<Number*>(cl->arg[1]))->value==0.0){
			double q=1.0;
			if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){
				if((static_cast<Number*>(cl->arg[0]))->value==0.0) q=NAN;
				}
			Node* s = new Number(q);
			delete cl;
			cl=s;
			}
		}
	return cl;
	}

Node* Log::clean(){
	Node* cl=new Log(arg[0]->clean());
	if(typeid(*(cl->arg[0])).name()==typeid(Number).name()){
		Node* s=new Number(cl->eval(NAN));
		delete cl;
		cl=s;
		}
	else if(typeid(*(cl->arg[0])).name()==typeid(Exp).name()){
		Node* s=cl->arg[0]->arg[0]->copy();
		delete cl;
		cl=s;
		}
	return cl;
	}
	
Node* X::clean(){return copy();} 
Node* Number::clean(){
	if(value<0.0) return new Minus(new Number(-value));//is this really a good idea?
	else return copy();
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
		return new X();
		}
	
	j=checkfor(s,m,'+');
	if(j!=(-1)){
		return new Sum(strton(s,j),strton(&s[j+1],m-j-1));
		}
	j=checkfor(s,m,'-');
	if(j>0){
		return new Sum(strton(s,j),strton(&s[j],m-j));
		}
	j=checkfor(s,m,'*');
	if(j!=-1){
		return new Prod(strton(s,j),strton(&s[j+1],m-j-1));
		}
	j=checkfor(s,m,'/');
	if(j!=-1){
		return new Prod(strton(s,j),new Pow(strton(&s[j+1],m-j-1),new Minus(new Number(1.0))));
		}
	j=checkfor(s,m,'^');
	if(j!=-1){
		return new Pow(strton(s,j),strton(&s[j+1],m-j-1));
		}
	if(s[0]=='-'){
		return new Minus(strton(&s[1],m-1));
		}
	if((ctrl-s)==m*sizeof(char)){
		cout<<pippo<<endl;
		return new Number(pippo);
		}
	
	if(m<6){
		goto err;
		}
	if((s[0]=='s')&&(s[1]=='i')&&(s[2]=='n')){
		/*
		Node* b=strton(&s[4],m-5);
		Sin* c= new Sin(b);
		return c;*/
		return new Sin(strton(&s[4],m-5));
		}
	
	if((s[0]=='c')&&(s[1]=='o')&&(s[2]=='s')){
		return new Cos(strton(&s[4],m-5));
		}
	
	if((s[0]=='e')&&(s[1]=='x')&&(s[2]=='p')){
		return new Exp(strton(&s[4],m-5));
		}
	
	if((s[0]=='l')&&(s[1]=='o')&&(s[2]=='g')){
		return new Log(strton(&s[4],m-5));
		}
		
	if(m<8){
		goto err;
		}
		
	if((s[0]=='p')&&(s[1]=='o')&&(s[2]=='w')){
		j=checkfor(&s[4],m-5,',');
		if((j<1)||(j>m-7)){
			goto err;
			}
		return new Pow(strton(&s[4],j),strton(&s[j+5],m-6-j));
	}
	err:
	cout << "Syntax Error" << endl; //learn about throwing exeptions
	return NULL;
}

Node* Sum ::set_ad(){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++){
		a[i]=arg[i]->set_ad();
		}
	return new Sum(a,ord);
	}

Node* Prod::set_ad(){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++){
		a[i]= new Prod(omit(i)->set_ad(),arg[i]->copy());
		}
	return new Sum(a,ord);
	}
	
Node* Minus::set_ad() {return new Minus(arg[0]->set_ad());}
Node* Exp::set_ad(){return new Prod(copy(),arg[0]->set_ad());}
Node* Sin::set_ad(){return new Prod(new Cos(arg[0]->copy()),arg[0]->set_ad());}
Node* Cos::set_ad(){return new Prod(new Minus(new Sin(arg[0]->copy())),arg[0]->set_ad());}
Node* Log::set_ad(){return new Prod(arg[0]->set_ad(),new Pow(arg[0]->copy(),new Minus(new Number(1.0))));}
Node* Pow::set_ad(){return new Prod(copy(),new Sum(new Prod(new Prod(arg[0]->set_ad(),new Pow(arg[0]->copy(),new Minus(new Number(1.0)))),arg[1]->copy()),new Prod(new Log(arg[0]->copy()),arg[1]->set_ad())));}
Node* X:: set_ad(){
	Node* a = new Number(1.0);
	return a;
	}
Node* Number::set_ad(){return new Number(0.0);}
/*
Node* Node::set_fd(){
	return new Prod(new Sum(shift_copy(h),new Minus(copy())),new Pow(new Number(h),new Minus(new Number(1.0))));
	}
*/
void Number::text(){cout<<value;}

void X::text(){cout<<'x';}

void Minus::text(){
	cout<<'-';
	arg[0]->text();
	}
	
void Sum::text(){
	cout<<'(';
	arg[0]->text();
	for(int i=1;i<ord;i++){
		if (typeid(arg[i])!=typeid(Minus*)) cout<<'+';
		arg[i]->text();
		}
	cout<<')';
	}
	
void Prod::text(){
	arg[0]->text();
	for(int i=1;i<ord;i++){
		cout<<'*';
		arg[i]->text();
		}	
	}
	
void Exp::text(){
	cout<<"exp(";
	arg[0]->text();
	cout<<')';
	}
	
void Cos::text(){
	cout<<"cos(";
	arg[0]->text();
	cout<<')';
	}
	
void Sin::text(){
	cout<<"sin(";
	arg[0]->text();
	cout<<')';
	}
	
void Log::text(){
	cout<<"log(";
	arg[0]->text();
	cout<<')';
	}
	
void Pow::text(){
	arg[0]->text();
	cout<<"^(";
	arg[1]->text();
	cout<<')';
	}

double Number::eval(double x){return value;}
double X::eval(double x){return x;}
double Minus::eval(double x){return -arg[0]->eval(x);}
double Sum::eval(double x){
	double r=0.0;
	for(int i=0;i<ord;i++){
		r=r+arg[i]->eval(x);
		}
	return r;
	}
double Prod::eval(double x){
	double r=1.0;
	for(int i=0;i<ord;i++){
		r=r*arg[i]->eval(x);
		}
	return r;
	}
	
double Exp::eval(double x){return exp(arg[0]->eval(x));}
double Log::eval(double x){return log(arg[0]->eval(x));}
double Sin::eval(double x){return sin((arg[0])->eval(x));}
double Cos::eval(double x){return cos(arg[0]->eval(x));}
double Pow::eval(double x){return pow(arg[0]->eval(x),arg[1]->eval(x));}
	
bool Node::compare(Node* a){//arguments must be ordered somewhere
	if ((ord!=a->ord)||(typeid(a)!=typeid(this))) return 0;
	bool s=1;
	for(int i=0;i<ord;i++){
		if(!(arg[i]->compare(a->arg[i]))) return 0;
		}
	return 1;
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
	
Node* Node::copy(){return 0;}
Node* Node::omit(int){return 0;}
Node* Node::set_ad(){cout<<"^^!"<<endl;return 0;}
void Node::text(){cout<<"ops!"<<endl;}
double Node::eval(double){return 0;}
Node* Node::fuse(Node*){return 0;}
