#include <iostream>
#include <math.h>
#include <cstring>
#include <string>
#include "symb.h"
using namespace std;

int main(int argc, char** argv) {
	if (argc > 2) {
    	printf("Usage:   %s <f(x)> \n", argv[0]);
    	printf("Example: %s 'sin(x)' \n", argv[0]);
    	exit(EXIT_FAILURE);
		}
	Node* f,* g;
	string in;
	char ctrl;
	if (argc==2) f = strton(argv[1],strlen(argv[1]));
	else{
		cout<<"Input a function: ";
		cin>>in;
		f = strton(const_cast<char *>(in.c_str()),in.size());
		}
	cout<<"Input an action (enter h for help): ";
	while(1){
		cin>>in;
		switch(in[0]){
			case 'i':
				cout<<"Input a new function f(x): ";
				cin>>in;
				delete f;
				f = strton(const_cast<char *>(in.c_str()),in.size());
				continue;
			case 's':
				g=f->clean();
				break;
			case 'd':
				for(int j=(in[1]==NULL?1:in[1]);j>0;j--){
					g=f->set_ad();
					delete f;
					f=g;
					}
				break;
			case 'e':
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
			"      e to evaluate"<<endl<<"      q to exit";
			}
		delete f;
		f=g;
		f->text();
		cout<<endl;
		}
	}
