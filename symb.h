#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;
#define h 2e-14

class Node{
	private:
		char type;
		double value;
	public:
		Node* arg1;
		Node* arg2;
	Node ();
	Node (char);
	Node (char a,double b);
	Node (char,Node*);
	Node (char,Node*,Node*);
	Node (Node* s);	//copy constuctor (whole new tree) (aka deep copy)
	Node (Node*,double);//shift constructor for the fd
	Node(char a, double x, Node* b, Node* c);
	~Node();//destruct
	
	Node* set_fd();
	Node* set_ad();
	Node* clean();
	int size();
	void text(); 
	double eval(double x);
	bool compare(Node*);
	};
	
Node* strton (char*,int);
int  checkfor(char* s, int m, char l);

