
void mstrcpy(char *dest, const char *src)
{
	register int i = 0;
	while (src[i] != 0) { dest[i] = src[i]; i++; }
	dest[i] = src[i];
}

int mstrcmp(const char *a, const char *b)
{
	register int i;
	for (i = 0; a[i] != 0; i++) { if (a[i] != b[i]) return a[i] - b[i]; }
	return a[i] - b[i];
}
#define DBG 0

#if DBG
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a , __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif 

typedef unsigned long long ll; 

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define MR 50
#define MM 10000
#define MSS 10
#define MSL  10
#define MU 1000
#define MHASH 10000
#define ML (MM*100)
#define MK 300

#define MAX_KEY MSL
#define MAX_TABLE (MHASH*2 +7)
#define NFND -9999999

//사용자수 최대 1000
// 단어 최대 10000
//  sendMail 최대 호출 10000
// 모든 함수 호출 30000; 

typedef struct
{
	char key[MAX_KEY + 1];
}Hash;
Hash tb[MAX_TABLE];

unsigned long _hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	}

	return hash % MAX_TABLE;
}

int find(const char *key)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFND;
}

int add(const char *key)
{
	unsigned long h = _hash(key);

	while (tb[h].key[0] != 0)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[h].key, key);	
	return h;
}

class MAIL {
public:
	int mid;
	int mix;
	int wcnt; 
	int whash[10];
};

MAIL mail[MM + 1];
int gmc = 0; 

MAIL * newMail() {
	MAIL * mp = &mail[gmc]; 
	mp->mid = 0; 
	mp->mix = gmc ; 
	mp->wcnt = 0; 
	F(i, 0, 9) mp->whash[0] = 0; 

	gmc++; 

	return mp; 
}

class LINK {
public: 
	MAIL *m;
	LINK *n;
	LINK *p; 
};

LINK link[ML + 1]; 
int glc = 0; 

LINK * newLink(MAIL * mp) {
	LINK * lp = &link[glc]; 
	lp->m = mp; 
	lp->p = lp->n = 0; 

	glc++; 
	return lp; 
}

LINK * user[MU + 1]; 

class USERINFO {
public:
	int uid;
	int cnt; 
	LINK * last;
};
USERINFO uinfo[MU + 1]; 

int N, K; 

void init(int n, int k)
{
	log("++++\n");

	

	F(i, 0, MU) uinfo[i] = { 0, }, user[i] = 0; 
	F(i, 0, glc - 1) link[i] = { 0, }; 
	F(i, 0, gmc - 1) mail[i] = { 0, };
	F(i, 0, MAX_TABLE - 1) tb[i].key[0] = 0; 

	N = n, K = k; 
	
	gmc = glc = 0; 
	log("----\n\n"); 
}

int makeword(char str[], int whash[]) {
	int ret = 0; 

	char * s, *e;
	s = e = str;
	char t;
	while (1)
	{
		if (*e == ' ' || *e == 0)
		{
			t = *e;
			*e = 0;

			int h = add(s);			
			whash[ret++] = h;			

			*e = t; // 원복 
			if (*e == 0)
			{
				break;
			}
			s = ++e;
		}
		else
		{
			e++;
		}
	}

	return ret; 
}

//메일별로 10개 이하 단어로 구성. 
int map[MM + 1][MK + 1]; 
//메시지별 해쉬값 번호 리스트를 저장합니다. 
// 특정 메시지

void sendMail(char str[], int sid, int rc, int rids[])
{
	log("++++ \n");
	log("str = %s \n",str); 
	log("sid = %d \n",sid);
	

	MAIL * mp = newMail(); 

	mp->wcnt = makeword(str, mp->whash); 

	F(i, 0, mp->wcnt - 1)log("whash[%d]= %d \n", i, mp->whash[i]);
	// word 개수와 리스트를 구했다. 
	// 나중에 단어 찾을때도 써먹자 

	F(i, 0, rc - 1) {
		LINK * lp = newLink(mp); 

		int uid = rids[i];
		uinfo[uid].cnt++; 
		if (!user[uid])
		{
			uinfo[uid].last = lp; 
		}

		lp->n = user[uid]; 
		if (user[uid]) 	user[uid]->p = lp;
		user[uid] = lp; 

		if (uinfo[uid].cnt > K) {
			uinfo[uid].cnt = K; 
			uinfo[uid].last = uinfo[uid].last->p;
			uinfo[uid].last->n = 0; // 맨 뒤꺼를 잘라낸다 			
		}
	}

	log("----\n\n");
}

int getCount(int uid)
{
	log("++++ uid = %d  (%d)\n", uid, uinfo[uid].cnt);
	int ret = 0; 

	ret = uinfo[uid].cnt;
	 
	log("----\n\n");
	return ret;
}

int deleteMail(int uid, char str[])
{
	log("++++ uid = %d, str = %s \n", uid, str);

	int ret = 0; 
	int ref_ids[10] = { 0, }; 

	MAIL temp = { 0, }; 
	temp.wcnt = makeword(str, temp.whash); 

	LINK * lp = user[uid];

	while (lp)
	{
		MAIL * mp = lp->m;
		if (mp->wcnt == temp.wcnt )
		{
			int pass = 1; 
			F(i, 0, mp->wcnt - 1) {
				if (temp.whash[i] != mp->whash[i]) {
					pass = 0; 
					break;
				}
			}
			if (pass)
			{
				ret++; 

				// 해당 리스트를 지웁니다. 
				int cnt = uinfo[uid].cnt; 
				if (cnt == 1) { // 하나면 
					uinfo[uid].last = 0; 
					user[uid] = 0; 
				}
				else if (uinfo[uid].last == lp) {	// 마지막이라면 
					uinfo[uid].last = uinfo[uid].last->p; // 한칸 앞으로 이동 
					uinfo[uid].last->n = 0; // 맨 뒤꺼를 잘라낸다 
				}
				else if( lp == user[uid]) // 맨 앞에꺼라면 
				{
					user[uid] = lp->n; // 다음껄로 지정. 
					user[uid]->p = 0; 
				}
				else {
					lp->n->p = lp->p; 
					lp->p->n = lp->n; 
				}
				uinfo[uid].cnt--; 
			}
		}
		
		lp = lp->n;
	}


	log("----\n\n");
	return ret;
}

int searchMail(int uid, char t[])
{
	log("++++uid = %d , str = %s \n", uid, t);
	int ret = 0; 

	LINK * lp = user[uid]; 

	int h = find(t); 
	if (h == NFND) return ret; 

	while (lp)
	{
		MAIL * mp = lp->m; 
		F(i, 0, mp->wcnt - 1) {
			if (h == mp->whash[i]) {
				ret++; 
				break;
			}
		}
		lp = lp->n; 
	}

	log("----\n\n");
	return ret;
}