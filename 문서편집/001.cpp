#define EE 1


#if EE 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else 
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b; a<=c; a++)

#define Max(a,b)	(((a)>(b)?(a):(b))
#define Min(a,b)	(((a)<(b)?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

template <typename T>
void addlink(T &l, T &r) {
	l->n = r; 
	r->p = l; 
}

#define M 100000
int gic = 0;

class Item {
public: 
	int ucnt; 
	int ulist[30];
	char c; 
	Item* n, * p; 
}item[M+1];


Item* newItem(char c) {
	Item* ip = &item[gic++];
	ip->c = c;
	ip->ucnt = 0;
	F(i, 0, 29) { ip->ulist[i] = -1; }
	return ip;
};



int W, L; 

Item head, tail; 

#define UM 1000
Item* umap[UM + 1]; 

Item* line[UM + 1]; 
Item top, bottom; 

void Init(int w, int l, char str[])
{
	//log("+++ w = %d, l = %d str = %s", w, l , str); 

	W = w; 
	L = l; 
	gic = 0; 

	head.n = &tail; 
	head.p = tail.p = 0; 
	tail.p = &head; 

	top.n = &bottom; 
	bottom.p = &top; 
	top.p = bottom.n = 0; 

	F(i, 0, UM - 1) {
		umap[i] = 0;
		line[i] = 0;
	}

	Item* left = &head; 
	Item* upper = &top; 

	int tcnt = 0; 
	int lcnt = 0; 


	F(i, 0, l - 1) {
		Item *ip = newItem(str[i]);
		
		if (tcnt == 0) {
			line[lcnt++] = ip;

			addlink(upper, ip);
			upper = ip;
			tcnt++; 
		}
		else { tcnt++; }

		if (ip->c == 'n' || tcnt ==W ) {
			tcnt = 0; 
		}


		addlink(left, ip); 
		left = ip; 
	}

	left->n = &tail; 
	tail.p = left;

	upper->n = &bottom; 
	bottom.p = upper; 

#if EE
	Item* ip = head.n; 
	tcnt = 0; 
	for (int i = 0; i < l; i++) {		
		printf("%c", ip->c); 
		tcnt++; 
		if (ip->c == '\n') {
			tcnt = 0; 
		}
		else if (tcnt == W) {
			printf("\n");
			tcnt = 0; 
		}
		ip = ip->n; 
	}

	Item* top  = line[0]; 
	while (line) {

	}

#endif 

	log("---\n");
}

void Edit(int id, int row, int col)
{
	log("+++ id = %d, row =%d, col = %d	", id, row, col);

	Item* ip = head.n; 

	
	int idx = row * W + col; 

	while (idx--) ip = ip->n; 

	log("ip->c =%c, ucnt = %d", ip->c, ip->ucnt); 

	int& ucnt = ip->ucnt; 
	ip->ulist[ucnt++] = id; 
	log("ip->c =%c, ucnt = %d , ulist[%d] ", ip->c, ip->ucnt, ip->ulist[ucnt]);

	umap[id] = ip; 

	log("---\n");
}

void Add(int id, int l, char str[])
{
	log("+++");

	Item * ip = umap[id]; 

	Item* left = ip->n;
	Item* right = ip->p; 

	log("ip=%c, left = %c, right = %c", ip->c, left->c, right->c); 


	F(i, 0, l - 1) {
		Item* np = newItem(str[i]);
		addlink(left, np);
		left = np;
	}
	addlink(left, ip); 

	log("---\n");
}

void Delete(int id, int length)
{
	log("+++");


	log("---\n");
}

void Substitute(int id, int length, char str[])
{
	log("+++");


	log("---\n");
}

int EditDone(int id)
{
	return 0;
}
