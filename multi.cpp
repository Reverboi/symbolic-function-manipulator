#include <iostream>
#include <math.h>
#include <cstring>
#include <string>
#include "symb.hpp"
using namespace std;

int main(int argc, char** argv) {
	if (argc > 2) {
    	printf("Usage:   %s < f (x)> \n", argv[0]);
    	printf("Example: %s 'sin(x)'\n", argv[0]);
    	exit(EXIT_FAILURE);
		}
	Node* f,* g=NULL;
	string in;
	char ctrl;
	if (argc==2) f = strton(argv[1],strlen(argv[1]));
	else{
		cout<<"Input a function: ";
		cin>>in;
		f = strton(const_cast<char *>(in.c_str()),in.size());
		}
	g=f->copy();
	delete g;
	while(1){
		start:
		cout<<"Input: ";
		cin>>in;
		switch(in[0]){
			case 't':
				f->text();
				cout<<endl;
				continue;
			case 'i':
				cout<<"Input a new function f(x): ";
				cin>>in;
				delete f;
				f = strton(const_cast<char *>(in.c_str()),in.size());
				continue;
			case 's':
				g=f->simplify();
				break;
			case 'm':
				g=f->match();
				break;
			case 'c':
				g=f->clean();
				break;
			case 'd':
				g=f->set_ad();
				break;
			case 'a':
				g=f->absorb();
				break;
			case 'e':
				g=f->expand();
				break;
			case 'v':
				double r;
				cout<<"Insert x value: ";
				cin>>r;
				cout<<"f("<<r<<")="<<f->eval(r)<<endl;
				continue;
			case 'q':
				delete f;
				return 0;
			default:
			cout<<"Press i to insert a new function"<<endl<<
			"      s to simplify"<<endl<<"      d to derive"<<endl<<
			"      e to expand"<<endl<<"      a to absorb"<<endl<<
			"      v to evaluate"<<endl<<"      t to print"<<endl<<"      q to exit"<<endl;
			goto start;
			}
		delete f;
		f=g;
		cout<<endl;
		}
	}
