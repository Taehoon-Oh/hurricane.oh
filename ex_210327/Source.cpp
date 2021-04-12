

#include <stdio.h>
#include <Windows.h>
#include <time.h>


#define LEN 10
#define MAX (LEN*2)

char a[LEN + 1] = { 0, };
char b[LEN+1] = { 0, };
int inta[LEN]; 
int intb[LEN];

int inta2[MAX];
int intb2[MAX];

int intc[MAX + 1]; 

#define Max(a,b)	(((a)>(b))?(a):(b))

void calc2(int * a, int * c ) {
	int res = 0;
	int up = 0;

	for (int i = 0; i < MAX; i++) {
		res = 0; 
		for (int j =Max( 0, i-LEN +1) ; j <= i/2; j++) {
			res += a[j] * a[i-j];
			if (j != (i - j)) res = res << 1; 
		}
		res += up; 
		c[i] = res & 0x1F; 
		up = res >> 5;		
	}
}

int main() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < LEN; i++) {		

		
		inta[i] = rand() % 32; 

		while (i == 0 && inta[i] == 0) {
			inta[i] = rand() % 32; 
		}

		if (inta[i] < 10) a[i] = '0' + inta[i]; 
		else a[i] = 'A' + inta[i] - 10; 

		intb[i] = rand() % 32;

		while (i == 0 && inta[i] == 0) {
			inta[i] = rand() % 32;
		}

		if (intb[i] < 10) b[i] = '0' + intb[i];
		else b[i] = 'A' + intb[i] - 10;
		
	}
	//inta[0] = inta[1] = inta[2] = 1; 
	//intb[0] = intb[1] = intb[2] = 1;

	calc2(inta, inta2); 
	calc2(intb, intb2);

	return 0; 
}