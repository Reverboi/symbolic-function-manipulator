#include <iostream>
#include <math.h>
#include <cstring>
#include "symb.hpp"
using namespace std;

		//CONSTRUCTORS

Node :: Node (){
	ord=0;
	arg=NULL;
	}

Node :: Node(Node** a,int b){
	arg=a;
	ord=b;
	}

Node :: Node(Node* b){
	ord=1;
	arg=nalloc(ord);
	arg[0]=b;
	}

Node :: Node(Node* b,Node* c){
	ord=2;
	arg=nalloc(ord);
	arg[0]=b;
	arg[1]=c;
	}

Number :: Number(double b){
	ord=0;
	arg=NULL;
	value=b;
	}

		//DESTRUCTOR

Node :: ~Node(){
	for(int i=0;i<ord;i++){
		if(arg[i]!=NULL) delete arg[i];
		}
	free(arg);
	}

			//COPY

Node* Node  :: copy(){
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->copy();//recursion
	return self(a,ord);
	}

Node* Number::copy(){return new Number(value);}

Node* Node :: omit(int b){
	if ((b<0)||(b>=ord)){
		cout<<"omit out of bounds"<<endl;
		return NULL;
		}
	Node** a=nalloc(ord-1);
	int i=0;
	for(int j=0;j<b;j++){
			a[i++]=arg[j]->copy();
		}
	for(int j=b+1;j<ord;j++){
			a[i++]=arg[j]->copy();
		}
	return self(a,ord-1);
	}

int Node::size(){
	int r=1;
	for(int i=0;i<ord;i++) r=r+arg[i]->size();
	return r;
	}
	
Node* Node::append(Node* s){
	int k=ord+1;
	Node** a=nalloc(k);
	for (int i=0;i<ord;i++) a[i]=arg[i]->copy();
	a[ord]=s->copy();
	return self(a,k);
	}

		//CLEAN    pre-computing, non-type-preserving

Node* Sum::clean(){
	if(ord==1) return arg[0]->copy();
	if(ord==0) return new Number(0.0); //spero non torni mai utile
	int n=ord;
	double tot=0.0;
	bool* f=(bool*)malloc(sizeof(bool)*ord);//flag
	for(int i=0;i<ord;i++){
		f[i]=true;
		if(typeid(*(arg[i])).name()==typeid(Number).name()){// conto i numeri
			tot += (static_cast<Number*>(arg[i]))->value; 
			n--;
			f[i]=false;
			}
		}
	if(n==0){
		free (f);
		return new Number(tot);
		}
	if(tot!=0.0) n++;
	else if(n==1){
		for(int i=0;i<ord;i++){
			if(f[i]) {
				free (f);
				return arg[i]->copy();
				}
			}
		}
	Node** a = nalloc(n);
	for(int i=0,j=0;i<ord;i++){
		if(f[i]) a[j++]=arg[i]->copy();
		}
	free(f);
	if (tot!=0.0) a[n-1]=new Number (tot);
	
	return new Sum(a,n);
	}

Node* Prod::clean(){
	if(ord==1) return arg[0]->copy();
	if(ord==0) return new Number(1.0); //spero non torni mai utile
	int n=ord;
	double tot=1.0;
	bool* f=(bool*)malloc(sizeof(bool)*ord);//flag
	for(int i=0;i<ord;i++){
		f[i]=true;
		if(typeid(*(arg[i])).name()==typeid(Number).name()){// conto i numeri
			tot = tot * (static_cast<Number*>(arg[i]))->value; 
			n--;
			f[i]=false;
			}
		}
	if((n==0)||(tot==0)){
		free (f);
		return new Number(tot);
		}
	if(tot!=1.0) n++;
	else if(n==1){
		for(int i=0;i<ord;i++){
			if(f[i]) {
				free (f);
				return arg[i]->copy();
				}
			}
		}
	Node** a = nalloc(n);
	for(int i=0,j=0;i<ord;i++){
		if(f[i]) a[j++]=arg[i]->copy();
		}
	free(f);
	if (tot!=1.0) a[n-1]=new Number (tot);
	
	return new Prod(a,n);
	}

Node* Node::clean(){
	if(typeid(*arg[0]).name()==typeid(Number).name()){///potreppe dare problemi se è un leaf node
		return new Number(eval(NAN));
		}
	return copy();
	}

Node* Pow::clean(){
	if((typeid(*arg[0]).name()==typeid(Number).name())&&(typeid(*arg[1]).name()==typeid(Number).name())){
		return new Number(eval(NAN));
		}
	if(typeid(*arg[0]).name()==typeid(Number).name()){
		if((static_cast<Number*>(arg[0]))->value==1.0) return new Number(1.0);
		if((static_cast<Number*>(arg[0]))->value==0.0) return new Number(0.0);
		}
	if(typeid(*arg[1]).name()==typeid(Number).name()){
		if((static_cast<Number*>(arg[1]))->value==0.0) return new Number(1.0);
		if((static_cast<Number*>(arg[1]))->value==1.0) return arg[0]->copy();
		}
	return copy();
	}

Node* Log::clean(){
	if(typeid(*arg[0]).name()==typeid(Number).name()){
		return new Number(eval(NAN));//doesent matter whwere you evaluate it, thats for x
		}
	if(typeid(*arg[0]).name()==typeid(Exp).name()){
		return arg[0]->arg[0]->copy();
		}
	return copy();
	}
	
Node* X::clean(){return copy();} 

Node* Number::clean(){
	return copy();
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
	if(s[m-1]==')'){
		if (checkfor(s,m,'(')==0) return strton(&s[1],m-2);
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
		return new Prod(strton(s,j),new Pow(strton(&s[j+1],m-j-1),new Number(-1.0)));
		}
	j=checkfor(s,m,'^');
	if(j!=-1){
		return new Pow(strton(s,j),strton(&s[j+1],m-j-1));
		}
	if(s[0]=='-'){
		return new Prod(new Number(-1.0),strton(&s[1],m-1));
		}
	if((ctrl-s)==m*sizeof(char)){
		return new Number(pippo);
		}
	
	if(m<6){
		goto err;
		}
	if((s[0]=='s')&&(s[1]=='i')&&(s[2]=='n')){
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
			cout<<'?'<<endl;
			goto err;
			}
		return new Pow(strton(&s[4],j),strton(&s[j+5],m-6-j));
		}
	err:
	cout << "Syntax Error" << endl; //learn about throwing exeptions
	return NULL;
}

			//SET_AD

Node* Sum ::set_ad(){
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i] = arg[i]->set_ad();
	return new Sum(a,ord);
	}

Node* Prod::set_ad(){
	if(ord==1) return arg[0]->set_ad();
	Node** a=nalloc(ord);
	
	for(int i=0;i<ord;i++){
		
		a[i] = new Prod(omit(i),arg[i]->set_ad());
		}
	return new Sum(a,ord);
	}


Node* Exp::set_ad(){return new Prod(copy(),arg[0]->set_ad());}
Node* Sin::set_ad(){return new Prod(new Cos(arg[0]->copy()),arg[0]->set_ad());}
Node* Cos::set_ad(){return new Prod(new Prod(new Sin(arg[0]->copy()),arg[0]->set_ad()),new Number(-1.0));}
Node* Log::set_ad(){return new Prod(arg[0]->set_ad(),new Pow(arg[0]->copy(),new Number(-1.0)));}
Node* Pow::set_ad(){return new Prod(copy(),new Sum(new Prod(new Prod(arg[0]->set_ad(),new Pow(arg[0]->copy(),new Number(-1.0))),arg[1]->copy()),new Prod(new Log(arg[0]->copy()),arg[1]->set_ad())));}
Node* X:: set_ad(){return new Number(1.0);}
Node* Number::set_ad(){return new Number(0.0);}

			//TEXT

void Number::text(){
	if(value>=0) cout<<value;
	else cout<<'('<<value<<')';
	}

void X::text(){cout<<'x';}

void Sum::text(){
	arg[0]->text();//not ideal
	for(int i=1;i<ord;i++){
		cout<<'+';
		arg[i]->text();
		}
	}

void Prod::text(){
	double t=1.0;
	bool* f=(bool*)malloc(ord*sizeof(bool));
	for(int i=0;i<ord;i++){// i know a display function shouldn't do computation but i just wanted to make it work
		f[i]=true;
		if(typeid(*arg[i]).name()==typeid(Number).name()){
			t=t*(static_cast<Number*>(arg[i]))->value;
			f[i]=false;
			}
		}
	if (t==-1.0) cout<<'-';
	else if (t!=1) cout<<t;
	for(int i=0,j=0;i<ord;i++){
		if(f[i]){
			j++;
			if((j!=1)||((j==1)&&(t!=1)&&(t!=-1))) cout<<'*'; 
			if(arg[i]->ord>1){
				cout<<'(';
				arg[i]->text();
				cout<<')';
				}
			else arg[i]->text();
			}
		}
	free (f);
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
	if(arg[0]->ord>1){
		cout<<'(';
		arg[0]->text();
		cout<<')';
		}
	else arg[0]->text();
	cout<<"^";
	if(arg[1]->ord>1){
		cout<<'(';
		arg[1]->text();
		cout<<')';
		}
	else arg[1]->text();
	}

			//EVAL

double Number::eval(double x){return value;}
double X::eval(double x){return x;}

double Sum::eval(double x){
	double r=0.0;
	for(int i=0;i<ord;i++) r=r+arg[i]->eval(x);//recursion
	return r;
	}
	
double Prod::eval(double x){
	double r=1.0;
	for(int i=0;i<ord;i++) r=r*arg[i]->eval(x);//recursion
	return r;
	}
	
double Exp::eval(double x){return exp(arg[0]->eval(x));}
double Log::eval(double x){return log(arg[0]->eval(x));}
double Sin::eval(double x){return sin(arg[0]->eval(x));}
double Cos::eval(double x){return cos(arg[0]->eval(x));}
double Pow::eval(double x){return pow(arg[0]->eval(x),arg[1]->eval(x));}
	
bool Node::compare(Node* a){//arguments must be ordered somewhere, actually they don't
	if ((ord!=a->ord)||(typeid(a)!=typeid(this))) return 0;
	bool s=1;
	for(int i=0;i<ord;i++){
		if(!(arg[i]->compare(a->arg[i]))) return 0;//recursion
		}
	return 1;
	}

bool operator== (const Node& aa, const Node& bb){
	Node* a=(Node*)&aa;
	Node* b=(Node*)&bb;
	if ((typeid(*a)!=typeid(*b))||(a->ord!=b->ord)){
		return false;
		}
	bool* k=(bool*)malloc(sizeof(bool)*a->ord);
	for(int i=0;i<a->ord;i++) k[i]=true;
	for(int i=0;i<a->ord;i++){
		for(int j=0;j<b->ord;j++){
			if((*(a->arg[i])==*(b->arg[j]))&&(k[j])){//recursion
				k[j]=false;
				break;
				}
			if(j==b->ord-1){
				return false;
				}
			}
		}
	if(typeid(*a)==typeid(Number)){
		if((static_cast<Number*>(a))->value!=(static_cast<Number*>(b))->value){
			return false;
			}
		}
	return true;
	}

int checkfor(char* s, int m, char l){
	int i,k=0;
	for (i=m-1;i>=0;i--){
		if (s[i]==')') k++;
		else if(s[i]=='(') k--;
		if((s[i]==l)&&(k==0)) {
			break;
			}
		}
	return i;
	}
	
			//ABSORB

Node* Sum  :: absorb(){
	int n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Sum).name()){
			n+=arg[i]->ord;
			}
		else n++;
	}
	Node** g=nalloc(n);
	n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Sum).name()){
			for(int j=0;j<arg[i]->ord;j++){
				g[n++]=arg[i]->arg[j]->copy();
				}
			}
		else g[n++]=arg[i]->copy();
		}
	return new Sum(g,n);
	}

Node* Prod  :: absorb(){//is basically the same as Sum ::absorb
	int n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Prod).name()){
			n+=arg[i]->ord;
			}
		else n++;
	}
	Node** g=nalloc(n);
	n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Prod).name()){
			for(int j=0;j<arg[i]->ord;j++){
				g[n++]=arg[i]->arg[j]->copy();
				}
			}
		else g[n++]=arg[i]->copy();
		}
	return new Prod(g,n);
	}

Node* Pow :: absorb(){
	if(typeid(*arg[0]).name()==typeid(Pow).name()){//manca l'assorbimento per basi naturali e
		Node* c=arg[0]->arg[0]->copy();
		Node* d=new Prod(arg[0]->arg[1]->copy(),arg[1]->copy());//potrei chiedergli di assorbire questo nuovo prodotto o non serve?
		return new Pow(c,d);
		}
	return copy();
	}
	
Node* Node :: absorb(){return copy();}//could be tolta from the node def
	
			//EXPAND

Node* Node :: expand(){return copy();}//also maybe not needed

Node* Prod::expand(){//very proud of this one
	int n=1;
	for(int i=0;i<ord;i++){
		if(typeid(*arg[i]).name()==typeid(Sum).name()){
			n=n*arg[i]->ord;
			}
		}
	Node*** c=(Node***)malloc(sizeof(Node**)*n);
	for(int i=0;i<n;i++) c[i]=nalloc(ord);
	int m=1;
	for(int j=0;j<ord;j++){
		if(typeid(*arg[j]).name()==typeid(Sum).name()){
			for(int i=0;i<n;i++) c[i][j]=arg[j]->arg[(i/m)%arg[j]->ord]->copy();
			m=m*arg[j]->ord;
			}
		else for(int i=0;i<n;i++) c[i][j]=arg[j]->copy();
		}
	Node** b=nalloc(n);
	for(int i=0;i<n;i++) b[i] = new Prod(c[i],ord);
	return new Sum(b,n);
	}
	
Node* Pow  :: expand(){
	if(typeid(*(arg[0])).name()==typeid(Prod).name()){
		Node** a=nalloc(arg[0]->ord);
		for(int i=0;i<arg[0]->ord;i++) a[i]=new Pow(arg[0]->arg[i]->copy(),arg[1]->copy());
		return new Prod(a,arg[0]->ord);
		}
	else return copy();
	}


			//MATCH

Node* Prod :: match(){
	int* key=(int*)malloc(ord*sizeof(int));
	bool* is_pow=(bool*)malloc(ord*sizeof(bool));
	for(int i=0; i<ord;i++){
		key[i]=ord;
		if(typeid(*(arg[i])).name()==typeid(Pow).name()) is_pow[i]=true;
		else is_pow[i]=false;
		}
	int s=0; // numero di "famiglie"
	for(int i=0; i<ord;i++){
		if(key[i]==ord){
			key[i]=s++;
			Node* a;
			if(is_pow[i]) a=arg[i]->arg[0];
			else a=arg[i];
			for(int j=i+1;j<ord;j++){
				Node* b;
				if(is_pow[j]) b=arg[j]->arg[0];
				else b=arg[j];
				if((*a)==(*b)) key[j]=key[i];
				}
			}
		}
	Node** q = nalloc(s);// elem della produttoria
	for(int i=0; i<s;i++){
		int k=0;
		for(int j=0;j<ord;j++){
			if(key[j]==i) k++;
			}
		Node** p=nalloc(k);
		Node* base;
		k=0;
		bool need_base=true;
		for(int j=0;j<ord; j++){
			if(key[j]==i){
				if(need_base){
					if(is_pow[j]) base=arg[j]->arg[0]->copy();
					else base = arg[j]->copy();
					need_base=false;
					}
				if(is_pow[j]) p[k++]=arg[j]->arg[1]->copy();
				else p[k++]=new Number(1.0);
				}
			}
		
		if(k==1){
			q[i]=new Pow(base,p[0]);
			free (p);
			}
		else q[i]=new Pow(base,new Sum(p,k));
		}
	free(key);
	free(is_pow);
	return new Prod(q,s);//può ritornare una produttoria da una solo elemento
	}

Node* Node :: match(){return copy();}

			//SIMPLIFY

Node* Node:: simplify(){ // voglio usare solo questa
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->simplify();
	Node* simp = self(a,ord);
	Node* cl = simp->clean();
	if(typeid(*simp).name()!=typeid(*cl).name()){
		delete simp;
		return cl;
		}
	delete simp;
	
	Node* abs = cl->absorb();
	delete cl;
	
	Node* exp = abs->expand(); // expecting a sum but not always
	delete abs;
	
	Node** b=(Node**)malloc(exp->ord*sizeof(Node*));
	for(int i=0;i<exp->ord;i++) b[i]=exp->arg[i]->absorb();
	abs = exp->self(b,exp->ord);
	delete exp;
	
	Node** c=(Node**)malloc(abs->ord*sizeof(Node*));
	for(int i=0;i<abs->ord;i++) c[i]=abs->arg[i]->clean();
	Node* cul = abs->self(c,abs->ord);
	delete abs;
	
	cl=cul->clean();
	delete cul;
	
	Node* mat = cl->match();
	delete cl;
	
	Node** d=(Node**)malloc(mat->ord*sizeof(Node*));
	for(int i=0;i<mat->ord;i++) d[i]=mat->arg[i]->clean();
	cul = mat->self(d,mat->ord);
	delete mat;
	
	cl=cul->clean();
	delete cul;
	return cl;
	}

			//SELF

Node* Node :: self(Node** a,int b){return NULL;}		
Node* Sum  :: self(Node** a,int b){return new Sum(a,b);}
Node* Prod :: self(Node** a,int b){return new Prod(a,b);}
Node* Pow  :: self(Node** a,int b){return new Pow(a,b);}
Node* Exp  :: self(Node** a,int b){return new Exp(a,b);}
Node* Cos  :: self(Node** a,int b){return new Cos(a,b);}
Node* Sin  :: self(Node** a,int b){return new Sin(a,b);}
Node* Log  :: self(Node** a,int b){return new Log(a,b);}
Node* Number::self(Node** a,int b){return new Number(value);}
Node* X    :: self(Node** a,int b){return new X(a,b);}

Node** nalloc(int a){
	if (a<1) return NULL;
	else return (Node**)malloc(a*sizeof(Node*));
	}
