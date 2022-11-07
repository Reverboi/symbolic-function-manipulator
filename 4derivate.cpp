#include <iostream>
#include <math.h>
#include <cstring>
#include "symb.hpp"
using namespace std;

int main(int argc,char** argv) {
	if (argc != 2) {
    	printf("Usage:   %s < f (x)> \n", argv[0]);
    	printf("Example: %s 'sin(x)' \n", argv[0]);
    	exit(EXIT_FAILURE);
		}

	Node* f = strton(argv[1],strlen(argv[1]));
	Node* g=f->clean();
	delete f;
	for(int i=0;i<4;i++){
		g->text();
		cout << endl;
		cout << i << "th derivative equals:\t" << g->eval(1) << endl << endl;
		f=g->set_ad();
		delete g;
		g=f->clean();
		delete f;
		f=g->clean();
		delete g;
		g=f->clean();
		}

	delete g;
	return 0;
	}
