#include <stdio.h>

typedef struct _LIST {
	int i;
	int v;
	int u;
	int p; 
}LIST;

LIST list[10000]; 






int main() {


	list[9].i = 9;
	list[9].v = 92;
	list[9].u = 32;
	list[9].p = 7; 



	LIST &temp = list[9]; 

	temp.i++; 

	return 0; 


}