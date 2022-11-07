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

Number :: Number(double b){
	ord=0;
	arg=NULL;
	value=b;
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
	
bool Node::just_numbers(){
	for(int i=0;i<ord;i++){
		if(typeid(*arg[i]).name()!=typeid(Number).name()) return 0;
		}
	return 1;
	}

Node* Node::fuse(int a){
	if ((a<0)||(a>=ord)){
		cout<<"fuse out of bounds"<<endl;
		return NULL;
		}
	Node* d=omit(a);
	for(int i=0;i<arg[a]->ord;i++){
		Node* s=d->append(arg[a]->arg[i]);
		delete d;
		d=s;
		}
	return d;
	}
	
Node* Node::append(Node* s){
	int k=ord+1;
	Node** a=nalloc(k);
	for (int i=0;i<ord;i++) a[i]=arg[i]->copy();
	a[ord]=s->copy();
	return self(a,k);
	}

			//CLEAN

Node* Sum::clean(){
	Node** c=nalloc(ord);
	for(int i=0;i<ord;i++) c[i]=arg[i]->clean();//recursion
	Node* cl= new Sum(c,ord);
	for(int i=0;i<cl->ord;i++){
		if(typeid(*(cl->arg[i])).name()==typeid(Number).name()){// elimino gli zero
			if((static_cast<Number*>(cl->arg[i]))->value==0.0){
				Node* s=cl->omit(i--);//...!!
				delete cl;
				cl=s;
				}
			}
		}
	if(cl->ord==0){
		Node* s=new Number(0.0);
		delete cl;
		cl=s;
		}
	if(cl->ord==1){
		Node* s=cl->arg[0]->copy();
		delete cl;
		cl=s;
		}
	return cl;
	}
	
Node* Prod::clean(){
	Node** c=nalloc(ord);
	int n_minus=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Minus).name()){
			n_minus++;
			c[i]=arg[i]->arg[0]->copy();//!recursion
			}
		else c[i]=arg[i]->copy();//!recursion
		}
	Node** d=nalloc(ord);
	for(int i=0;i<ord;i++){
		d[i]=c[i]->clean();
		delete c[i];
		}
	free(c);
	Node* cl=new Prod(d,ord);
	for(int i=0;i<cl->ord;i++){
		if(typeid(*(cl->arg[i])).name()==typeid(Number).name()){
			if((static_cast<Number*>(cl->arg[i]))->value==0.0){
				delete cl;
				return new Number(0.0);
				}
			if((static_cast<Number*>(cl->arg[i]))->value==1.0){
				Node* s=cl->omit(i--);
				delete cl;
				cl=s;
				}
			}
		}
	if(cl->ord==0){
		Node* s=new Number(1.0);
		delete cl;
		cl=s;
		}
	if(cl->ord==1){
		Node* s=cl->arg[0]->copy();
		delete cl;
		cl=s;
		}
	if(n_minus%2==0) return cl;
	else return new Minus(cl);
	}

Node* Minus::clean(){
	Node* cl=arg[0]->clean();//recursion
	if(typeid(*cl).name()==typeid(Number).name()){		//-0 = 0
		if((static_cast<Number*>(cl))->value==0.0){
			return cl;// barely cheating, barely
			}
		}
	else if(typeid(*cl).name()==typeid(Minus).name()){	//-(-())=()
		Node* s=cl->arg[0]->copy();
		delete cl;
		return s;
		}
	else if(typeid(*cl).name()==typeid(Sum).name()){
		int k=cl->ord;
		Node** a=nalloc(k);
		for(int i=0;i<k;i++) a[i]=new Minus(cl->arg[i]->copy());
		delete cl;
		return new Sum(a,k);
		}
	return new Minus(cl);
	}

Node* Cos::clean(){
	Node* cl=arg[0]->clean();//recursion
	if(typeid(*cl).name()==typeid(Number).name()){
		if(static_cast<Number*>(cl)->value==0.0){
			delete cl;
			return new Number(1.0);
			}
		}
	if(typeid(*(cl)).name()==typeid(Minus).name()){//il coseno è pari
		Node* s=cl->arg[0]->copy();
		delete cl;
		cl=s;
		}
	return new Cos(cl);
	}
	
Node* Sin::clean(){
	Node* cl=arg[0]->clean();//recursion
	if(typeid(*cl).name()==typeid(Minus).name()){//il seno è dispari
		Node* s=cl->arg[0]->copy();
		delete cl;
		return new Minus(new Sin(s));
		}
	if(typeid(*cl).name()==typeid(Number).name()){
		if(static_cast<Number*>(cl)->value==0.0){
			return cl;
			}
		}
	return new Sin(cl);
	}

Node* Exp::clean(){
	Node* cl=arg[0]->clean();//recursion
	if(typeid(*cl).name()==typeid(Number).name()){
		if(static_cast<Number*>(cl)->value==0.0){
			delete cl;
			return new Number(1.0);
			}
		}
	return new Exp(cl);
	}

Node* Pow::clean(){
	Node* a=arg[0]->clean();//recursion
	Node* b=arg[1]->clean();//recursion
	if(typeid(*a).name()==typeid(Number).name()){
		if((static_cast<Number*>(a))->value==1.0){
			delete b;
			return a;
			}
		if((static_cast<Number*>(a))->value==0.0){
			double q=0.0;
			if(typeid(*b).name()==typeid(Number).name()){
				if((static_cast<Number*>(b))->value==0.0) q=NAN;
				}
			delete a;
			delete b;
			return new Number(q);
			}
		}
	if(typeid(*b).name()==typeid(Number).name()){
		if((static_cast<Number*>(b))->value==0.0){
			delete a;
			delete b;
			return new Number(1.0);
			}
		if((static_cast<Number*>(b))->value==1.0){
			delete b;
			return a;
			}
		}
	return new Pow(a,b);
	}

Node* Log::clean(){
	Node* cl=arg[0]->clean();//recursion
	if(typeid(*cl).name()==typeid(Number).name()){
		if(static_cast<Number*>(cl)->value==1.0){
			delete cl;
			return new Number(0.0);
			}
		}
	if(typeid(*cl).name()==typeid(Exp).name()){
		Node* s=cl->arg[0]->copy();
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
		//cout<<pippo<<endl;
		return new Number(pippo);
		}
	
	if(m<6){
		cout<<'p'<<endl;
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
		cout<<'!'<<endl;
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
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i] = new Prod(omit(i),arg[i]->set_ad());
	return new Sum(a,ord);
	}

Node* Minus::set_ad(){return new Minus(arg[0]->set_ad());}
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

			//TEXT

void Number::text(){cout<<value;}

void X::text(){cout<<'x';}

void Minus::text(){
	cout<<"(-";
	arg[0]->text();
	cout<<')';
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

			//EVAL

double Number::eval(double x){return value;}
double X::eval(double x){return x;}
double Minus::eval(double x){return -arg[0]->eval(x);}//recursion

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
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->absorb();//recursion
	int n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(a[i])).name()==typeid(Sum).name()){
			n+=a[i]->ord;
			}
		else n++;
	}
	Node** g=nalloc(n);
	n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(a[i])).name()==typeid(Sum).name()){
			for(int j=0;j<a[i]->ord;j++){
				g[n++]=a[i]->arg[j]->copy();
				}
			}
		else g[n++]=a[i]->copy();
		delete a[i];
		}
	free(a);
	return new Sum(g,n);
	}

Node* Prod  :: absorb(){
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->absorb();//recursion
	int n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(a[i])).name()==typeid(Prod).name()){
			n+=a[i]->ord;
			}
		else n++;
	}
	Node** g=nalloc(n);
	n=0;
	for(int i=0;i<ord;i++){
		if(typeid(*(a[i])).name()==typeid(Prod).name()){
			for(int j=0;j<a[i]->ord;j++){
				g[n++]=a[i]->arg[j]->copy();
				}
			}
		else g[n++]=a[i]->copy();
		delete a[i];
		}
	free(a);
	return new Prod(g,n);
	}

Node* Pow :: absorb(){
	Node* a=arg[0]->absorb();//recursion
	Node* b=arg[1]->absorb();//recursion
	if(typeid(*a).name()==typeid(Pow).name()){
		Node* c=a->arg[0]->copy();
		Node* d=new Prod(a->arg[1]->copy(),b);
		delete a;
		return new Pow(c,d);
		}
	return new Pow(a,b);
	}
	
Node* Node :: absorb(){
	Node** a = nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->absorb();//recursion
	return self(a,ord);
	}
	
			//EXPAND

Node* Node  :: expand(){
	Node** a = nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->expand();//recursion
	return self(a,ord);
	}

Node* Minus::expand(){
	Node* a=arg[0]->expand();
	if(typeid(*a).name()==typeid(Sum).name()){
		auto k=a->ord;
		Node** b=nalloc(k);
		for(int i=0;i<k;i++){
			b[i]=new Minus(a->arg[i]->copy());
			}
		delete a;
		return new Sum(b,k);
		}
	return new Minus(a);
	}

Node* Prod::expand(){//very proud of this one
	Node** a=nalloc(ord);
	for(int i=0;i<ord;i++) a[i]=arg[i]->expand();//recursion
	int n=1;
	for(int i=0;i<ord;i++){
		if(typeid(*a[i]).name()==typeid(Sum).name()){
			n=n*a[i]->ord;
			}
		}
	Node*** c=(Node***)malloc(sizeof(Node**)*n);
	for(int i=0;i<n;i++) c[i]=nalloc(ord);
	int m=1;
	for(int j=0;j<ord;j++){
		if(typeid(*a[j]).name()==typeid(Sum).name()){
			for(int i=0;i<n;i++) c[i][j]=a[j]->arg[(i/m)%a[j]->ord]->copy();
			m=m*a[j]->ord;
			}
		else for(int i=0;i<n;i++) c[i][j]=a[j]->copy();
		delete a[j];
		}
	free(a);
	Node** b=nalloc(n);
	for(int i=0;i<n;i++) b[i] = new Prod(c[i],ord);
	return new Sum(b,n);
	}
	
Node* Pow  :: expand(){
	if(typeid(*(arg[0])).name()==typeid(Prod).name()){
		Node* g=arg[1]->expand();//recursion
		Node** a=nalloc(ord);
		for(int i=0;i<arg[0]->ord;i++) a[i]=new Pow(arg[0]->arg[i]->expand(),g->copy());//recursion
		delete g;
		return new Prod(a,arg[0]->ord);
		}
	else return new Pow(arg[0]->expand(),arg[1]->expand());//recursion
	}

/*
			//MATCH

Node* Sum  :: match(){
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->match();
	return new Sum(a,ord);
	}

Node* Prod :: match(){
	for(int i=0;i<ord;i++){
		if(typeid(*(arg[i])).name()==typeid(Sum).name()){
			Node** a=(Node**)malloc(arg[i]->ord*sizeof(Node*));
			Node* b=omit(i);
			for(int j=0;j<arg[i]->ord;j++) a[j]=b->append(arg[i]->arg[j])->match();
			return new Sum(a,arg[i]->ord);
			}
		}
	Node** a=(Node**)malloc(ord*sizeof(Node*));
	for(int i=0;i<ord;i++) a[i]=arg[i]->match();
	return new Prod(a,ord);
	}

Node* Pow  :: match(){
	Node* g=arg[1]->match();
	if(typeid(*(arg[0])).name()==typeid(Prod).name()){
		Node** a=(Node**)malloc(arg[0]->ord*sizeof(Node*));
		for(int i=0;i<arg[0]->ord;i++) a[i]=new Pow(arg[0]->arg[i]->match(),g);
		return new Prod(a,arg[0]->ord);
		}
	else return new Pow(arg[0]->match(),g);
	}

Node* Minus:: match(){return new Minus(arg[0]->match());}
Node* Exp  :: match(){return new Exp(arg[0]->match());}
Node* Cos  :: match(){return new Cos(arg[0]->match());}
Node* Sin  :: match(){return new Sin(arg[0]->match());}
Node* Log  :: match(){return new Log(arg[0]->match());}
Node* X    :: match(){return new X();}
Node* Number::match(){return new Number(value);}

Node* Node :: match(){return self(arg[0]->match());}
*/
			//SELF
			
Node* Node :: self(Node** a,int b){return NULL;}		
Node* Sum  :: self(Node** a,int b){return new Sum(a,b);}
Node* Prod :: self(Node** a,int b){return new Prod(a,b);}
Node* Pow  :: self(Node** a,int b){return new Pow(a,b);}
Node* Minus:: self(Node** a,int b){return new Minus(a,b);}
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
/*
Node* Node::omit(int){return 0;}
Node* Node::fuse(int a){return 0;}
Node* Node::append(Node* a){return 0;}
*/
