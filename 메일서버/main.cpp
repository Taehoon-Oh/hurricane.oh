#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define INIT         0
#define SENDMAIL     1
#define GETCOUNT     2
#define DELETEMAIL   3
#define SEARCHMAIL   4

extern void init(int N, int K);
extern void sendMail(char subject[], int uID, int cnt, int rIDs[]);
extern int getCount(int uID);
extern int deleteMail(int uID, char subject[]);
extern int searchMail(int uID, char text[]);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#define MAX_WORD 10000
static char Word[MAX_WORD][11];

static int N, W, minR, maxR, SW;

#define MAX 7
int max; 

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 214013 + 2531011;
	return (mSeed >> 16) & 0x7FFF;
}

static void make_string(int seed)
{
	mSeed = seed;

	for (int i = 0; i < W; ++i) {
		int length = 5 + pseudo_rand() % 6;
		for (int k = 0; k < length; ++k) {
			Word[i][k] = 'a' + pseudo_rand() % 26;
		}
		Word[i][length] = '\0';
	}
}

static void send_mail(int seed)
{
	char str[200];
	int pos = 0;

	mSeed = seed;

	int wcnt = 1 + pseudo_rand() % SW;
	for (int i = 0; i < wcnt; ++i) {
		int widx = pseudo_rand() % W;
		for (int k = 0; k < 10; ++k) {
			if (Word[widx][k] == '\0') break;
			str[pos++] = Word[widx][k];
		}
		str[pos++] = ' ';
	}
	str[pos - 1] = '\0';

	int uid = pseudo_rand() % N;
	int rcnt = minR + pseudo_rand() % (maxR - minR + 1);
	int rids[50];
	for (int i = 0; i < rcnt; ++i) {
		rids[i] = pseudo_rand() % N;
	}

	sendMail(str, uid, rcnt, rids);
}

static int delete_mail(int uid, int seed)
{
	char str[200];
	int pos = 0;

	mSeed = seed;

	int wcnt = 1 + pseudo_rand() % SW;
	for (int i = 0; i < wcnt; ++i) {
		int widx = pseudo_rand() % W;
		for (int k = 0; k < 10; ++k) {
			if (Word[widx][k] == '\0') break;
			str[pos++] = Word[widx][k];
		}
		str[pos++] = ' ';
	}
	str[pos - 1] = '\0';

	return deleteMail(uid, str);
}

static int run(int answer)
{
	int Q, K, cmd, sample, seed, param1, param2, ret;
	char str[30];

	scanf("%d %d %d %d", &Q, &sample, &N, &K);
	if (sample == 0) {
		scanf("%d %d %d %d %d", &W, &minR, &maxR, &SW, &seed);
		make_string(seed);
	}

	init(N, K);

	for (int i = 1; i < Q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case SENDMAIL:
			if (sample == 1) {
				int uid, rcnt;
				int rids[10];
				scanf("%s %d %d", str, &uid, &rcnt);
				for (int k = 0; k < rcnt; ++k) scanf("%d", &rids[k]);
				for (int k = 0; k < 30; ++k) {
					if (str[k] == '\0') break;
					if (str[k] == '_') str[k] = ' ';
				}
				if (max > MAX) {
					sendMail(str, uid, rcnt, rids);
				}
			}
			else {
				scanf("%d", &seed);
				send_mail(seed);
			}
			break;
		case GETCOUNT:
			scanf("%d %d", &param1, &param2);
			if (max > MAX) {
				ret = getCount(param1);
				if (ret != param2) {
					printf("ret = %d , ans=%d \n", ret, param2);
					answer = 0;
					exit(0);
				}
			}
			break;
		case DELETEMAIL:
			if (sample == 1) {
				int uid;
				scanf("%d %s %d", &param1, str, &param2);
				for (int k = 0; k < 30; ++k) {
					if (str[k] == '\0') break;
					if (str[k] == '_') str[k] = ' ';
				}
				if (max > MAX) {
					ret = deleteMail(param1, str);
					if (ret != param2) {
						printf("ret = %d , ans=%d \n", ret, param2);
						answer = 0;
						exit(0);
					}
				}
			}
			else {
				scanf("%d %d %d", &param1, &seed, &param2);
				if (max > MAX) {
					ret = delete_mail(param1, seed);
					if (ret != param2) {
						printf("ret = %d , ans=%d \n", ret, param2);
						answer = 0;
						exit(0);
					}
				}
			}
			break;
		case SEARCHMAIL:
			scanf("%d %s %d", &param1, str, &param2);
			if (max > MAX) {
				ret = searchMail(param1, str);
				if (ret != param2) {
					printf("ret = %d , ans=%d \n", ret, param2);
					answer = 0;
					exit(0);
				}
			}
			break;
		default:
			break;
		}
	}

	return answer;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int t = GetTickCount();

	int T, Mark;
	scanf("%d %d", &T, &Mark);

	for (int tc = 1; tc <= T; tc++) {
		max = tc; 
		printf("#%d %d\n", tc, run(Mark));
	}

	printf("time = %d ", GetTickCount() - t);

	return 0;
}
