#define EE 0


#if EE

#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)

#else 

#define log(a,...)

#endif

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))



#define ML (10000)
#define MS (10)
#define MC (1000)

int L=ML, N=MS, C=MC;

class Server {
public:
	int x;
	int cnum;
}server[MS];

class Map {
public:
	int id; 
	int dist; 
	int operator < (Map pre) {
		if (dist == pre.dist) return id < pre.id;
		else dist < pre.dist; 
	}
};

class User {
public:
	int x;
	int uid; 
	int far;	
	int sid;
	int del; 
	Map smap[MS]; 

	int operator < (User pre) {
		if (far == pre.far) return uid < pre.uid;
		else return far < pre.far;
	}

}user[ML];
int guc = 0;

User* newUser(int uid, int x) {
	User* up = &user[uid];
	up->x = x; 
	up->uid = uid; 
	up->del = 0; 
	return up; 
}

#define MAX_SIZE ML

User * heap[MAX_SIZE];
int heapSize = 0;

int heapPush(int id); 
int heapPop(User* value); 
void heapInit(void);
int cmap[ML];

void init(int l, int n, int c, int axis[MS])
{
	L = l, N = n, C = c; 
	log("+++ L=%d, N=%d, C = %d ", L, N, C); 
	guc = 0; 

	F(i, 0, N - 1) {
		server[i].x = axis[i]; 
		log("sid[%d] = %d ",i,  axis[i]);		
	}


	log("---"); 
}

void quickSort(int first, int last,User * up )
{
	int pivot;
	int i;
	int j;
	Map temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			
			while (up->smap[i] < up->smap[pivot] && i < last)
			{
				i++;
			}
			while (up->smap[pivot] < up->smap [j])
			{
				j--;
			}
			if (i < j)
			{
				temp = up->smap[i];
				up->smap[i] = up->smap[j];
				up->smap[j] = temp;
			}
		}

		temp = up->smap[pivot];
		up->smap[pivot] = up->smap[j];
		up->smap[j] = temp;

		quickSort(first, j - 1, up);
		quickSort(j + 1, last, up);
	}
}


void 	GetServerDist(User * up) {

	int temp[MS] = { 0, }; 

	int c = up->x; 
	int max = -1; 

	F(i, 0, N - 1) {
		int s = server[i].x; 
		up->smap[i].dist  = Min(Max(c, s) - Min(c, s), 10 + Min(c, s) - Max(c, s)); 
		up->smap[i].id = i; 

		if (up->smap[i].dist > max) {
			max = up->smap[i].dist;
		}
		log("c = %d, s=%d , d=%d", up->x, server[i].x, up->smap[i].dist);
	}
	up->far = max; 


	quickSort(0, N - 1, up); 

	return;
}

void serverInit() {
	F(i, 0, N - 1) {
		server[i].cnum = 0; 
	}

}

int ulist[ML]; 
int add_user(int uid, int x)
{
	log("+++ uid = %d, x = %d", uid, x);
	User* up = newUser(uid, x); 
	ulist[guc++] = uid; 
	up->del = 0; 
	int ret = 0; 

	// 힙 초기화 
	heapInit(); 
	serverInit(); 

	// 현재 사용자들로 다시 순서를 정한다. 
	F(i, 0, guc - 2) {

		if (user [ ulist[i]].del == 1)// removed 
		{
			continue; 
		}
		else heapPush(ulist[i]);
	}

	// 각 서버와 사용자간 거리를 구하고 user Q 를 초기화 하고 다시 넣어 준다. 
	// 
	GetServerDist(up);

	heapPush(uid); 

	F(i, 0, guc - 1) {		
		User top ={0,}	;
		heapPop(&top); 
		F(j, 0, N-1) {
			int sid = top.smap[j].id; 
			log("sid = %d, cnum = %d ", sid, server[sid].cnum); 
			if (server[sid].cnum < C) {
				server[sid].cnum++;
				top.sid = sid; 
				if (top.uid == uid) ret = sid; 
				break; 
			}
			else continue; 
		}
	}

	log("--- ret = %d", ret );
	return ret;
}

int remove_user(int uid)
{
	log("+++ uid = %d	", uid);
	int ret = user[uid].sid; 

	heapInit();
	serverInit();


	F(i, 0, guc - 1) {

		if (user[ulist[i]].del == 1)// removed 
		{
			continue;
		}
		else heapPush(ulist[i]);
	}

	guc--;


	F(i, 0, guc - 1) {
		User top = { 0, };
		heapPop(&top);
		F(j, 0, N - 1) {
			int sid = top.smap[j].id;
			log("sid = %d, cnum = %d ", sid, server[sid].cnum);
			if (server[sid].cnum < C) {
				server[sid].cnum++;
				top.sid = sid;
				if (top.uid == uid) ret = sid;
				break;
			}
			else continue;
		}
	}


	log("--- ret = %d", ret);
	return ret;
}

int get_users(int sid)
{
	log("+++ sid = %d	", sid);
	int ret = server[sid].cnum; 

	log("--- ret = %d	", ret);
	return 0;
}

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

	heap[heapSize] = &user[value];

	int current = heapSize;
	while (current > 0 && *heap[current] < *heap[(current - 1) / 2])
	{
		User *  temp = heap[(current - 1) / 2];
		heap[(current - 1) / 2] = heap[current];
		heap[current] = temp;
		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;

	return 1;
}

int heapPop(User* value)
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

		User *  temp = heap[current];
		heap[current] = heap[child];
		heap[child] = temp;

		current = child;
	}
	return 1;
}
