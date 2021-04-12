#define EE 0

#if EE
#include <stdio.h>
#define log(a,...)	printf ("%10s[%03d]:" a "\n",__FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))

#define NFD  -9999999
#define MU 1000
#define MA 10
#define MP 40000
#define ML (MP*MU)
#define MAX_TABLE (MP*2 +7)
typedef unsigned long long ll; 

typedef struct
{
	ll key;
}_Hash;
_Hash tb[MAX_TABLE];

unsigned long _hash(ll c)
{
	ll hash = 5381;	
	hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	return hash % MAX_TABLE;
}

int add(ll key);
int find(ll  key);



class Pkg {
public: 
	int pid;
	int pix; 
	int a;
	int p; 
	int r;
	int operator < (Pkg pre) {
		if (p == pre.p) return pid < pre.pid; 
		else return p < pre.p; 
	}

}pkg[MAX_TABLE];
int gpc = 0; 
Pkg * newPkg(int pix, int pid, int area, int price) {
	Pkg * pp = &pkg[pix]; 
	pp->pid = pid;
	pp->pix = pix; 
	pp->a = area;
	pp->p = price; 
	pp->r = 0; 
	return pp; 
}

int glc = 0; 
class Link {
public:
	Pkg * pp; 
	Link *n; 
	Link *p; 
}link [ML +1];
Link * newLink(Pkg * pp) {
	Link * lp = &link[glc++]; 
	lp->pp = pp; 
	lp->n = lp->p = 0; 
	return lp; 
}

class List {
public: 
	int a[MA + 1]; 
	void operator + (List in) {
		F(i, 0, MA) a[i] += in.a[i]; 		
	}
	void operator = (List in) {
		F(i, 0, MA) a[i] = in.a[i];
	}
};

class User {
public: 
	int cnt; 
	Link head;
	Link tail;	
	List rlist; 
	void init() {
		cnt = 0; 
		head.n = &tail;
		tail.p = &head; 
		head.p = tail.n = 0; 
		rlist = { 0, }; 
	}
}user[MU+1];

class Area {
public: 
#define MAX_SIZE (MP+1)

	int heap[MAX_SIZE];
	int heapSize = 0;

	void heapInit(void)
	{
		heapSize = 0;
	}

	int heapPush(int value)
	{
		if (heapSize + 1 > MAX_SIZE)
		{
			//printf("queue is full!");
			return 0;
		}

		heap[heapSize] = value;

		int current = heapSize;
		while (current > 0 && pkg[heap[current]] < pkg[heap[(current - 1) / 2]])
		{
			int temp = heap[(current - 1) / 2];
			heap[(current - 1) / 2] = heap[current];
			heap[current] = temp;
			current = (current - 1) / 2;
		}

		heapSize = heapSize + 1;

		return 1;
	}

	int heapPop(int *value)
	{
		if (heapSize <= 0)
		{
			return -1;
		}

		*value = heap[0];
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
				child = pkg[heap[current * 2 + 1]] < pkg[heap[current * 2 + 2]] ? current * 2 + 1 : current * 2 + 2;
			}

			if (pkg[heap[current]] < pkg[heap[child]])
			{
				break;
			}

			int temp = heap[current];
			heap[current] = heap[child];
			heap[child] = temp;

			current = child;
		}
		return 1;
	}

}area[MA+1];


int fmap[MU + 1][MU + 1];
int rmap[MU + 1][MA + 1];

void init(int N, int M)
{
	log("++++"); 
	glc = gpc = 0; 

	F(i, 0, MAX_TABLE - 1) tb[i].key = MAX_TABLE, pkg[i] = { 0, };
	F(i, 0, MU) {
		user[i].init();
		F(j, 0, MU) fmap[i][i] = 0; 
		//F(j, 0, MA) rmap[i][j] = 0; 

		fmap[i][0] = 1, fmap[i][1] = i; 
	}

	gpc = glc = 0;

	F(i, 0, MA) area[i].heapInit(); 
	log("----");

}

void befriend(int i1, int i2)
{
	log("++++ i1=%d, i2=%d", i1, i2);
	int cnt = ++fmap[i1][0]; 
	fmap[i1][cnt] = i2; 

	cnt = ++fmap[i2][0];
	fmap[i2][cnt] = i1;
	log("---- %d: fcnt = %d,  %d added", i1, cnt, i2);
}

void add(int pi, int a, int pr)
{
	log("++++ pi=%d, a=%d , pr=%d", pi,a,pr);

	int h = add(pi);
	log("pi %d => h=%d", pi, h);
	Pkg * pp = newPkg( h , pi,  a,  pr);  // hash  번 인텍스의 값을 그대로 사용해야 합니다

	area[a].heapPush(h); 
	log("a=%d, heapcnt = %d", a, area[a].heapSize); 
	log("----");
	
}

void reserve(int ui, int pi)
{
	log("++++ ui=%d, pi=%d", ui, pi);

	//int cnt = ++rmap[ui][0]; 
	int h = find(pi); 
	log("pi %d => h=%d", pi, h);
	if (h == NFD) return; 

	int area = pkg[h].a; 
	pkg[h].r = 1; 
	log("pi %d => a=%d", pi, area);

	user[ui].rlist.a[area]++; 

	log("ui %d: 1(%d) 2(%d) 3(%d) 4(%d) 5(%d) 6(%d) 7(%d) 8(%d) 9(%d) 10(%d)",ui, user[ui].rlist.a[1], user[ui].rlist.a[2], user[ui].rlist.a[3], user[ui].rlist.a[4], user[ui].rlist.a[5], user[ui].rlist.a[6], user[ui].rlist.a[7], user[ui].rlist.a[8], user[ui].rlist.a[9], user[ui].rlist.a[10]);
	log("----");	
}

void quickSort(int first, int last, int input[]); 

int recommend(int ui)
{
	log("++++ ui=%d", ui);	

	List list = { 0 ,};
	// 친구리스트를 구한다. 
	int fcnt = fmap[ui][0]; 
	log("fcnt = %d", fcnt); 

	Pkg temp = { 0, }; 

	F(i, 1, fcnt) {
		int fid = fmap[ui][i];
		log("fid = %d", fid);
		list + user[fid].rlist; 
		log("1(%d) 2(%d) 3(%d) 4(%d) 5(%d) 6(%d) 7(%d) 8(%d) 9(%d) 10(%d)",list.a[1], list.a[2], list.a[3],list.a[4], list.a[5], list.a[6],list.a[7], list.a[8], list.a[9], list.a[10]);
	}
	List sort = list; 

	quickSort(1, 10, sort.a);
	log("1(%d) 2(%d) 3(%d) 4(%d) 5(%d) 6(%d) 7(%d) 8(%d) 9(%d) 10(%d)", sort.a[1], sort.a[2], sort.a[3], sort.a[4], sort.a[5], sort.a[6], sort.a[7], sort.a[8], sort.a[9], sort.a[10]);

	Pkg ret; 
	ret.p = 1001;
	ret.pid = 1000000001;
	
	NF(i, 10, 1) {
		int max = sort.a[i]; 

		int pop = 0;
		F(j, 1, 10) {
			if (max != list.a[j]) continue; 

			while (area[j].heapPop(&pop) != -1) {
				if (pkg[pop].r == 1) continue;
				if ( pkg[pop] < ret) ret = pkg[pop];

				area[j].heapPush(pop); 
				break; 
			}
		}
		
		if (ret.pid != 1000000001) break;

		while (i <= 10 && max == sort.a[i + 1]) i++; 
	}

	log("----");	
	return ret.pid;
}



int find(ll key)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key != MAX_TABLE && cnt--)
	{
		if (tb[h].key == key)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFD;
}

int add(ll key)
{
	unsigned long h = _hash(key);

	while (tb[h].key != MAX_TABLE)
	{
		if (tb[h].key == key)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	tb[h].key = key;
	return h;
}


void quickSort(int first, int last, int input[])
{
	int pivot;
	int i;
	int j;
	int temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (input[i] <= input[pivot] && i < last)
			{
				i++;
			}
			while (input[j] > input[pivot])
			{
				j--;
			}
			if (i < j)
			{
				temp = input[i];
				input[i] = input[j];
				input[j] = temp;
			}
		}

		temp = input[pivot];
		input[pivot] = input[j];
		input[j] = temp;

		quickSort(first, j - 1, input);
		quickSort(j + 1, last,input);
	}
}