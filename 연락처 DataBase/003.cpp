void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != 0) { dest[i] = src[i]; i++; }
	dest[i] = src[i];
}

int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != 0; i++) { if (a[i] != b[i]) return a[i] - b[i]; }
	return a[i] - b[i];
}

#define DBG 1

#if DBG
#include <stdio.h>
#define log(a,...) printf("%10s[%03d]: " a ,__FUNCTION__,__LINE__,##__VA_ARGS__)

#else
#define log(a,...)
#define logd(a)
#define logc(a)
#define logs(a)

#endif

#define F(a,b,c)	for(int a=b;a<=c;a++)
#define NF(a,b,c)	for(int a=b;a>=c;a--)


typedef enum
{
	NAME,
	NUM,
	DAY,
	MAIL,
	MEMO
} FIELD;
#define MF	5
#define MSTR	19
#define MR 50000

#define MAX_KEY MSTR
#define MAX_TABLE (MR*2+7)

typedef struct
{
	char key[MAX_KEY + 1];
}Hash;
Hash tb[MF][MAX_TABLE];
#define NFOUND -9999999



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

int find(const char *key, int f)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	while (tb[f][h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[f][h].key, key) == 0)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFOUND;
}

int add(const char *key, int f)
{
	unsigned long h = _hash(key);

	while (tb[f][h].key[0] != 0)
	{
		if (mstrcmp(tb[f][h].key, key) == 0)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[f][h].key, key);
	return h;
}




typedef struct
{
	int count;
	char str[20];
} RESULT;



class MSG {
public:
	int mid;
	int midx;
	char str[MF][MSTR + 1];
	MSG * p[MF];
	MSG * n[MF];

	void show(int f) {
#if DBG
		MSG * mp = this;
		while (mp) {
		log("mid= %d, midx= %d \n", mid, midx);
		F(i, 0, MEMO) printf("%s(%d)", str[i],i);
		printf("\n");

		mp = mp->n[f]; 
	    }
#endif 
	}
};

int gmc = 0;
MSG msg[MR + 1];

MSG * newMsg() {
	MSG * mp = &msg[gmc];
	mp->mid = 0;
	mp->midx = gmc++;
	F(i, 0, MEMO)	mp->str[i][0] = 0, mp->n[i] = mp->p[i] = 0;
	return mp;
}


MSG * hash[MF][MAX_TABLE];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitDB()
{
	log("++++\n");

	F(i, 0, gmc) msg[i] = { 0, };
	F(i, 0, MEMO) F(j, 0, MAX_TABLE - 1) tb[i][j].key[0] = 0, hash[i][j] = 0;

	gmc = 0;
	log("----\n\n");
}

void Add(char* name, char* num, char* day, char* mail, char* memo)
{
	log("++++ name = %s, num = %s, day = %s, mail = %s, meme = %s\n", name, num, day, mail, memo);
	MSG *mp = newMsg();
	mp->midx = gmc - 1;

	mstrcpy(mp->str[NAME], name);
	mstrcpy(mp->str[NUM], num);
	mstrcpy(mp->str[DAY], day);
	mstrcpy(mp->str[MAIL], mail);
	mstrcpy(mp->str[MEMO], memo);

	F(i, 0, MEMO) {
		int h = add(mp->str[i], i);

		mp->n[i] = hash[i][h];

		if (hash[i][h])	hash[i][h]->p[i] = mp; 
		hash[i][h] = mp;
	}

	log("----\n\n");
}

int Delete(FIELD f, char* str)
{
	log("++++\n");
	int ret = 0;
	int h = find(str, f);
	if (h == NFOUND) return ret;

	MSG * mp = hash[f][h];
	while (mp) {

		F(i, 0, MEMO) {
			

			if (mp->n[f])  mp->n[f]->p[f] = mp->p[f];
			if (mp->p[f])  mp->p[f]->n[f] = mp->n[f];
			if (!mp->n[f] && !mp->p[f]) hash[f][h] = 0;
		}
		ret++; 
		mp =mp -> n[f]; 
	}

	log("----\n\n");
	return ret;
}

int Change(FIELD f, char* str, FIELD cf, char* cstr)
{
	log("++++\n");
	int ret = 0;
	int h = find(str, f);
	if (h == NFOUND) return ret;

	
	MSG * mp = hash[f][h];
	mp->show(f);
	while (mp) {

		mp->show(cf); 
		if (mp->n[cf])  mp->n[cf]->p[cf] = mp->p[cf];
		if (mp->p[cf])  mp->p[cf]->n[cf] = mp->n[cf];
		mp->n[cf] = 0; 
		mp->show(cf);

		mstrcpy(mp->str[cf], cstr); 

		h = add(cstr, cf);
		mp->show(cf);
		mp->n[cf] = hash[cf][h]; 

		if (hash[cf][h])	hash[cf][h]->p[cf] = mp; 

		hash[cf][h] = mp; 

		ret++; 
		mp = mp->n[f];

	}


	log("----\n\n");
	return ret;
}

RESULT Search(FIELD f, char* str, FIELD rf)
{
	log("++++\n");
	RESULT ret = { 0, };

	int h = find(str, f);
	if (h == NFOUND) return ret;

	MSG * mp = hash[f][h];

	while (mp) {

		if (!ret.count) mstrcpy(ret.str, mp->str[rf]);
		ret.count++;
		mp = mp->n[f];
	}


	log("----\n\n");
	return ret;
}