#define EE 0

#if EE
//#include <stdio.h>
//#define log(a,...)	printf("%10s[%03d] " a"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define garolink(l,r) ((l)->n=(r); (r)->p=(l);)
template <typename T> void swap(T &a, T&b) { T t = a; a = b; b = t; }
typedef unsigned long long ll;

#define MAX 40000
#define HASH (MAX*2 +7)
#define AREA 10
#define MU 1000

int fmap[MU + 1][22];
int amap[MU + 1][AREA + 1];

#define GPID(a)	(a&((1<<30)-1))

class Tr {
public:
	ll val;  // price << 30	
	int area;
	int heapidx;
}tr[HASH];

class PQ {
public:

#define MAX_SIZE (MAX+1)

	Tr * heap[MAX_SIZE];
	int heapSize = 0;

	void heapInit(void)
	{
		heapSize = 0;
	}

	void up(int cur) {
		int current = cur;
		while (current > 0 && heap[current]->val < heap[(current - 1) / 2]->val)
		{
			Tr * temp = heap[(current - 1) / 2];
			heap[(current - 1) / 2] = heap[current];
			heap[current] = temp;

			heap[(current - 1) / 2]->heapidx = (current - 1) / 2;
			heap[current]->heapidx = current;

			current = (current - 1) / 2;
		}
	}

	int heapPush(Tr * value)
	{
		if (heapSize + 1 > MAX_SIZE)
		{
			//printf("queue is full!");
			return 0;
		}

		heap[heapSize] = value;
		value->heapidx = heapSize; 

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

			Tr * temp = heap[current];
			heap[current] = heap[child];
			heap[child] = temp;

			heap[current]->heapidx	= current;
			heap[child]->heapidx = child;

			current = child;
		}
	}

	int heapPop(Tr * &value)
	{
		if (heapSize <= 0)
		{
			return 0;
		}

		value = heap[0];
		heapSize = heapSize - 1;

		heap[0] = heap[heapSize];
		heap[0]->heapidx = 0;

		down(0); 		
		return 1;
	}

	void change(int idx) {

	}
	void del(int idx) {
		//ll pre = heap[idx]->val; 		
		heap[idx] = heap[--heapSize]; 
		heap[idx]->heapidx = idx; 
		//ll next = heap[idx]->val;
		up(idx); 
		down(idx); 
	}
}pq[AREA+1];

int N, M;
void init(int n, int m)
{
	N = n, M = m;
	log("+++ N=%d, M=%d", N, M);

	F(i, 1, N) {
		fmap[i][0] = 1; 
		fmap[i][1] = i; 
		F(j, 1, M) {
			amap[i][j] = 0;
		}
	}
	F(i, 1, M) {
		pq[i].heapInit(); 
	}
	F(i, 0, HASH - 1) {
		tr[i].val = 0;
		tr[i].area = 0; 
		tr[i].heapidx = 0; 
	}

	log("---\n");
}

void befriend(int u1, int u2)
{
	log("+++ u1 = %d, u2=%d", u1, u2);
	fmap[u1][++fmap[u1][0]] = u2; 
	fmap[u2][++fmap[u2][0]] = u1;

	log("---\n");
}

void add(int pid, int area, int price)
{
	log("+++ pid =%d, area=%d, price=%d", pid, area, price);

	int h = pid % HASH;

	while (tr[h].val != 0) {
		h = (h + 1) % HASH;
	}
	tr[h].val = ((ll)price << 30) + pid;
	log("val = %lld ", tr[h].val);
	tr[h].area = area;

	pq[area].heapPush(&tr[h]);

	

	log("--- h=%d, pq[%d].heap[%d]  heapSize = %d \n", h, area, tr[h].heapidx, pq[area].heapSize);
}

void reserve(int uid, int pid)
{
	log("+++ uid= %d , pid = %d", uid, pid);

	int h = pid % HASH;
	while (GPID(tr[h].val) != pid) {
		h = (h + 1) % HASH;
	}
	int a = tr[h].area; 

	amap[uid][a]++;

	log("area = %d heapidx = %d , val = %lld",a, tr[h].heapidx, tr[h].val); 

	pq[a].del(tr[h].heapidx); 

	log("---heapidx = %d \n", tr[h].heapidx);
}

int recommend(int uid)
{
	log("+++ uid=%d", uid);
	


	int alist[AREA + 1] = { 0,0,0,0,0,0,0,0,0,0,0 };
	int sort[AREA + 1] = { 0,1,2,3,4,5,6,7,8,9,10 };	
	F(i, 1, fmap[uid][0]) {
		int fid = fmap[uid][i]; 
		log("fid= %d", fid); 
		F(j, 1, M) {			
			alist[j] += amap[fid][j];
		}
	}

	F(i, 1, M) {
		F(j, i + 1, M) {
			if (alist[i] < alist[j]) {
				swap(alist[i], alist[j]); 
				swap(sort[i], sort[j]); 
			}
		}
	}

	int max = alist[1]; 
	log("max = %d", max); 
	ll ret = (ll)2000<<30; 
	for (register int i = 1; i<=M && alist[i]==max ; i++) {
		if (pq[sort[i]].heapSize) {
			 ret =Min(pq[sort[i]].heap[0]->val, ret);
		}
		if (ret == (ll)2000 << 30) max = alist[i + 1];
	}

	log("---ret = %lld\n", GPID(ret));
	return (int)GPID(ret);
}