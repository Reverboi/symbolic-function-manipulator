#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;
class Node{
	public:
		int ord; 	//number of arguments
		Node** arg; //array containing the adresses of its arguments
	Node ();
	Node (Node**,int);
	Node (Node*);
	Node (Node*,Node*);
	virtual Node* self(Node** a, int b);
	//virtual Node (Node*,double); //shift constructor for the fd
	~Node();//destructor
	virtual Node* copy();//deep copy
	//virtual Node* shift_copy(double);
	virtual Node* omit(int b);
	//virtual Node* set_fd();
	virtual Node* set_ad()=0;
	virtual Node* clean()=0;
	virtual Node* absorb();
	virtual Node* expand();
	//virtual Node* match()=0;
	virtual Node* append(Node*);
	int size();
	virtual void text()=0;
	virtual double eval(double x)=0;
	bool compare(Node*);
	friend bool operator==(const Node&,const Node&);
	bool just_numbers();
	virtual Node* fuse(int a);
	};

class Number: public Node{
	public:
	double value;
	Node* clean();
	Node* set_ad();
	Node* copy();
	Node* self(Node** a, int b);
	//Node* set_fd();
	Number(double a);//:Node(){value=a;}//calls default node contructor
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
	//Node* set_fd();
	Sum(Node* a,Node* b) : Node(a,b){};
	Sum(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Minus: public Node{
	public:
	Minus(Node* a) : Node(a){};
	Node* clean();
	Node* expand();
	Node* self(Node** a, int b);
	Node* set_ad();
	Minus(Node** a,int b) : Node(a,b){};
	//Node* set_fd();
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
	//Node* set_fd();
	Prod(Node* a,Node* b) : Node(a,b){};
	Prod(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Exp:public Node{
	public:
	Node* clean();
	Node* self(Node** a, int b);
	Node* set_ad();
	//Node* set_fd();
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
	//Node* set_fd();
	Log(Node* a) : Node(a){};
	Log(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Cos:public Node{
	public:
	Node* clean();
	Node* self(Node** a, int b);
	Node* set_ad();
	//Node* set_fd();
	Cos(Node* a) : Node(a){};
	Cos(Node** a,int b) : Node(a,b){};
	void text();
	double eval(double);
	};

class Sin:public Node{
	public:
	Node* clean();
	Node* set_ad();
	Node* self(Node** a, int b);
	//Node* set_fd();
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
	Node* self(Node** a, int b);
	Node* expand();
	//Node* set_fd();
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
	//Node* set_fd();
	void text();
	double eval(double);
	};

Node** nalloc(int);
Node* strton (char*,int);
int  checkfor(char* s, int m, char l);
