#define DBG 0

#if DBG 
#include <stdio.h>
#define log(...)	printf("%10s[%03d]:", __FUNCTION__, __LINE__); printf(__VA_ARGS__)

#else 

#define log(a,...)

#endif 

#define F(a,b,c)	for(register int a=b;a<=c;a++)	
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define MU 1000
#define MP 40000  // 총 4만개의 상품  add 함수 
#define MA 10
#define MPID	1000000000
#define MPRICE	1000
#define ML MP*100

#define MAX_TABLE (MP*2 +7)
#define NFND -9999999
#define MF 20


typedef struct
{
	int key; 
}Hash;
Hash tb[MAX_TABLE];

unsigned long hash(int key)
{
	unsigned long hash = 5381;

	hash = (((key << 5) + hash) ) % MAX_TABLE;


	return hash % MAX_TABLE;
}

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

int add(int key )
{
	unsigned long h = hash(key);

	while (tb[h].key != 0)
	{
		if (tb[h].key ==  key)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}	

	tb[h].key = key; 
	return h;
}


class PKG {
public:
	int pid; 
	int hid;
	int reserved;
	int price;
	int area; 
	int operator < ( PKG p)  {
		if (price == p.price) {
			if (pid< p.pid) {
				return 1;
			}
			else {
				return 0; 
			}
		}
		else {
			if (price < p.price) {
				return 1;
			}
			else {
				return 0; 
			}
		}
	}
};

PKG pkg[MAX_TABLE];
PKG * newPkg(int pid, int hid, int price, int area) {
	PKG*pp = &pkg[hid]; 
	pp->pid = pid;
	pp->hid = hid;
	pp->area = area; 
	pp->price = price; 
	pp->reserved = 0; 

	return pp; 
}

class USER {
public:
	int uid;
	int fcnt;
	int f[MF + 1]; 
	int acnt; 
	int area[MA + 1]; 
};
USER user[MU + 1]; 

class LINK {
public:
	PKG * pp;
	LINK * n;
	LINK * p; 
};

LINK link[ML + 1]; 
int glc = 0; 

LINK * newLink(PKG *pp) {
	LINK * lp = &link[glc++]; 
	lp->pp = pp; 
	lp->n = lp->p = 0; 

	return lp; 
}

#define MAX_SIZE MP*2 

class AINFO {
public:
	int cnt;
	LINK * last;
	LINK * start;



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
		while (current > 0 && pkg [heap[current] ]  < pkg [heap[(current - 1) / 2]] )
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
		while (1) {
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

			if (pkg[*value].reserved == 0) {
				break;
			}
		}
		return 1;
	}



};

AINFO ainfo[MA + 1]; 



void init(int N, int M)
{
	log("++++\n");

	F(i, 0, MAX_TABLE-1) pkg[i] = { 0, };
	F(i, 0, MU) user[i] = { 0, }, user[i].fcnt =1, user[i].f[0]=i; // 자기 자신을 친구 리스트에 넣어 둔다. 
	F(i, 0, glc-1) link[i] = { 0, };
	F(i, 0, MA) {
		ainfo[i].cnt = 0, ainfo[i].last = ainfo[i].start = 0;
		ainfo[i].heapSize = 0; 
	}
	F(i, 0, MAX_TABLE-1)	tb[i].key = 0; 
	
	glc = 0; 
	log("----\n\n");
}

void befriend(int uid1, int uid2)
{
	log("++++ uid1=%d, uid2=%d\n", uid1, uid2);

	int cnt = ++user[uid1].fcnt; 
	user[uid1].f[cnt-1] = uid2;

	log("uid %d : fcnt = %d", uid1, cnt);

	cnt = ++user[uid2].fcnt;
	user[uid2].f[cnt-1] = uid1;

	log("uid %d : fcnt = %d", uid2, cnt);

	log("----\n\n");
}

void add(int pid, int a, int price)
{
	log("++++ pid=%d, area = %d, price = %d	\n", pid, a, price);

	// 사용자별 link link 에 추가 한다. 
	int h = add(pid); 

	PKG * pp = newPkg(pid, h, price,a); 	
	
	
	// 지역별 정보 리스트에 추가하는데 PQ  로 넣으면 나중에 관리가 편하다. 
	LINK * lp = newLink(pp); 
	ainfo[a].cnt++; 
	if (ainfo[a].start == 0) ainfo[a].last = lp; 
	
	lp->n = ainfo[a].start; 
	if (ainfo[a].start) ainfo[a].start->p = lp; 
	ainfo[a].start = lp; 

	if (!ainfo[a].heapPush(h)) {
			log("push error \n");
	}
	log("area %d: heapSize =  %d\n",a, ainfo[a].heapSize );

	
	log("----\n\n");
}

void reserve(int uid, int pid)
{
	log("++++ uid = %d, pid = %d\n", uid, pid);

	// 사용자의 예약 지역 정보  update

	int h = add(pid); 
	if (h == NFND) return; 

	pkg[h].reserved = 1; 

	int cnt = user[uid].acnt++; 
	int a = pkg[h].area; 
	user[uid].area[a] ++;  // 해당 지역의 예약 숫자 증가 

	log("---- area %d \n\n", a);
}

int input[11];

void quickSort(int first, int last)
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

		quickSort(first, j - 1);
		quickSort(j + 1, last);
	}
}

int recommend(int uid)
{
	log("++++ uid=%d\n", uid);

	int rmap[11] = { 0, }; 
	F(i, 0, 10) rmap[i] = 0; 
	int max = 0; 

	int fcnt = user[uid].fcnt; 

	log("fcnt = %d\n", fcnt);
	// 친구 리스트를 구하고 
	F(i, 0, fcnt-1) {
		int fid = user[uid].f[i]; 
		log("fid = %d\n", fid);

		F(i, 1, 10) {
			rmap[i] += user[fid].area[i]; // 사용자별 지역 예약 합을 합한다 최대값을 구하기 우해서 

			if (rmap[i] > max) max = rmap[i]; 
		}//친구 들의 예약 지역의 합을 더하고 		
	}
	
	F(i, 0, 10) input[i] = rmap[i];

	quickSort(1, 10); 
	//최다 지역들중 최소 가격, 최소 pid 가진 것을 return 한다. 

	

	PKG ret = { 0, }; 
	ret.pid = 1000000000;
	ret.price = 2000; 

	for (int j = 10; j >= 1; j--) {
		max = input[j]; 
		

		F(i, 1, MA) {
			int top;

			if (rmap[i] != max) continue;

			if (ainfo[i].heapPop(&top) == -1) continue;

			log("pop pid =%d, price = %d \n", pkg[top].pid, pkg[top].price);

			if (pkg[top] < ret  ) ret = pkg[top];
			log("ret pid =%d, price = %d \n", ret.pid, ret.price);

			ainfo[i].heapPush(top); // 다시 넣어 야 한다. 

		}
		if (ret.pid == 1000000000) {
			while (input[j + 1] == max) j++; 
		}
		else break; 

	}


	log("----\n\n");
	return ret.pid;
}