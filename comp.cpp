#include <iostream>
#include <math.h>
#include <cstring>
#include <string>
#include "symb.hpp"
using namespace std;

int main() {
	Node* f,* g=NULL;
	string in1,in2;
	char ctrl;
	cout<<"Input a function: ";
	cin>>in1;
	cout<<"Input another function: ";
	cin>>in2;
	f = strton(const_cast<char *>(in1.c_str()),in1.size());
	g = strton(const_cast<char *>(in2.c_str()),in2.size());
	Node* h=f->clean();
	auto i=g->clean();
	delete f,g;
	f=h->absorb();
	g=i->absorb();
	cout<<"this should be one or true or whatev:.."<<(*g==*f)<<endl;
	delete f,g,h,i;
	}
