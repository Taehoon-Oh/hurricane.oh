#define EE 1

#if EE
#include <stdio.h>
#include <stdlib.h>
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
template <typename T> void garolink(T &l, T &r) { l->n = r; r->p = l; }


#define MAX 101
#define SUB 10

class Info {
public:
	int genre;
	int stime;
};



class Item {
public:
	int val; // time <<15 + 3000 
	int mid;
	int hid;
	Item *n, *p;
}item[MAX];

Item genhead[8][31], gentail[8][31];//head.val = 맨 처음 값을 가지고 있자. 
Item head[31], tail[31];

#define MAX_SIZE (SUB+1)

class PQ {
public:

	Item * heap[MAX_SIZE];
	int heapSize = 0;

	void heapInit(void)
	{
		heapSize = 0;
	}

	void up(int cur) {
		int current = cur;
		while (current > 0 && heap[current]->val < heap[(current - 1) / 2]->val)
		{
			Item * temp = heap[(current - 1) / 2];
			heap[(current - 1) / 2] = heap[current];
			heap[current] = temp;

			heap[(current - 1) / 2]->hid = (current - 1) / 2;
			heap[current]->hid = current;

			current = (current - 1) / 2;
		}
	}

	int heapPush(Item * value)
	{
		if (heapSize + 1 > MAX_SIZE)
		{
			//printf("queue is full!");
			return 0;
		}

		heap[heapSize] = value;
		heap[heapSize]->hid = heapSize;

		up(heapSize);
		

		heapSize = heapSize + 1;

		return 1;
	}
	void down(int cur) {
		int current = cur;
		while (current * 2 + 1 < heapSize)
		{
			int child;
			if (current * 2 + 2 == heapSize)
			{
				child = current * 2 + 1;
			}
			else
			{
				child = heap[current * 2 + 1]->val < heap[current * 2 + 2]->val ? current * 2 + 1 : current * 2 + 2;
			}

			if (heap[current]->val < heap[child]->val)
			{
				break;
			}

			Item * temp = heap[current];
			heap[current] = heap[child];
			heap[child] = temp;

			heap[current]->hid = current;
			heap[child]->hid = child;

			current = child;
		}
	}

	int heapPop(Item *&value)
	{
		if (heapSize <= 0)
		{
			return 0;
		}

		value = heap[0];
		heapSize = heapSize - 1;

		heap[0] = heap[heapSize];
		heap[0]->hid = 0; 

		down(0);

		
		return 1;
	}

	void remove(int idx) {
		heap[idx] = heap[--heapSize]; 
		heap[idx]->hid = idx; 
		up(idx);
		down(idx); 
	}
}pq[MAX / SUB + 1];


void print() {
	F(j, 0, MAX / SUB) {
		Item * ip = 0; 
		printf("\nindex [%d]:", j);
		while (pq[j].heapPop(ip)) {
			printf("%03d<-", ip->val); 
		}
		printf("\n");
	}
}

void add(Item * in ) {
	//Item * ip = in; 
	//F(j, 0, MAX / SUB) {
	//	if (pq[j].heapSize < SUB || pq[j].heap[0]->val < ip->val) {// 더 커서 여기에 넣어야 한다면 
	//		pq[j].heapPush(ip);

	//		if (pq[j].heapSize > SUB) {
	//			pq[j].heapPop(ip);
	//			continue;
	//		}
	//		else break;
	//	}
	//	else continue;
	//}

	Item * ip = in; 
	F(j, 0, MAX / SUB) {
		if (pq[j].heapSize < SUB || pq[j].heap[0]->val < ip->val) {
			pq[j].heapPush(ip); 

			if (pq[j].heapSize > SUB) pq[j].heapPop(ip); 
			else break; // 추가를 했으니 개수가 초과하지 않으면 멈춰야 한다. 
		}		
	}
}

void update(Item *ip, int val) {

	/*int idx = 0; 
	F(j, 0, MAX / SUB) {
		if (pq[j].heap[0]->val < ip->val) {
			idx = j;
			break; 
		}
	}
	pq[idx].remove(ip->hid);
	ip->val = val; 

	add(ip); */

	int idx = 0; 
	F(j, 0, MAX / SUB) {
		if (pq[j].heap[0]->val < ip->val) {
			idx = j; 
			break; // 몇번째 인덱스에 속한것이니 확인한다. 
		}
	}

	//골라진 인덱스를 가지고 
	pq[idx].remove(ip->hid); // 내 위치 자리를 없앤다. 

	ip->val = val; 
	add(ip); 
}

class Test {
public: 
};



int main() {
	int N = MAX;

	int stime = GetTickCount();
	F(i, 0, MAX - 1) {
		//item[i].val = ((i + 1) << 15) ;
		item[i].val = i;
		Item * ip = &item[i];
		add(ip); 
	}

	//item[4].val = 200; 
	update(&item[4], 200);

	print();

	log("time = %d", GetTickCount() - stime); 
	return 0;
}