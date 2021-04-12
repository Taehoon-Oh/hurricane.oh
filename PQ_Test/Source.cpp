#define EE 1

#if EE
#include <stdio.h>
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


#define MAX 65


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

#define MAX_SIZE MAX

Item * heap[MAX_SIZE];
int heapSize = 0;

void heapInit(void)
{
	heapSize = 0;
}
int heapPop(Item *&value);
int heapPush(Item * value);
void change(int i, int c); 
void del(int i);


int main() {
	F(i, 0, MAX - 1) {
		item[i].val = i; 
		heapPush(&item[i]);
	}
	Item * ip; 	
	//F(i, 0, MAX - 1) {
	//	heapPop(ip);
	//	log("ip->val=%d", ip->val); 
	//}
	//
	//F(i, 0, MAX - 1) {	
	//	heapPush(&item[i]);
	//}

	int idx = 0; 
	int order = 0; 

	//del(40);
	//del(0);

	NF(i, MAX - 1, 0) {		
		F(j, 0, heapSize- 1) {
			if (heap[j]->val == i) {
				log("%d th = %d :  index %d   , hid = %d", order++,heap[j]->val, j, heap[j]->hid);
				break; 
			}
		}
		
	}

	//item[79].val = 200; 



	//order = 0;
	//NF(i, MAX-1, 0) {
	//	F(j, 0, MAX - 1) {
	//		if (heap[j]->val == i) {
	//			log("%d th = %d :  index %d   , hid = %d", order++, heap[j]->val, j, heap[j]->hid);
	//			break;
	//		}
	//	}

	//}

	return 0; 
}


void up(int cur) {
	int current = cur;
	
	while (current > 0 && heap[current]->val > heap[(current - 1) / 2]->val)
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
	value->hid = heapSize;
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
			child = heap[current * 2 + 1]->val > heap[current * 2 + 2]->val ? current * 2 + 1 : current * 2 + 2;
		}

		if (heap[current]->val > heap[child]->val)
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
	heap[heapSize]->hid = 0; 

	down(0);
	
	return 1;
}

void change(int i, int c) {
	int pval = heap[item[i].hid]->val; 
	item[i].val = c; 

	if (heap[item[i].hid]->val > pval) up(item[i].hid);
	else down(item[i].hid);
}

void del(int i) {
	heap[i] = heap[--heapSize];
	heap[i]->hid = i; 

	up(i);
	down(i); 

}