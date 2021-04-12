
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

//����ڼ� �ִ� 1000
// �ܾ� �ִ� 10000
//  sendMail �ִ� ȣ�� 10000
// ��� �Լ� ȣ�� 30000; 

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

			*e = t; // ���� 
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

//���Ϻ��� 10�� ���� �ܾ�� ����. 
int map[MM + 1][MK + 1]; 
//�޽����� �ؽ��� ��ȣ ����Ʈ�� �����մϴ�. 
// Ư�� �޽���

void sendMail(char str[], int sid, int rc, int rids[])
{
	log("++++ \n");
	log("str = %s \n",str); 
	log("sid = %d \n",sid);
	

	MAIL * mp = newMail(); 

	mp->wcnt = makeword(str, mp->whash); 

	F(i, 0, mp->wcnt - 1)log("whash[%d]= %d \n", i, mp->whash[i]);
	// word ������ ����Ʈ�� ���ߴ�. 
	// ���߿� �ܾ� ã������ ����� 

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
			uinfo[uid].last->n = 0; // �� �ڲ��� �߶󳽴� 			
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

				// �ش� ����Ʈ�� ����ϴ�. 
				int cnt = uinfo[uid].cnt; 
				if (cnt == 1) { // �ϳ��� 
					uinfo[uid].last = 0; 
					user[uid] = 0; 
				}
				else if (uinfo[uid].last == lp) {	// �������̶�� 
					uinfo[uid].last = uinfo[uid].last->p; // ��ĭ ������ �̵� 
					uinfo[uid].last->n = 0; // �� �ڲ��� �߶󳽴� 
				}
				else if( lp == user[uid]) // �� �տ������ 
				{
					user[uid] = lp->n; // �������� ����. 
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