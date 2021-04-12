#define DDD 1

#if DDD
#include <stdio.h>
#define log(a,...) printf("%10s[%03d]:" a, __FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif 

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))

template <typename T>
void Swap(T &a, T&b) {
	T t = a; 
	a = b; 
	b = t; 
}

#define MP (40000*2 +1)

#define MAX_TABLE MP

typedef struct
{
	int key;	
}Hash;
Hash tb[MAX_TABLE];

unsigned long hash(int key)
{
	unsigned long hash = 5381;
	int c=key;

	hash = (((c << 5) + hash)) % MAX_TABLE;

	return hash % MAX_TABLE;
}
#define NFND -9999999
int find(int key)
{
	unsigned long h = hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key != 0 && cnt--)
	{
		if (tb[h].key == key)
		{
			tb[h].key = key; 
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFND;
}

int add( int  key)
{
	unsigned long h = hash(key);

	while (tb[h].key != 0)
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

class Pkg {
public: 
	int id; 
	int ix; 
	int p;
	int r;
	int a; 
	int operator < (Pkg pre) {
		if (p == pre.p) return id < pre.id; 
		else return p < pre.p; 
	}
};

Pkg pkg[MP + 1];
Pkg * newPkg(int pix, int pid, int price, int area) {
	Pkg * pp = &pkg[pix];
	pp->id = pid; 
	pp->ix = pix; 
	pp->p = price; 
	pp->r = 0;
	pp->a = area;
	return pp; 
}

#define MAX_SIZE 40000
class Area {
public:
	int id;


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
};

#define MU 1000
#define MA 10
Area area[MA + 1]; 

int fmap[MU + 1][MU + 1]; 
int rmap[MU + 1][MA + 1]; 


void init(int N, int M)
{
	F(i, 0, MU) {
		F(j, 0, MU) fmap[i][j] = 0, fmap[i][0] = 1, fmap[i][1] = i;
		F(j, 0, MA) {
			rmap[i][j] = 0;
		}
	}
	F(j, 0, MA) area[j].heapInit();

	F(i, 0, MP) pkg[i] = { 0, }, tb[i].key=0; 

}

void befriend(int i1, int i2)
{
	log("++++ id1 = %d, id2= %d	\n",i1, i2);

	int fcnt = ++fmap[i1][0]; 
	fmap[i1][fcnt] = i2; 
	fcnt = ++fmap[i2][0];
	fmap[i2][fcnt] = i1;

	log("----id %d , fcnt = %d \n",i1, fcnt);
}

void add(int pi, int a, int pr)
{
	log("++++ pid= %d, area = %d, price = %d \n", pi, a, pr);

	int h = add(pi); 

	Pkg * pp = newPkg(h, pi,pr, a)	; 

	Area * ap = &area[a]; 
	ap->heapPush(h);

	log("----area %d : size = %d \n", a, ap->heapSize);
}

void reserve(int ui, int pi)
{
	log("++++uid = %d, pid = %d \n",ui,pi);
	int ix = find(pi); 


	int aid = pkg[ix].a; 
	pkg[ix].r = 1; 


	int rcnt = ++rmap[ui][aid];

	Area * ap = &area[aid];
	int temp; 
	while (1) {
		if (ap->heapSize && pkg[ap->heap[0]].r == 1) ap->heapPop(&temp); 
		else break; 
	}
	log("----rmap [%d][%d]=	%d\n",ui,aid, rmap[ui][aid]);
}

void quickSort(int first, int last, int input[]);
int recommend(int ui)
{
	log("++++id = %d \n",ui);
	int fcnt = fmap[ui][0]; 
	log("fcnt = %d\n", fcnt); 
	int list[MA + 1] = { 0, }; 
	int qlist[MA + 1] = { 0, };
	int max = 0; 

	F(i, 1, fcnt) { // 모든 친구들에 대해사 각 지역별 예약수를 조사한다. 
		int fid = fmap[ui][i]; 
		F(j, 1, MA) { 
			list[j] += rmap[fid][j]; 
			//log("%d area rcnt = %d \n", j, list[j]); 
			if (list[j] > max) max = list[j]; 
		}  // 지역별 친구들 예약수를 다 합힌다 
	}
	if (max !=0) {
		F(i, 0, MA) qlist[i] = list[i]; // 복사 해두고 

		quickSort(1, 10, qlist);
	}

	F(i, 1, MA) {

		log("sort %d \n", qlist[i]);
	}

	Pkg ret = { 0, }; 
	ret.p = 1001; 
	for (int i = 10; i > 0;i--) {
		max = qlist[i]; 

		F(j, 1, MA) {// 
			
			if (list[j] == max && area[j].heapSize) {// max 로 예약한 지역에 대해서만 조사를 해야 한다. 

				int tix = area[j].heap[0];
				
				Pkg t = pkg[tix]; 
				if (t.id != 0) {
					log("ret.price = %d, pid = %d  vs  t.pritc=%d, t.pid = %d \n", ret.p, ret.id, t.p, t.id);
					if (t < ret) {
						ret = t;
					}
				}
			}
		}
		if (ret.id != 0)break; 
		else while (qlist[i - 1] == max) i--; 
	}


	log("----\n");
	return ret.id;
}

void quickSort(int first, int last,int input[])
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

		quickSort(first, j - 1,   input);
		quickSort(j + 1, last,   input);
	}
}