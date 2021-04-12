#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

extern void init(int N, int M);
extern void befriend(int uid1, int uid2);
extern void add(int pid, int area, int price);
extern void reserve(int uid, int pid);
extern int  recommend(int uid);

extern class Pkg pkg; 

#define INIT			100
#define BEFRIEND		200
#define ADD				300
#define RESERVE			400
#define RECOMMEND		500

int testcase;
int target = 7; 

#define MAX 8
int max;

static bool run()
{
	int L, N, M;
	bool okay = false;

	int pid, uid;
	int uid1, uid2;
	int area, price;

	int ans;
	int cmd, ret;

	scanf("%d", &L);

	for (int l = 0; l < L; ++l)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case INIT:
			scanf("%d %d", &N, &M);
			if (max < MAX ) continue;
			init(N, M);			
			okay = true;
			break;
		case BEFRIEND:
			scanf("%d %d", &uid1, &uid2);
			if (max < MAX) continue;
			befriend(uid1, uid2);
			
			break;
		case ADD:
			scanf("%d %d %d", &pid, &area, &price);
			if (max < MAX) continue;
			add(pid, area, price);
			break;
		case RESERVE:
			scanf("%d %d", &uid, &pid);
			if (max < MAX) continue;
			reserve(uid, pid);
			break;
		case RECOMMEND:
			scanf("%d %d", &uid, &ans);
			if (max < MAX) continue;
			ret = recommend(uid);
			if (ret != ans) {
				okay = false;
				printf("ret = %d, ans = %d \n", ret, ans); 
				exit(0);
			}
			break;
		}
	}

	return okay;
}

int main() {
	int TC, MARK;

	//freopen("sample_input.txt", "r", stdin);

	setbuf(stdout, NULL);
	scanf("%d %d", &TC, &MARK);

	int start = GetTickCount(); 

	for ( testcase = 1; testcase <= TC; ++testcase) {
		max = testcase; 
		int score = run() ? MARK : 0;
		printf("#%d %d\n", testcase, score);
	}

	printf("time = %d \n", GetTickCount()- start);

	return 0;
}

/*
void init(int N, int M)
{

}

void befriend(int uid1, int uid2)
{

}

void add(int pid, int area, int price)
{

}

void reserve(int uid, int pid)
{

}

int recommend(int uid)
{
	return 0;
}

*/