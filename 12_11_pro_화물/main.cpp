#include<stdio.h>

typedef unsigned long long ll;
#define F(a,b,c)	for(register int a=b;a<=c;a++)

#define MAX 300
#define HASH (2*MAX+7) 
class Item {
public:
	ll high;
	ll low;
}item[MAX+1];

void getlong(char str[] , Item * ip) {
	char *e = str + 0;	
	ip->high = ip->low = 0; 
	char index = 0; 

	for(;*e !='\0' ; e++, index ++){
		if ( index <5) ip->high = (ip->high << 7) + *e - '0' + 1; 
		else ip->low = (ip->low<<7) + *e - '0' + 1;		
	}

}



#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

class Rect {
public:
	int sx, sy;
	int ex, ey;
	bool isoverlap (Rect *in){
		return !(sy > in->ey || sx > in->ex || in->sx > ex || in->sy > ey);
	}
	Rect overlap(Rect * in) {		

		Rect ret = { 0, }; 
		ret.sx = Max(ex, in->sx);
		ret.sy = Max(ey, in->sy); 
		ret.ex = Min(sx, in->ex); 
		ret.ey = Min(sy, in->ey); 		
		return ret; 
	}
};

class Node {
public: 
	Rect rt;
	int h; 
	Node * n, *p;
}node[MAX+1][MAX +1];

Node serohead[MAX + 1], serotail[MAX + 1]; 

Node rect[MAX]; 
int grc = 0; 
int ghc = 1; 
Node recthead, recttail; 

void addrect(int sx, int sy, int ex, int ey) {
	bool done = false; 
	Rect temp = { sx,sy,ex,ey };
	F(k, 1, MAX) {
		Node * cur = serohead[k].n; 
		while (cur != &serotail[k]) {
			if (cur->rt.isoverlap(&temp)) cur = cur->n; 
			else { // 빈자리를 찾으면 거기에 추가 하면 된다. 
				rect[grc++].rt = temp;

				F(i, sx, ex) {
					F(j, sy, ey) {
						node[i][j].h = ghc ++;						
					}
				}
				return; 
			}
		}
	}
}

void print() {
	F(i, 1, 10) {
		F(j, 1, 10) {
			printf("%02d ", node[i][j].h);
		}
		printf("\n"); 
	}
}

int main() {

	F(j, 1, MAX) {
		Node * cur = &serohead[j]; 
		F(i, 1, MAX) {
			cur->n = &node[i][j];
			node[i][j].rt = { i,j,i,j };
			node[i][j].h = 0; 
			node[i][j].p = cur; 

			cur = cur->n;
		}
		cur->n = &serotail[j];
		serotail[j].p = cur; 
	}
	recthead.n = &recttail;
	recttail.p = &recthead;


	print(); 
	addrect(1, 2, 2, 2); 
	print();


	char a[11] = "zzzzzzzzzz"; 
	char b[11] = "zzzzz";

	Item temp = { 0, }; 
	getlong(a, &temp); 
	int h = (temp.high%HASH + temp.low%HASH) % HASH; 

	while (item[h].high != 0) {
		h = (h + 1) % HASH;
	}
	item[h] = temp; 


	getlong(b, &temp);
	h = (temp.high%HASH + temp.low%HASH) % HASH;
	while (item[h].high != 0) {
		h = (h + 1) % HASH;
	}
	item[h] = temp;
	

	return 0;
}