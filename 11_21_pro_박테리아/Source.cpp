#include <stdio.h>


class Item {
public:
	int r;
	int c;
	Item *n, *p;
	int operator < (Item in) {
		if (r == in.r) return c > in.c;
		else return r > in.r;
	}
	Item operator - (Item in) {
		Item temp = { r - in.r, c - in.c };
		return temp;
	}

	int operator == (Item in) {
		return (r == in.r && c == in.c);
	}

}item[501 * 501];

int gic = 0;

class List {
public:
	Item head, tail;
	int cnt;
	int lid;
	List * n, *p;
	int operator ==(List in) {
		int ret = 1;
		Item * left = head.n;
		Item * right = in.head.n;
		for (int i = 1; i <= cnt; i++) {
			Item lbase = *left - *head.n;
			Item rbase = *right - *in.head.n;

			if (lbase == rbase) {
				left = left->n;
				right = right->n;
			}
			else {
				ret = 0;
				break;
			}

		}
		return ret;
	}
}list[251 * 251];

int glc = 0;


#define MAX_SIZE 100

Item * heap[MAX_SIZE];

int heapSize = 0;

void heapInit(void)
{
	heapSize = 0;
}

template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t; }

template <typename T>
void garolink(T &l, T &r) { l->n = r; r->p = l; }

int heapPush(Item * value)
{
	if (heapSize + 1 > MAX_SIZE)
	{
		printf("queue is full!");
		return 0;
	}

	heap[heapSize] = value;

	int current = heapSize;
	while (current > 0 && * heap[current] < *heap[(current - 1) / 2])
	{

		swap(heap[current], heap[(current - 1) / 2]);

		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;

	return 1;
}

int heapPop(Item  * &value)
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

		current = child;
	}
	return 1;
}


class GR {
public:
	List head, tail;
	int cnt;
}gr[201];


int listmap[501][501] = { 0, };

#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))




int checknew(List * &lp) {
	int ret = 1;
	int lid = lp->lid;
	int cnt = lp->cnt;


	List * ghead = gr[cnt].head.n;
	List * gtail = &gr[cnt].tail;

	while (ghead != gtail)
	{
		if (*ghead == *lp) {
			ret = 0;
			break;
		}
		ghead = ghead->n;
	}

	return ret;
}

int add(int r, int c) {
	int ret = 0;
	Item * ip = &item[gic++];
	ip->r = r, ip->c = c;
	heapPush(ip);

	int lid = listmap[r][Max(1, c - 1)];
	int rid = listmap[r][Min(10, c + 1)];
	int uid = listmap[Max(1, r - 1)][c];
	int did = listmap[Min(10, r + 1)][c];

	if (lid) {
		List * llp = &list[lid];

		//이전 점은 이제 리스트에서 뺀다
		List * ghead = gr[llp->cnt].head.n;
		List * gtail = &gr[llp->cnt].tail;

		while (ghead != gtail) {
			if (ghead == llp) {
				garolink(ghead->p, ghead->n);
				if (checknew(llp)) gr[llp->cnt].cnt--;
				break;
			}
			ghead = ghead->n;
		}

		Item * head = llp->head.n;
		Item * tail = &llp->tail;
		//이전 점들을 다 넣고
		while (head != tail)
		{
			heapPush(head);
			head = head->n;
		}

	}
	if (rid) {
		List * llp = &list[rid];

		//이전 점은 이제 리스트에서 뺀다
		List * ghead = gr[llp->cnt].head.n;
		List * gtail = &gr[llp->cnt].tail;

		while (ghead != gtail) {
			if (ghead == llp) {
				garolink(ghead->p, ghead->n);
				if (checknew(llp)) gr[llp->cnt].cnt--;
				break;
			}
			ghead = ghead->n;
		}

		Item * head = llp->head.n;
		Item * tail = &llp->tail;
		//이전 점들을 다 넣고
		while (head != tail)
		{
			heapPush(head);
			head = head->n;
		}
	}
	if (uid) {
		List * llp = &list[uid];

		//이전 점은 이제 리스트에서 뺀다
		List * ghead = gr[llp->cnt].head.n;
		List * gtail = &gr[llp->cnt].tail;

		while (ghead != gtail) {
			if (ghead == llp) {
				garolink(ghead->p, ghead->n);
				if (checknew(llp)) gr[llp->cnt].cnt--;
				break;
			}
			ghead = ghead->n;
		}

		Item * head = llp->head.n;
		Item * tail = &llp->tail;
		//이전 점들을 다 넣고
		while (head != tail)
		{
			heapPush(head);
			head = head->n;
		}
	}
	if (did) {
		List * llp = &list[did];

		//이전 점은 이제 리스트에서 뺀다
		List * ghead = gr[llp->cnt].head.n;
		List * gtail = &gr[llp->cnt].tail;

		while (ghead != gtail) {
			if (ghead == llp) {
				garolink(ghead->p, ghead->n);
				if (checknew(llp)) gr[llp->cnt].cnt--;
				break;
			}
			ghead = ghead->n;
		}

		Item * head = llp->head.n;
		Item * tail = &llp->tail;
		//이전 점들을 다 넣고
		while (head != tail)
		{
			heapPush(head);
			head = head->n;
		}
	}

	List * lp = &list[glc++];
	lp->lid = glc - 1;

	Item * head = &lp->head;
	Item * tail = &lp->tail;

	Item * itemp = 0;
	while (heapPop(itemp)) {
		garolink(head, itemp);
		head = itemp;
		listmap[itemp->r][itemp->c] = lp->lid;
		lp->cnt++; //  실제 개수를 카운트 한다.
	}
	garolink(head, tail);

	//개수별로 있는 것들 뒤진다.

	if (checknew(lp)) {
		gr[lp->cnt].cnt++;
	}

	lp->n = gr[lp->cnt].head.n;
	gr[lp->cnt].head.n->p = lp;

	lp->p = &gr[lp->cnt].head;
	gr[lp->cnt].head.n = lp;

	for (int i = 1; i <= 200; i++) {
		ret += gr[i].cnt;
		if (gr[i].cnt) printf("gr[%d]: %d \n", i, gr[i].cnt);
	}

	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++) {
			printf("%02d ", listmap[i][j]);
		}
		printf("\n");
	}
	return ret;
}

int main() {
	heapInit();
	gic = glc = 1;
	for (int i = 1; i <= 200; i++) {
		gr[i].head.n = &gr[i].tail;
		gr[i].tail.p = &gr[i].head;
		gr[i].cnt = 0;
	}

	printf(" group =  %d \n", add(1, 1));
	printf(" group =  %d \n", add(3, 3));
	printf(" group =  %d \n", add(3, 2));
	printf(" group =  %d \n", add(1, 2));

	printf(" group =  %d \n", add(6, 6));
	printf(" group =  %d \n", add(6, 7));
	printf(" group =  %d \n", add(6, 8));

	printf(" group =  %d \n", add(1, 3));
	printf(" group =  %d \n", add(2, 3));


}