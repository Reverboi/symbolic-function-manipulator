#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;
#define h 2e-14
class Node{
	private:
	
	public:
		int ord; //number of arguments
		Node** arg; //array containing the adresses of its arguments
	Node ();
	Node (Node**,int);
	Node (Node*);
	Node (Node*,Node*);
	//virtual Node (Node*,double);//shift constructor for the fd
	~Node();//destruct
	virtual Node* copy();//deep copy
	//virtual Node* shift_copy(double);
	virtual Node* omit(int);
	//virtual Node* set_fd();
	virtual Node* set_ad();
	virtual Node* clean()=0;
	int size();
	virtual void text(); 
	virtual double eval(double x);
	bool compare(Node*);
	bool just_numbers();
	virtual Node* fuse(Node*);
	//virtual double get_value()=0;
	};
	
class Number: public Node{
	
	public:
	
	double value;
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Number(double a);//:Node(){value=a;}//calls default node contructor
	void text();
	double eval(double);
	Node* copy();
	Node* omit(){return 0;};
	Node* fuse(){return 0;};
	//Node* shift_copy();
	//double get_value();
	};

class Sum: public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Sum(Node* a,Node* b) : Node(a,b){};
	Sum(Node** a,int b) : Node(a,b){};
	Node* omit(int a);
	Node* fuse(Node*);
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Minus: public Node{
	public:
	Minus(Node* a) : Node(a){};
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Prod: public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Prod(Node* a,Node* b) : Node(a,b){};
	Prod(Node**a,int b) : Node(a,b){};
	Node* omit(int a);
	Node* fuse(Node*);
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Exp:public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Exp(Node* a) : Node(a){};
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Log:public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Log(Node* a) : Node(a){};
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Cos:public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Cos(Node* a) : Node(a){};
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Sin:public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Sin(Node* a) : Node(a){};
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class Pow:public Node{
	public:
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	Pow(Node* a,Node* b) : Node(a,b){};
	void text();
	double eval(double);
	Node* copy();
	//Node* shift_copy();
	};
class X:public Node{
	public:
	X():Node(){};
	~X();
	Node* clean();
	Node* set_ad();
	//Node* set_fd();
	//Node* shift_copy(double);
	void text();
	double eval(double);
	Node* copy();
	Node* fuse(){return 0;};
	Node* omit(){return 0;};
	};
	
Node* strton (char*,int);
int  checkfor(char* s, int m, char l);

