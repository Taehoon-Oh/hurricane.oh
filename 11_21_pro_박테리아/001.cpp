#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf(register int a=b;a<=c;a++)
#else
#define log(a,...)

#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void garolink(T &l, T& r) { l->p = r; r->p = l; }

template <typename T>
void swap(T &a, T& b) { T t = a; a = b; b = t; }

#define MAX 500

class Item {
public:
	int r;
	int c; 
	Item * n, *p; 

	Item operator - (Item in) {
		Item temp = { r - in.r, c - in.c }; 
		return temp; 
	};
	int operator != (Item in) {
		return (r == in.r && c == in.c); 
	}
	int operator < (Item in) {

		if (r == in.r) return c < in.c; 
		else return r < in.r; 

	}
}item[(MAX + 1)* (MAX + 1)];
int gic = 0; 

Item * newItem(int r, int c) {
	Item *ip = &item[gic++];
	ip->r = r;
	ip->c = c;
	return ip; 
}


class List {
public:
	Item itemhead, itemtail;
	int itemcnt;
	int patid; 
	List * n, *p;
}list[251*251];
int glc = 0; 

List * newList() {
	List * lp = &list[glc];
	lp->patid = glc++; 
	lp->itemhead.n = &lp->itemtail;
	lp->itemtail.p = &lp->itemhead;
	return lp; 
}

class GR {
public:
	List listhead, listtail;	
	int listcnt;
	int patcnt;
}gr[201];

#define MAX_SIZE 201

Item * heap[MAX_SIZE];
int heapSize = 0;

void heapInit(void);
int heapPush(Item * &value); 
int heapPop(Item * &value); 
int listmap[MAX + 1][MAX + 1] = { 0, };

int R, C;

int same(List  * left, List * right) {
	int ret = 1; 

	Item delta = { left->itemhead.n->r - right->itemhead.n->r , left->itemhead.n->c - right->itemhead.n->c  }; 	

	Item *l = left->itemhead.n; 
	Item *r = right->itemhead.n;

	while (l != &left->itemtail) {
		if (( *l - delta ) != (*r-delta) ) {
			ret = 0; 
			break; 
		}

		l = l->n; 
		r = r->n; 
	}

	return ret;
}
int patid; 
int  checknew (List * lp) {
	int ret = 1; 
	int itemcnt = lp->itemcnt; 

	List * listtemp = gr[itemcnt].listhead.n;
	List * listtail = &gr[itemcnt].listtail;

	while (listtemp != listtail)
	{
		if (same(listtemp, lp)) {
			garolink( lp, listtemp->n ); // ���� ������ �ڸ��� �־��ְ� �׷��� ���߿� ���ﶧ ���ϴ�. 
			garolink(listtemp, lp);
			lp->patid = listtemp->patid; // ���� ���� ������ id �� �� �־��ְ� 
			ret = 0; 
		}
		listtemp = listtemp->n;
	}

	if (ret == 1) { // �����Ϳ� �ٴ°� ���ٸ� 
		garolink(lp, gr[itemcnt].listhead.n); 
		listtemp = &gr[itemcnt].listhead; 
		garolink(listtemp, lp);
		gr[itemcnt].patcnt++; 
		lp->patid = patid++; 
	}
	gr[itemcnt].listcnt++; 
	return ret; 
}

void remove(int lid) {
	List * lp = &list[lid]; 
	int patid = lp->patid; 
	int itemcnt = lp->itemcnt; 

	List * listtemp = gr[itemcnt].listhead.n; 
	List * listtail = &gr[itemcnt].listtail;

	// ����Ʈ���� ���鼭 ���� ���� ã�Ƽ� ����� 
	// ���ﶧ �̰��� �ش� ������ ������ ������ �հ� �ڸ� Ȯ���ϸ� �ȴ�. 
	// ������ �����̶�� ���� ī���͸� ���δ�. 

	while (listtemp != listtail)
	{
		int temppatid = listtemp->patid; 
		if (listtemp == lp) {
			garolink(listtemp->p, listtemp->n); 
			if (listtemp->p->patid != temppatid && listtemp->n->patid != temppatid) {// ���� ������ patid ��� 
				gr[itemcnt].patcnt--; 
				// �̷��� ����ٴ� ���� �� ������� �߰��� �Ѵٴ� ������ �ִ�.
			}
			break;
		}
		listtemp = listtemp->n; 
	}

	Item * itemtemp = lp->itemhead.n;
	Item * itemtail = &lp->itemtail; 

	while (itemtemp != itemtail) {
		heapPush(itemtemp);
		itemtemp = itemtemp->n; 
	}
}

void add(int r, int c) {
	Item * ip = newItem(r, c);
	int uid = listmap[Max(1, r - 1)][c]; 
	int did = listmap[Min(R, r + 1)][c];
	int lid = listmap[r][Max(1, c - 1)];
	int rid = listmap[r][Min(C, c+1)];

	if (uid) { remove(uid); };
	if (did) { remove(did); };
	if (lid) { remove(lid); };
	if (rid) { remove(rid); };

	heapPush(ip); // �� �ڽŵ� �־�� �Ѵ�. 

	Item * ipop=0; 
	int itemcnt = 0; 
	List * lp = newList(); 
	Item * ihead = &lp->itemhead; 
	Item * itail = &lp->itemtail;
	

	while (heapPop(ipop)) { // �������� �ִٸ� 
		garolink(ihead, ipop); 
		ihead = ipop; 
		itemcnt++; 
	}
	garolink(ihead, itail); 
	lp->itemcnt = itemcnt; 
	checknew(lp); // ���⼭ ��� �׷쿡 ������ �ǰ� �ϰ� 

	ihead = lp->itemhead.n;
	itail = &lp->itemtail;
	while (ihead != itail)
	{
		listmap[ihead->r][ihead->c] = lp->patid; 
		ihead = ihead->n; 
	}	

	int ret = 0; 
	F(i, 1, 200) {
		if (gr[i].patcnt) {
			printf("gr[%d]= %d", i, gr[i].patcnt); 
			ret += gr[i].patcnt; 
		}
	}
	printf("grcnt =  %d", ret);


}

int main() {
	R = 10, C = 10; 
	patid = 0; 
	gic = glc = 0; 
	F(i, 0, 200) {
		gr[i].listhead.n = &gr[i].listtail; 
		gr[i].listtail.p = &gr[i].listhead;
		gr[i].listcnt = gr[i].patcnt = 0; 
	}

	add(1, 1); 
	add(1, 3); 
	add(1, 2); 	

	return 0; 

}



void heapInit(void)
{
	heapSize = 0;
}

int heapPush(Item * &value)
{
	if (heapSize + 1 > MAX_SIZE)
	{
		//printf("queue is full!");
		return 0;
	}

	heap[heapSize] = value;

	int current = heapSize;
	while (current > 0 && *heap[current] < *heap[(current - 1) / 2])
	{
		swap(heap[current], heap[(current - 1) / 2]);
		/*int temp = heap[(current - 1) / 2];
		heap[(current - 1) / 2] = heap[current];
		heap[current] = temp;*/

		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;

	return 1;
}

int heapPop(Item * &value)
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

		swap(heap[current], heap[child]);

		/*int temp = heap[current];
		heap[current] = heap[child];
		heap[child] = temp;*/

		current = child;
	}
	return 1;
}
