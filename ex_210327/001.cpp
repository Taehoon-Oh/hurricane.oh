#include <stdio.h>
#include <malloc.h>
#define log(a,...)	printf("%s[%d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)



#define MAXCHAR 15000

typedef unsigned long long ll;

ll llA[MAXCHAR / 5 + 1];
ll llB[MAXCHAR / 5 + 1];
ll llR[(MAXCHAR / 5) * 2 + 1];

void jcalc(char * chR, char * chA, char * chB)
{
	F(i, 0, MAXCHAR - 1) {
		llA[i / 5] = (llA[i / 5] << 5) + chA[i] - '0';
		llB[i / 5] = (llB[i / 5] << 5) + chB[i] - '0';
	}

	F(i, 0, MAXCHAR / 5 - 1) {
		F(j, 0, MAXCHAR / 5 - 1) {
			llR[i + j] += (llA[i] * llB[j]);
		}
		if (i % 15 == 14) {
			F(k, 0, i+j) {
				llR[i + j + 1] = llR[i + j] >> 30;
				llR[i + j] &= 0x3fffffff;
			}
		}
	}

	NF(i, MAXCHAR * 2 - 1, 1) {
		//llR[i + 1] += (llR[i] >> 5); 
		chR[i] = (llR[i / 5] & 0x1F) + '0';
		llR[i / 5] = (llR[i / 5] >> 5);
	}

}