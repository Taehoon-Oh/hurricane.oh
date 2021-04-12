#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%s[%03d] " a "\n",__FUNCTION__, __LINE__, ##__VA_ARGS)
#else
#define log(a,...)
#endif 

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

#define MAX 10000
#define DMAX 50

typedef unsigned long long ll; 


class Item {
public:
	int id; 
	ll val; 
	bool done;
	int dnum;
	int drink[DMAX];
	int qid[DMAX];
	
}item[MAX+1];




class PQ {
public:
#define MAX_SIZE MAX

	int index; 
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

			heap[(current - 1) / 2]->qid[index] = (current - 1) / 2;
			heap[current]->qid[index] = current;

			current = (current - 1) / 2;
		}
	}

	int heapPush(Item * value)
	{
		if (heapSize + 1 > MAX_SIZE)
		{
			printf("queue is full!");
			return 0;
		}

		heap[heapSize] = value;
		heap[heapSize]->qid[index] = heapSize;

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

			heap[current]->qid[index] = current;
			heap[child]->qid[index] = child;

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
		heap[0]->qid[index] = 0; 

		down(0);
		
		return 1;
	}

	void change(int cur) {
		up(cur); 
		down(cur);
	}

	void remove(int cur) {

		heap[cur] = heap[--heapSize]; 
		up( cur); 
		down(cur);
	}

}pq[DMAX];


int main() {

	F(i, 0, 10) {
		item[i].id = i; 
		item[i].val = i;
		item[i].done = false;

		pq[1].index = 1; 
		pq[1].heapPush(&item[i]); 
	}

	item[4].val = 12;
	pq[1].change(item[4].qid[1]);

	//item[8].val = 1; 
	//item[10].done = true; 
	pq[1].remove(item[5].qid[1]);


	Item * cur = 0; 
	F(i, 0, 10) {
		while (pq[1].heapPop(cur)) {
			printf("id = %d, val = %lld\n", cur->id, cur->val); 
		}

	}


	return 0; 
}