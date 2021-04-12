#define EE 0

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else 
#define log(a,...)
#endif
template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t; }
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))
#define F(a,b,c)	for(register int a =b;a<=c;a++)
#define NF(a,b,c)	for(register int a =b;a>=c;a--)

typedef unsigned long long ll; 

#define MAX 40000
#define HASH (80001)
#define MU 1000
#define MA 10

class Pkg {
public: 	
	ll score; // price <<30 | pid
	bool operator <(Pkg in) {
		return score < in.score; 
	}
}pkg[HASH];

int fmap[MU+1][MU+1];
int amap[MU+1][MA+1]; 


class PG{
public:

#define MAX_SIZE (MAX+1)

	Pkg * heap[MAX_SIZE];
	int heapSize = 0;

	void heapInit(void)
	{
		heapSize = 0;
	}

	int heapPush(Pkg * value)
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
			Pkg * temp = heap[(current - 1) / 2];
			heap[(current - 1) / 2] = heap[current];
			heap[current] = temp;
			current = (current - 1) / 2;
		}

		heapSize = heapSize + 1;

		return 1;
	}

	int heapPop(Pkg * &value)
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
				child = *heap[current * 2 + 1] < *heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
			}

			if (*heap[current] < *heap[child])
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
}pq[MA + 1]; 


int N, M; 

void init(int n, int m)
{
	N = n, M = m; 
	log("++++ N=%d, M=%d", N, M);

	F(i, 1, N) {
		fmap[i][fmap[i][0] = 1] = i;
		F(j, 1, M)amap[i][j] = 0;
	}

	F(j, 1, M)pq[j].heapInit();

	F(i, 0, HASH - 1) pkg[i].score = 0; 

	log("----");

}

void befriend(int u1, int u2)
{
	log("++++ u1=%d, u2=%d", u1, u2);
	fmap[u1][++fmap[u1][0]] = u2; 
	fmap[u2][++fmap[u2][0]] = u1;

	log("----");
}

void add(int pi, int a, int pr)
{
	log("++++ pi= %d, a=%d, pr = %d", pi, a, pr);
	ll score = ((ll)pr << 35) | ((ll)pi<<5) | (a<<1) ; 

	log("score %lld , pr= %lld, pi = %lld",score,  score >> 35 , (score >> 5)& ((1 << 30) - 1));
	int h = pi % HASH; 
	while (pkg[h].score != 0) {
		h = (h + 1) % HASH;
	}
	pkg[h].score = score;

	pq[a].heapPush(&pkg[h]); 

	log("----");
}

void reserve(int ui, int pi)
{
	log("++++ui = %d, pi = %d", ui, pi);
	int h = pi % HASH;
	while ( ((pkg[h].score >>5)&  ((1 << 30) - 1) )!= pi) {
		h = (h + 1) % HASH;
	}
	ll score = pkg[h].score = ( pkg[h].score | 1 ) ;
	log("h=%d, score = %lld , pi = % lld  a = %lld ", h,score, (score >> 5)&  ((1 << 30) - 1) , (score>>1) &  ((1 << 4) - 1));

	
	amap[ui][(score >>1 )& ((1 << 4) - 1)] ++; 

	log("----");
}


int recommend(int ui)
{
	log("++++ ui = %d", ui);
	int list[MA+1] = { 0, };
	int index[MA + 1] = { 0,1,2,3,4,5,6,7,8,9,10 }; 
	F(i, 1, fmap[ui][0]) {
		int fid = fmap[ui][i];
		F(j, 1, M) {
			list[j] += amap[fid][j]; 
		}
	}
	F(i, 1, M) {
		F(j, i + 1, M) {
			if (list[i] < list[j]) swap(list[i], list[j]), swap(index[i], index[j]); 
		}
		log("list[%d]=%d , index[%d]=%d ", i, list[i], i, index[i]); 
	}

	int cur = 1; 
	Pkg ret = { ((ll)1)<<45 };
	int max = list[1]; 
	while (max == list[cur] ) {
		while (pq[index[cur]].heapSize && pq[index[cur]].heap[0]->score & 1 ) {
			Pkg * temp = 0; 
			pq[index[cur]].heapPop(temp); 
		}

		if (pq[index[cur]].heapSize &&   pq[index[cur]].heap[0]->score < ret.score ) {
			//ret = (int)(pq[index[cur]].heap[0]->score >> 5)& ((1 << 30) - 1);
			ret.score = pq[index[cur]].heap[0]->score; 
		}

		cur++;
		if (cur > M) break; 
		if (ret.score == (((ll)1) << 45) ) max = list[cur];
	}
	
	max = (int)(ret.score >> 5)& ((1 << 30) - 1);
	log("---- pid = %d", max);
	return (int)(ret.score >> 5)& ((1 << 30) - 1);
}
