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

#define DBG 0

#if DBG
//#include <stdio.h>
#define log(a)	printf(#a"\n\n");
#define logd(a)	printf(#a "=%d\n",a);
#define logs(a)	printf(#a "=%s\n",a);
#define logc(a)	printf(#a "=%c\n",a);

#else
//#include <stdio.h>

#define log(a)	
#define logd(a)
#define logs(a)
#define logc(a)

#endif

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))
#define F(a,b,c)	for(int a=b;a<=c;a++)


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
	if (!mstrcmp("vdsxucwmai@o.com", key))
	{
		int k = 9;
	}
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


class REC {
public:
	int ridx;
	int deleted;
	char str[MF][MSTR + 1];
};
REC rec[MR + 1];
int grc;
REC *newRec(int idx) {
	REC * rp = &rec[grc++];
	rp->ridx = grc - 1;
	rp->deleted = 0;

	return rp;
}

class LINK {
public:
	REC * r;
	LINK * n[MF];
	//LINK * p[MF]; // 스트링이 5개 있으니까 	
};

int glc = 0;
LINK link[MR + 1];
LINK * newLink(REC * rp) {
	LINK * lp = &link[glc++];
	lp->r = rp;

	F(i, 0, MEMO) lp->n[i] =  0;
	return lp;
}

LINK * hash[MF][MAX_TABLE];


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitDB()
{
	log(InitDB++++ + );


	F(i, 0, MAX_TABLE - 1) {
		F(j, 0, 4) hash[j][i] = 0, tb[j][i].key[0] = 0;		
	}

	F(i, 0,  glc - 1) link[i] = { 0, };
	F(i, 0, grc - 1) rec[i] = { 0, };

	grc = glc = 0;
	log(InitDB----);
}

void Add(char* name, char* num, char* day, char* mail, char* memo)
{
	log(ADD++);
	logs(name);	logs(num); logs(day); logs(mail); logs(memo);
	REC * rp = newRec(grc);
	mstrcpy(rp->str[NAME], name);
	mstrcpy(rp->str[NUM], num);
	mstrcpy(rp->str[DAY], day);
	mstrcpy(rp->str[MAIL], mail);
	mstrcpy(rp->str[MEMO], memo);

	LINK * lp = newLink(rp);
	F(i, 0, MEMO) {


		int h = add(rp->str[i], i);
		logs(rp->str[i]);
		logd(h);

		lp->n[i] = hash[i][h];
		log(hash[i][h]);

		//if (hash[i][h]) hash[i][h]->p[i] = lp;
		hash[i][h] = lp;
	}
	log(ADD--);
}

int Delete(FIELD field, char* str)
{
	log(Delete++);
	int ret = 0;

	int h = find(str, field);
	if (h == NFOUND)
	{
		return ret;
	}

	LINK * lp = hash[field][h];


	while (lp)
	{
		int ridx = lp->r->ridx;

		if (rec[ridx].deleted == 0 && !mstrcmp(str, lp->r->str[field])) {

			logd(ridx);
			logd(rec[ridx].deleted);


			rec[ridx].deleted = 1;
			logd(rec[ridx].deleted);

			ret++;
			logd(ret);

		}
		lp = lp->n[field];
	}
	log(Delete--);
	//printf("Delete: %d \n", cnt);
	return ret;
}

int Change(FIELD field, char* str, FIELD changefield, char* changestr)
{
	log(Change++);
	logd(field); logs(str); logd(changefield); logs(changestr);

	int ret = 0;

	int h = find(str,field );
	if (h == NFOUND)
	{
		return ret;
	}

	LINK * lp = hash[field][h];



	while (lp)
	{
		int ridx = lp->r->ridx;

		if (rec[ridx].deleted == 0 && !mstrcmp(str, lp->r->str[field])) {
			
			logd(ridx);
			logd(rec[ridx].deleted);


			logs(rec[ridx].str[changefield]);


			mstrcpy(rec[ridx].str[changefield], changestr); // 새로운 값으로 바꿔버리고 
			logs(rec[ridx].str[changefield]);


			int nh = add(rec[ridx].str[changefield], changefield);

			LINK * nlp = newLink(&rec[ridx]);

			nlp->n[changefield] = hash[changefield][nh];
			//if (hash[changefield][nh]) hash[changefield][nh]->p[changefield] = nlp;

			hash[changefield][nh] = nlp;

			ret++;
			logd(ret);

		}
		lp = lp->n[field];
	}

	log(Change--);

	//printf("Change: %d \n", cnt);
	return ret;
}

RESULT Search(FIELD field, char* str, FIELD ret_field)
{
	log(Search++);
	logd(field); logs(str); logd(ret_field);

	RESULT ret = { 0, };

	int h = find(str, field);
	if (h == NFOUND) return ret;

	LINK * lp = hash[field][h];

	
	while (lp) {
		int ridx = lp->r->ridx;

		if (rec[ridx].deleted == 0 && !mstrcmp(str, lp->r->str[field])) {// 지우지 않고 추가만 할 것이기 때문에 

			
			logd(ridx);
			logs(rec[ridx].str[ret_field]);
			logd(rec[ridx].deleted);

			mstrcpy(ret.str, rec[ridx].str[ret_field]); //해당 rec 의 요구한 인자를 답에 넣는다. 
			ret.count++;
			logd(ret.count);

		}
		lp = lp->n[field]; // 인풋 필드로 계속 검색해 나간다. 
	}

	log(Search--);
	//printf("Search: %d \n", cnt); 
	return ret;
}