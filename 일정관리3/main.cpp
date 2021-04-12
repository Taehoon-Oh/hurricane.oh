#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include<stdlib.h>

typedef enum {
	INIT,
	ADD,
	DELETE,
	CHANGE,
	GETCOUNT
} COMMAND;

extern void init();
extern void addEvent(int uid, char ename[], int groupid);
extern int deleteEvent(int uid, char ename[]);
extern int changeEvent(int uid, char ename[], char cname[]);
extern int getCount(int uid);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int C; 

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 431345 + 2531999;
	return mSeed & 0x7FFFFFFF;
}

static void make_string(char str[], int seed)
{
	mSeed = seed;
	int length = 5 + pseudo_rand() % 10;
	for (int i = 0; i < length; ++i) {
		str[i] = 'a' + pseudo_rand() % 26;
	}
	str[length] = 0;
}

#define  MAX 1
#define TEST 0

static int run()
{
	int answer = 100;

	int cmd, p1, p2, p3, p4;
	char str1[15], str2[15];

	int N, ret, flag;
	scanf("%d %d %d", &cmd, &N, &flag);

	init();

	for (int i = 1; i < N; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case ADD:
			if (flag == 1) {
				scanf("%d %s %d", &p1, str1, &p3);
			}
			else {
				scanf("%d %d %d", &p1, &p2, &p3);
				make_string(str1, p2);
			}
			if ( i >=MAX )			addEvent(p1, str1, p3);
			break;

		case DELETE:
			if (flag == 1) {
				scanf("%d %s %d", &p1, str1, &p3);
			}
			else {
				scanf("%d %d %d", &p1, &p2, &p3);
				make_string(str1, p2);
			}
			if (i >= MAX) {
				ret = deleteEvent(p1, str1);
				if (TEST) {
					if (ret != p1) {
						printf("ret = %d , p1 = %d", ret, p1);
						exit(0);
						answer = 0;
					}
				}
			}
			break;

		case CHANGE:
			if (flag == 1) {
				scanf("%d %s %s %d", &p1, str1, str2, &p4);
			}
			else {
				scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
				make_string(str1, p2);
				make_string(str2, p3);
			}
			if (i >= MAX) {
				ret = changeEvent(p1, str1, str2);
				if (TEST) {
					if (ret != p4) {
						printf("ret = %d , p4 = %d", ret, p4);
						exit(0);
						answer = 0;
					}
				}
			}
			break;

		case GETCOUNT:
			scanf("%d %d", &p1, &p2);
			if (i >= MAX) {
				ret = getCount(p1);
				if (TEST) {
					if (ret != p2) {
						printf("ret = %d , p2 = %d", ret, p2);
						exit(0);
						answer = 0;
					}
				}
			}
			break;

		default:
			break;
		}
	}

	return answer;
}

int initcnt = 0;
int addEventcnt = 0;
int deleteEventcnt = 0;
int getCountcnt = 0;
int changeEventcnt;

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T;
	scanf("%d", &T);
	C = 0; 

	//initcnt = 0;
	//addEventcnt = 0;
	//deleteEventcnt = 0;
	//getCountcnt = 0;
	//changeEventcnt = 0; 

	for (int tc = 1; tc <= T; tc++) {
		C++; 
		printf("#%d %d\n", tc, run());
	}

	//printf("addEventcnt= %d\n", addEventcnt);
	//printf("deleteEventcnt= %d\n", deleteEventcnt);
	//printf("changeEventcnt= %d\n", changeEventcnt);
	//printf("getCountcnt= %d\n", getCountcnt);

	return 0;
}

/*
addEventcnt= 133355
deleteEventcnt= 12600
changeEventcnt= 2199
getCountcnt= 12586

//void mstrcpy(char dst[], const char src[]) {
//	int c = 0;
//	while ((dst[c] = src[c]) != 0) ++c;
//}

//int mstrcmp(const char str1[], const char str2[]) {
//	int c = 0;
//	while (str1[c] != 0 && str1[c] == str2[c]) ++c;
//	return str1[c] - str2[c];
//}

void init()
{
}

void addEvent(int uid, char ename[], int groupid)
{
}

int deleteEvent(int uid, char ename[])
{
	return 0;
}

int changeEvent(int uid, char ename[], char cname[])
{
	return 0;
}

int getCount(int uid)
{
	return 0;
}
*/
