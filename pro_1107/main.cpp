#define EE 1


#if EE
#include <stdio.h>
#define log(a,...) printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__, ##__VA__ARGS__)

#else 
#define log(a,...)

#endif
#define F(a,b,c)	for (register int a=b; a<=c; a++)
#define Max(a,b)	(((a)>(b))?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

//class Trie;
//Trie* root; 
//
//Trie trie[50000]; 
//int gtc = 0; 
//
//
//
//class Trie {
//public:
//	Trie* child[26];
//	int cnt;		
//};
//
//Trie* newTrie() {
//	Trie* tp = &trie[gtc++];
//	return tp;
//}




void init() {


}

void add(char str[]) {
	

}

int find(char str[]) {

	int ret = 0;

	return ret; 

}

int remove(char str[]) {

	int ret=0; 

	return ret; 
}


template <typename T>
void addlink(T& l, T& r) { l->n = r; r->p = l; }

class Cur {
public: 
	int x;
	int y; 
	int idx; 
}cur;

class Empty; 


int X = 5, Y = 4;

class Empty {
public: 
	int x; 
	int y; 
	Empty* n; 
	Empty* p; 
	Empty* u;
	Empty* d;
}empty[100][100];  

Empty ehead[100], etail[100];

Empty top, bottom;

int find(int idx, int idy) {
	/*Empty* sero = ehead[i].n;

	while (sero != bottom&)
	{
		if (temp->x >= cur.x && temp->y >= cur.y)
		{
			break;
		}
		temp = temp->n;
	}
	if (temp == &etail[i]) {
		temp = ehead[i].n;
	}
	cur.x = temp->x;
	cur.y = temp->y;
	addlink(temp->p, temp->n);*/

	int xx = idx; 
	Empty * sero = &ehead[xx];

	while (sero !=&bottom)
	{		

		Empty * garo = sero->n;

		while (garo != &etail[xx]) {
			if (garo->x >= idx && garo->y >= idy) break;
			garo = garo->n;
		}

		if (garo != &etail[xx]) {
			cur.x = garo->x, cur.y = garo->y; 
			printf("(cur.x = %d, cur.y = %d) \n", garo->x, garo->y);
			return 1; 	
		}
		sero = sero->d; 
	}

	return 0; 
}



int map[100][100];

void showall() {
	Empty* sero = top.d;
	int idx = 0;

	printf("\n");
	printf("\n");
	while (sero != &bottom) {

		Empty* garo = sero->n;

		while (garo != &etail[idx])
		{

			printf("(%d, %d) ", garo->x, garo->y);
			garo = garo->n;
		}
		printf("\n");
		idx++;
		sero = sero->d;
	}
}

int main() {

	Empty * up = &top; 

	F(i, 0, X-1) {
		Empty* pre = &ehead[i];
		up->d = pre; 
		pre->u = up; 
		up = pre; 
		F(j, 0, Y-1) {
			empty[i][j].x = i; 
			empty[i][j].y = j;

			empty[i][j].p = pre; 
			pre->n = &empty[i][j]; 
			pre = &empty[i][j];
		}
		pre->n = &etail[i];
		etail[i].p = pre;
	}

	up->d = &bottom; 
	bottom.u = up; 		

	showall(); 

	addlink(empty[1][1].p, empty[1][1].n); 
	addlink(empty[1][2].p, empty[1][2].n);
	addlink(empty[1][3].p, empty[1][3].n);
	addlink(empty[1][0].p, empty[1][0].n);

	addlink(empty[2][0].p, empty[2][0].n);

	addlink(empty[3][1].p, empty[3][1].n);
	addlink(empty[3][2].p, empty[3][2].n);
	addlink(empty[3][3].p, empty[3][3].n);
	addlink(empty[3][0].p, empty[3][0].n);

	showall();

	find(2, 0);
	find(3, 0);

	return 1; 
}
