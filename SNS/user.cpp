#define DBG 1

#if DBG

#include <stdio.h>
#define log(...)	printf("%15s[%03d]:	", __FUNCTION__, __LINE__);\
					printf(__VA_ARGS__);\
					printf("\n");          


#else
#define log(...)
#endif


#define M_U      1000
#define M_T       5000
#define M_C			100
#define M_M 50000
#define M_H 5000


#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))
#define F(a,b,c) for(int a =b;a<=c;a++)



typedef unsigned long long ll;



class MSG {
public:
	int mid , muid , mtime , mcnum;
	ll mclist[M_C + 1];
	MSG *un;
	MSG *hn;
};

MSG msg[M_M + 1]; 

class USR {
public:
	int uid;
	int ucnum;
	MSG * next;	   
};

USR usr[M_U + 1];

class HASH {
public:
	int hid;
	int hcnum;
	ll hval;
	MSG * next;
};

HASH hash[M_H + 1];






// The below commented functions are for your reference. If you want 
// to use it, uncomment these functions.

int mstrcmp(const char *a, const char *b)
{
int i;
for (i = 0; a[i] != '\0'; ++i) if (a[i] != b[i]) return a[i] - b[i];
return a[i] - b[i];
}

void mstrcpy(char *dest, const char *src)
{
int i = 0;
while (src[i] != '\0') { dest[i] = src[i]; i++; }
dest[i] = src[i];
}

int gmcnt;

extern void init() {
	log("++++\n");


	gmcnt = 0;
	F(i, 0, M_M) {
		msg[i].mid = 0;
		msg[i].muid = 0;
		msg[i].mclist[0] = 0;
		msg[i].hn = 0;
		msg[i].un = 0;
	}

	F(i,0, M_U) {
		usr[i].ucnum = 0;
		usr[i].uid = 0;
		usr[i].next = 0; 
	}
	F(i, 0, M_H) {
		hash[i].hcnum = 0;
	}
	gmcnt = 0; 

	log("----\n\n\n");
}


void createMessage(int msgID, int userID, char msgData[])
{
	log("++++"); 
	log("msgID=%d",msgID);
	msg[gmcnt].mid = msgID;
	msg[gmcnt].muid = userID;
	usr[userID].uid = userID;
	msg[gmcnt].un = usr[userID].next;
	usr[userID].next = &msg[gmcnt];
	
	usr[userID].ucnum++;
	gmcnt++; 

#if DBG
	printf("uid[%d]:", userID);
	MSG * pcur = usr[userID].next;
	F(i, 1, usr[userID].ucnum) {
		printf("%d ->", pcur->mid);
		pcur = pcur->un; 
	}
	printf("\n");
#endif 

	log("----\n\n\n");
}

void followUser(int userID1, int userID2)
{
	log("++++");


	log("----\n\n\n");
}

int searchByHashtag(char tagName[], int retIDs[])
{
	log("++++");


	log("----\n\n\n");
	return 0;
}

int getMessages(int userID, int retIDs[])
{
	log("++++");


	log("----\n\n\n");
	return 0;
}