#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;
class Node{
	public:
		int ord; 	//number of arguments
		Node** arg; //pointer to the array containing the adresses of its arguments
	Node ();
	Node (Node**,int);
	Node (Node*);
	Node (Node*,Node*);
	virtual Node* self(Node** a, int b);
	~Node();//destructor
	virtual Node* copy();//deep copy
	virtual Node* omit(int b);
	virtual Node* set_ad()=0;
	virtual Node* clean();
	virtual Node* absorb();
	virtual Node* expand();
	virtual Node* simplify();
	//virtual Node* match()=0;
	virtual Node* append(Node*);
	int size();
	virtual void text()=0;
	virtual double eval(double x)=0;
	bool compare(Node*);
	friend bool operator==(const Node&,const Node&);
	virtual Node* match();
	};

class Number: public Node{
	public:
	double value;
	Node* clean();
	Node* set_ad();
	Node* copy();
	Node* self(Node** a, int b);
	Number(double a);//redundant with the base class
	Number(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	Node* self(double);
	};

class Sum: public Node{
	public:
	Node* clean();
	Node* set_ad();
	Node* self(Node** a, int b);
	Node* absorb();
	//Node* simplify();
	Sum(Node* a,Node* b) : Node(a,b){};
	Sum(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Prod: public Node{
	public:
	Node* clean();
	Node* set_ad();
	Node* self(Node** a, int b);
	Node* expand();
	Node* absorb();
	Node* match();
	Prod(Node* a,Node* b) : Node(a,b){};
	Prod(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Exp:public Node{
	public:
	Node* self(Node** a, int b);
	Node* set_ad();
	Exp(Node* a) : Node(a){};
	Exp(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Log:public Node{
	public:
	Node* clean();
	Node* set_ad();
	Node* self(Node** a, int b);
	Log(Node* a) : Node(a){};
	Log(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Cos:public Node{
	public:
	Node* self(Node** a, int b);
	Node* set_ad();
	Cos(Node* a) : Node(a){};
	Cos(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Sin:public Node{
	public:
	Node* set_ad();
	Node* self(Node** a, int b);
	Sin(Node* a) : Node(a){};
	Sin(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Pow:public Node{
	public:
	Node* absorb();
	Node* clean();
	Node* set_ad();
	//Node* simplify();
	Node* self(Node** a, int b);
	Node* expand();
	Pow(Node* a,Node* b) : Node(a,b){};
	Pow(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class X:public Node{
	public:
	X():Node(){};
	X(Node** a,int b) : Node(a,b){};
	~X();
	Node* clean();
	Node* self(Node** a, int b);
	Node* set_ad();
	void text();
	double eval(double);
	};

Node** nalloc(int);
Node* strton (char*,int);
int  checkfor(char* s, int m, char l);
