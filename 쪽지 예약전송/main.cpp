#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define CMD_INIT				100
#define CMD_SEND_MESSAGE		200
#define CMD_RETRIEVE_SENTBOX	300
#define CMD_RETRIEVE_INBOX		400
#define CMD_READ_MESSAGE		500
#define CMD_DELETE_MESSAGE		600

#define MAXM					3

extern void init(int N);
extern void sendMessage(int cTimestamp, int uID1, int uID2, int mID, int scheduleTimestamp);
extern int  retrieveSentbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM]);
extern int  retrieveInbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM]);
extern int  readMessage(int cTimestamp, int uID, int mID);
extern int  deleteMessage(int cTimestamp, int uID, int mID);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#define MAX 5
int testcase; 

static bool run()
{
	int Q, N;
	int cTimestamp, scheduleTimestamp;
	int uID, uID1, uID2, mID;
	int ret, ans;
	int mIDList[MAXM], uIDList[MAXM], readList[MAXM];
	int mIDList_a[MAXM], uIDList_a[MAXM], readList_a[MAXM];

	bool okay = false;

	scanf("%d", &Q);

	for (int q = 0; q <= Q; ++q)
	{
		int op;
		scanf("%d", &op);

		switch (op)
		{
		case CMD_INIT:
			scanf("%d", &N);
			init(N);
			okay = true;
			break;
		case CMD_SEND_MESSAGE:
			scanf("%d %d %d %d %d", &cTimestamp, &uID1, &uID2, &mID, &scheduleTimestamp);
			if (okay)
				sendMessage(cTimestamp, uID1, uID2, mID, scheduleTimestamp);
			break;
		case CMD_RETRIEVE_SENTBOX:
			scanf("%d %d", &cTimestamp, &uID);
			if (okay)
				ret = retrieveSentbox(cTimestamp, uID, mIDList, uIDList, readList);
			scanf("%d", &ans);
			for (int i = 0; i < ans; ++i)
				scanf("%d %d %d", &mIDList_a[i], &uIDList_a[i], &readList_a[i]);
			if (testcase >= MAX) {
				if (ret != ans)
				{
					printf("ret = %d, ans = %d ", ret, ans); 
					exit(0); 
					okay = false;
				}
				else
				{
					for (int i = 0; i < ans; ++i)
						if (mIDList[i] != mIDList_a[i]
							|| uIDList[i] != uIDList_a[i] || readList[i] != readList_a[i]) {
							printf("ret = %d, ans = %d ", ret, ans);
							exit(0);
							okay = false;
						}
				}
			}
			break;
		case CMD_RETRIEVE_INBOX:
			scanf("%d %d", &cTimestamp, &uID);
			if (okay)
				ret = retrieveInbox(cTimestamp, uID, mIDList, uIDList, readList);
			scanf("%d", &ans);
			for (int i = 0; i < ans; ++i)
				scanf("%d %d %d", &mIDList_a[i], &uIDList_a[i], &readList_a[i]);
			if (testcase >= MAX) {
				if (ret != ans)
				{
					printf("ret = %d, ans = %d ", ret, ans);
					exit(0);
					okay = false;
				}
				else
				{
					for (int i = 0; i < ans; ++i)
						if (mIDList[i] != mIDList_a[i]
							|| uIDList[i] != uIDList_a[i] || readList[i] != readList_a[i]) {
							
							exit(0);
							okay = false;
						}
				}
			}
			break;
		case CMD_READ_MESSAGE:
			scanf("%d %d %d", &cTimestamp, &uID, &mID);
			if (okay)
				ret = readMessage(cTimestamp, uID, mID);
			scanf("%d", &ans);
			if (testcase >= MAX) {
				if (ret != ans) {
					printf("ret = %d, ans = %d ", ret, ans);
					exit(0);
					okay = false;
				}
			}
			break;
		case CMD_DELETE_MESSAGE:
			scanf("%d %d %d", &cTimestamp, &uID, &mID);
			if (okay)
				ret = deleteMessage(cTimestamp, uID, mID);
			scanf("%d", &ans);
			if (testcase >= MAX) {
				if (ret != ans) {
					printf("ret = %d, ans = %d ", ret, ans);
					exit(0);
					okay = false;
				}
			}
			break;
		}
	}

	return okay;
}

int main()
{
	int start = GetTickCount(); 
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);
	
	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		testcase = tc; 
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	start = GetTickCount() - start; 

	printf("time = %d \n",start );

	return 0;
}


/*
#define MAXM	3

void init(int N)
{

}

void sendMessage(int cTimestamp, int uID1, int uID2, int mID, int scheduleTimestamp)
{

}

int retrieveSentbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM])
{
	return -1;
}

int retrieveInbox(int cTimestamp, int uID, int mIDList[MAXM], int uIDList[MAXM], int readList[MAXM])
{
	return -1;
}

int readMessage(int cTimestamp, int uID, int mID)
{
	return -1;
}

int deleteMessage(int cTimestamp, int uID, int mID)
{
	return -1;
}
*/