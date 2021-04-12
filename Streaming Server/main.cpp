#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif
#include <stdio.h>
#include <stdlib.h>
#define MAXLength (10000)
#define MAXServer (10)
#define MAXUser (10000)

static int seed = 3;

extern void init(int L, int N, int C, int axis[10]);
extern int remove_user(int uid);
extern int add_user(int uid, int axis);
extern int get_users(int sid);
static void init_(int L, int N, int C);
static bool run(int l, int c, int r);

int test, T;
#define MM 0

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return seed & 0xffFFF;
}
static int viewer[MAXUser];

static bool run(int l, int n, int c, int r)
{
	init_(l, n, c);

	for (int i = 0; i < r; i++)
		viewer[i] = -1;

	bool accepted = true;
	int usercnt = 0;
	int uid = 0;
	while (r--)
	{
		int chk = pseudo_rand() % 100;

		if (chk < 70)
		{
			if (usercnt == (n * c))
				continue;
			int axis = pseudo_rand() % l;
			viewer[uid] = axis;

			add_user(uid, axis);
			uid++;
			usercnt++;
		}
		else if (chk < 95)
		{
			while (true)
			{
				if (usercnt == 0)
				{
					r++;
					break;
				}
				int rid = pseudo_rand() % uid;
				if (viewer[rid] > -1)
				{
					viewer[rid] = -1;
					remove_user(rid);
					usercnt--;
					break;
				}
			}
		}
		else
		{
			int ans = -1;
			scanf("%d", &ans);
			int sid = pseudo_rand() % n;		

			int ret = get_users(sid); 
			if (test > MM) {
				if (ans != ret)
				{
					accepted = false;
					printf("ans = %d, ret = %d \n", ans, ret);
					exit(0);
				}
			}
		}
	}
	int ans = -1;
	scanf("%d", &ans);
	int sid = pseudo_rand() % n;
	if (test > MM) {
	int ret = get_users(sid);



		if (ans != ret) {
			accepted = false;
			printf("ans = %d, ret = %d \n", ans, ret);
			exit(0);
		}
	}

	return accepted;
}

static bool run2(int l, int n, int c, int r)
{
	init_(l, n, c);
	bool accepted = true;
	int op, sid, uid, axis, ans, ret;

	while (r--)
	{
		scanf("%d", &op);
		switch (op)
		{
		case 0:
			scanf("%d %d %d", &uid, &axis, &ans);
			if (test > MM) {
			ret = add_user(uid, axis);

			
				if (ret != ans)
					accepted = false;
			}
			break;
		case 1:
			scanf("%d %d", &uid, &ans);
			if (test > MM) {
			ret = remove_user(uid);
			
				if (ret != ans)
					accepted = false;
			}
			break;
		case 2:
			scanf("%d %d", &sid, &ans);
			if (test > MM) {
				ret = get_users(sid);
				if (ret != ans)
					accepted = false;
			}
			break;
		default:
			break;
		}
	}

	scanf("%d %d", &sid, &ans);
	if (test > MM) {
		ret = get_users(sid);
		if (ret != ans)
			accepted = false;
	}

	return accepted;
}
static void init_(int L, int N, int C)
{
	int axis[10];

	for (int i = 0; i < N; i++) {
		scanf("%d", &axis[i]);
	}
	init(L, N, C, axis);
}

int main()
{

	setbuf(stdout, NULL);
	//freopen("eval_input.txt", "r", stdin);

	scanf("%d", &T);
	for (test = 1; test <= T; test++)
	{
		int L, N, C, R;

		scanf("%d %d %d %d", &L, &N, &C, &R);
		bool ret = false;
		if (test <= 5)
			ret = run2(L, N, C, R);
		else
			ret = run(L, N, C, R);

		printf("#%d %d\n", test, ret == true ? 100 : 0);
	}
	return 0;
}

/*
#define MAXLength (10000)
#define MAXServer (10)

void init(int L, int N, int C, int axis[MAXServer])
{

}

int add_user(int uid, int axis)
{
	return 0;
}

int remove_user(int uid)
{
	return 0;
}

int get_users(int sid)
{
	return 0;
}

*/

