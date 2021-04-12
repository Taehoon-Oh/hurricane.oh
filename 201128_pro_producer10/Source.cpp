#define EE 1

#if EE
#include <stdio.h>
#define log(a,...) printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else 
#define log(a,...)
#endif
#define F(a,b,c)	for(register int a=b;a<=c;a++)

template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t; }

template <typename T>
void garolink(T &l, T &r) { l->n = r; r->p = l; }

template <typename T>
void garoadd(T &base, T &in) { garolink(base->p, in); garolink(in, base); }


typedef unsigned long long ll;
#define MAX 20000
#define HASH (MAX+7)

class Tr {
public:
	char name[11];
	ll val; 
	int point;
	Tr * n, *p; 

	void init() {
		val = 0; n = p = 0; point = 0; 
	}
	bool operator < (Tr in) {
		if (point == in.point) return val < in.val; 
		else return point > in.point; 
	}
}tr[HASH];

Tr trhead, trtail;

ll getlong(const char str[]) {
	ll val = 0;

	const char *s;
	s = str; 

	bool last = 0; 
	F(i, 1, 10) {
		if (last) {
			val = (val << 5); 
		}
		else {
			val = (val << 5) + *s - 'a' + 1;
		}
		s++; 
		if (*s == '\0') last = 1; 
	}
	return val; 
}

int add(ll val) {
	int h = val % HASH;
	while (tr[h].val != 0)
	{
		h = (h + 1) % HASH; 
	}
	return h; 
}

int find(ll val) {
	int h = val % HASH;
	while (tr[h].val != val)
	{
		h = (h + 1) % HASH;
	}
	return h;
}

#define MAX_SIZE 100

Tr * heap[MAX_SIZE];
int heapSize = 0;

void heapInit(void)
{
	heapSize = 0;
}
int heapPop(Tr *&value);
int heapPush(Tr * value);


void tradd(const char name[]) {
	ll val = getlong(name);
	int h = add(val); 
	tr[h].init(); 
	tr[h].val = val;
	const char *s = name; 
	int i = 0;
	while (*s != '\0')	{
		tr[h].name[i] = name[i];
		i++; 
		s++; 
	}
	tr[h].name[i] = '\0';	

	Tr *head = &trhead;
	Tr *cur = &tr[h]; 
	garoadd(cur, head->n); 
}
void get() {

	Tr *head = trhead.n;
	Tr *last = &trtail;

	while (head != last )
	{
		heapPush(head);
		head = head->n; 
	}

	int cnt = 1; 
	while (heapPop(head)) 
	{
		log("%03d: name=%10s, point = %03d, val = %lld", cnt++, head->name, head->point,head->val);
	}	
}

int vote(const char name[], int point) {
	ll val = getlong(name); 
	int h = find(val); 

	tr[h].point += point; 
	return 1; 
}

int main() {
	trhead.n = &trtail;
	trtail.p = &trhead;
	tradd("aaad");
	tradd("aaa"); 
	tradd("aaab");
	tradd("aaaa");
	tradd("b");

	get(); 

	vote("b", 100); 
	vote("aaad", 1);
	get();


}

int heapPush(Tr * value)
{
	if (heapSize + 1 > MAX_SIZE)
	{
		printf("queue is full!");
		return 0;
	}

	heap[heapSize] = value;

	int current = heapSize;
	while (current > 0 && *heap[current] < *heap[(current - 1) / 2])
	{
		Tr * temp = heap[(current - 1) / 2];
		heap[(current - 1) / 2] = heap[current];
		heap[current] = temp;
		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;

	return 1;
}

int heapPop(Tr *&value)
{
	if (heapSize <= 0)
	{
		return 0;
	}

	value = heap[0];
	heapSize = heapSize - 1;

	heap[0] = heap[heapSize];

	int current = 0;
	while (current * 2 + 1 < heapSize)
	{
		int child;
		if (current * 2 + 2 == heapSize)
		{
			child = current * 2 + 1;
		}
		else
		{
			child = *heap[current * 2 + 1] < *heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
		}

		if (*heap[current] < *heap[child])
		{
			break;
		}

		Tr * temp = heap[current];
		heap[current] = heap[child];
		heap[child] = temp;

		current = child;
	}
	return 1;
}
