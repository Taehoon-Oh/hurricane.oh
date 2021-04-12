#define EE 1

#if EE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define log(a,...)	printf("%10s[%03d] " a "\n",__FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c) for(register int a = b; a <= c; a++)
#define NF(a,b,c) for(register int a = b; a >= c; a--)
#define Max(a,b,c)	(((a)>(b))?(a):(b))
#define Min(a,b,c)	(((a)<(b))?(a):(b))

template <typename T> void swap(T &a, T&b) { T t = a; a = b; b = t; }
//template <typename T> void garolink(T &l, T &r) { l->n = r; r->p = l; }
#define garolink(l,r)	(l)->n=(r);(r)->p=(l)
template <typename T> void removelink(T&t) { t->p->n = t->n; t->n->p = t->p; }


#define MAX 101
#define SUB 10


class Item {
public:
	int val; // time <<15 + 3000 	
	Item *n, *p; 
}item[MAX+1];

Item head[MAX / SUB + 1], tail[MAX / SUB + 1];

void down(int idx, int cnt) {
	Item* temp = &tail[idx];
	
	F(i, 1, cnt) {
		temp = temp->p; 		
	}

	temp->p->n = &tail[idx]; 
	tail[idx].p = temp->p;

	head[idx].val -= cnt; 


	Item * end=temp; 
	F(i, 2, cnt) {
		end = end->n; 
	}
	// 해당 개수 만큼 빼냈다. 
	end->n = head[idx + 1].n; 
	head[idx + 1].n->p = end; 


	temp->p = &head[idx + 1]; 
	head[idx + 1].n = temp; 
	head[idx + 1].val += cnt; 

}

void insert(Item * s, Item * e, Item * in) { // in 값에는 다시 나올 값이 들어가 있어야 한다. 
	Item * temp = s->n; 
	while (temp!=e)
	{
		if (in->val > temp->val) {			
			break;
		}
		temp = temp->n; 
	}

	garolink(temp->p, in); 
	garolink(in, temp);
	//temp->p->n = in;
	//in->p = temp->p;

	//temp->p = in;
	//in->n = temp; 
}

void add(Item * ip) {
	
	Item * temp = ip; 
	F(i, 0, MAX / SUB) {
		if(head[i].val< SUB){
			insert(&head[i], &tail[i], temp); 
			head[i].val++;
			break;
		}
		else if ( tail[i+1].p->val < temp->val) {
			insert(&head[i], &tail[i], temp);
			head[i].val++;
			F(j, i, MAX / SUB) {
				if(head[j].val > SUB)	down(j,  head[j].val- SUB);
			}			
		}
		else continue; 
	}
}

void update (Item * ip) {
	Item * temp = ip; 

	while (temp -> p != 0) {
		temp = temp->p; 
	}
	temp->val--;
	removelink(ip); 
	add(ip); 
}

void print() {
	F(i, 0, MAX / SUB) {
		Item * s = head[i].n;
		printf("index[%d]:", i);
		while (s!= &tail[i])
		{
			printf(" %03d->", s->val);
			s = s->n; 
		}
		printf("\n");
	}

}
int main() {
	int N = MAX; 

	F(i, 0, MAX / SUB) {
		head[i].n = &tail[i]; 
		tail[i].p = &head[i];
	}

	int stime = GetTickCount();
	F(i, 0, MAX) {
		item[i].val = i;
		item[i].n = item[i].p = 0;  
		add(&item[i]);			
	}

	print();

	item[4].val = 200;
	update(&item[4]);

	print();
	

	log("time = %d", GetTickCount() - stime); 


	return 0;
}














