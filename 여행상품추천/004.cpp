
#define EE 0

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n", __FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif

template <typename T>
void swap(T &a, T& b) { T t = a; a = b; b = t;  }

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define NF(a,b,c)	for (register int a=b;a>=c;a--)
#define U 1000
#define A 10
#define P 100000

int fmap[U + 1][U + 1]; 
int rmap[U + 1][A + 1]; 
typedef unsigned long long ll; 
#define MAX_TABLE (2*P+7)
class Pkg {
public:
	int pid; 
	int price;
	int r;
	int h; 
	int a; 
	ll v; 
}pkg[MAX_TABLE];

Pkg * find( int key)
{
	unsigned long h = key % MAX_TABLE;
	int cnt = MAX_TABLE;

	while (pkg[h].pid != 0 && cnt--)
	{
		if (pkg[h].pid == key)
		{
			log("h=%d", h);
			return &pkg[h];
		}
		h = (h + 1) % MAX_TABLE;
	}
	log("no h!!!");
	return 0;
}

Pkg * add(int key)
{
	unsigned long h = key % MAX_TABLE;

	while (pkg[h].pid != 0)
	{
		if (pkg[h].pid == key)
		{
			log("h=%d", h);
			return &pkg[h];
		}

		h = (h + 1) % MAX_TABLE;
	}
	log("h=%d", h);
	pkg[h].pid = key; 
	pkg[h].h = h; 
	return &pkg[h];
}

class Area {
public:
	int a;
#define MAX_SIZE P

	Pkg * heap[MAX_SIZE];
	int heapSize = 0;

	void init(void)
	{
		heapSize = 0;
	}

	int push(Pkg * value)
	{
		if (heapSize + 1 > MAX_SIZE)
		{
			//printf("queue is full!");
			return 0;
		}

		heap[heapSize] = value;

		int current = heapSize;
		while (current > 0 && heap[current]->v < heap[(current - 1) / 2]->v)
		{
			Pkg * temp = heap[(current - 1) / 2];
			heap[(current - 1) / 2] = heap[current];
			heap[current] = temp;
			current = (current - 1) / 2;
		}

		heapSize = heapSize + 1;

		return 1;
	}

	int pop(Pkg *value)
	{
		if (heapSize <= 0)
		{
			return -1;
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
				child = heap[current * 2 + 1]->v < heap[current * 2 + 2]->v ? current * 2 + 1 : current * 2 + 2;
			}

			if (heap[current]->v < heap[child]->v)
			{
				break;
			}

			Pkg * temp = heap[current];
			heap[current] = heap[child];
			heap[child] = temp;

			current = child;
		}
		return 1;
	}
}area[A+1];


void init(int N, int M)
{
	log("++++");

	F(i, 0, MAX_TABLE-1) pkg[i] = { 0, };
	F(i, 1, A) area[i].init(); 

	F(i, 0, U) {
		F(j, 0, U) fmap[i][j] = 0;
		fmap[i][0] = 1, fmap[i][1] = i;
		F(j, 0, A) rmap[i][j] = 0; 
		
	}
	
	log("----");

}

void befriend(int u1, int u2)
{
	log("++++ u1=%d, u2=%d", u1, u2); 
	int fcnt = ++fmap[u1][0];
	fmap[u1][fcnt] = u2; 
	log("%d : cnt = %d, %d added", u1, fcnt, u2); 

	fcnt = ++fmap[u2][0];
	fmap[u2][fcnt] = u1;
	log("%d : cnt = %d, %d added", u2, fcnt, u1);

	log("----"); 
}

void add(int pi, int a, int pr)
{
	log("++++ pi= %d, a=%d, pr = %d", pi, a, pr);

	Pkg * pp = add(pi); 
	if (!pp) {
		log("fail add");
		return; 
	}
	pp->a = a; 
	pp->price = pr; 
	pp->pid = pi; 
	pp->v = (ll)pr << 30 | pi; 

	log("h = %d, a = %d, price = %d, pid = %d, v = %lld", pp->h, pp->a, pp->price, pp->pid, pp->v); 

	area[pp->a].push(pp); 
	log("a %d : h = %dcnt = %d", pp->a, area[pp->a].heap[0]->h  ,area[pp->a].heapSize);

	log("----");
}

void reserve(int ui, int pi)
{
	log("++++ui = %d, pi = %d", ui, pi);
	Pkg * pp = find(pi); 
	if (!pp) {
		log("fail find");
		return;
	}

	pp->r = 1; 
	log("pid %d, reserved %d", pp->pid, pp->r); 

	rmap[ui][pp->a] ++; 
	log("ui %d : area %d reserved", ui, pp->a); 
	log("----");
}

int recommend(int ui)
{
	log("++++ ui = %d", ui);

	int list[A + 1] = { 0, };
	
	// 친구리스트를 구한다. 
	int fnum = fmap[ui][0]; 
	log("fnum = %d,",fnum); 
	F(i, 1, fnum) {
		int fid = fmap[ui][i]; 
		log("fid = %d,",fid);
		
		//예약 지역별 순위를 구한다. 
		F(j, 1, A) list[j] += rmap[fid][j];
	}

	F(j, 1, A) log(" list %d : %d", j, list[j]); 

	Pkg * ans = 0; 
	int last = 0; 
	F(j, 1, A) {
		while (area[j].heapSize > 0 && area[j].heap[0]->r == 1)area[j].pop(ans); 
		if (area[j].heapSize == 0) continue;
		ans = area[j].heap[0]; 
		last = j; 
		log("ans : pid = %d v = %lld  , list[%d]= %d", ans->pid, ans->v, ans->a, list[ans->a]);
		break; 
	}

	Pkg * temp=0; 
	F(j, last + 1, A) {
		while (area[j].heapSize > 0 && area[j].heap[0]->r == 1)area[j].pop(temp);
		if (area[j].heapSize == 0) continue; 
		temp = area[j].heap[0];
		log("temp: pid = %d  v = %lld  , list[%d]= %d", temp->pid, temp->v, temp->a, list[temp->a]);
		log("list: ans %d r %d, temp %d r %d", ans->a, list[ans->a],  temp->a, list[temp->a]);

		if (list[ans->a] == list[temp->a] )
		{
			if (temp->v < ans->v)
			{
				ans = temp;
			}
		}
		else if (list[ans->a] < list[temp->a])
		{
			ans = temp; 
		}
		log("ans : pid = %d v = %lld  , list[%d]= %d\n", ans->pid, ans->v, ans->a, list[ans->a]);
	}

	// 값이 있는 지역을 기준으로 해서 하나 선정한다 

	log("----");
	return ans->pid;
}

