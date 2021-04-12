
#define DBG 0

#if DBG
#include <stdio.h>
#define log(...)	printf("%10s[%03d]:",__FUNCTION__,__LINE__);printf(__VA_ARGS__)
#else
#define log(...)	
#endif 
#define F(a,b,c)	for(int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void Swap(T &a, T &b) {
	T temp = a;
	a = b;
	b = temp;
}

#define M_USER      1000
#define M_TAG       5000
#define M_MSG		50000
#define M_LINK		(M_MSG*30 )

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

#define MAX_KEY 10
#define MAX_DATA 128
#define MAX_TABLE (M_TAG*2 +7)

typedef struct
{
	char key[MAX_KEY + 1];
}Hash;
Hash tb[MAX_TABLE + 1];

class MSG {
public:
	int mid;
	int uid;
	int time;

	int operator < (MSG * mp) {
		int ret = 0;
		int otime = time;
		int ntime = mp->time;

		if (ntime > otime || (ntime == otime && uid > mp->uid)) {
			ret = 1;
		}
		return ret;
	}
};

MSG msg[M_MSG + 1];
int gmc = 0;

MSG * newMsg(int mid, int uid, int time) {
	MSG * mp = &msg[mid];
	mp->mid = mid;
	mp->uid = uid;
	mp->time = time;

	return mp;
}

class LINK {
public:
	MSG * m;
	LINK * n;
	LINK * p;
};
LINK link[M_LINK + 1];
int glc = 1;
LINK * newLink(MSG * mp) {
	LINK * lp = &link[glc++];
	lp->m = mp;
	lp->n = 0;
	lp->p = 0;

	return lp;
}



LINK * user[M_USER + 1];
LINK * hash[MAX_TABLE + 1]; // hash 랑 숫자가 같으면 된다. 

#define M_RES 5  //최대 답변 개수 즉 각 리스트들도 사실 5개 이상 만들 필요는 없다. 

int count = 0;
int ids[M_RES]; // 5개 답을 저장할 곳안

int map[M_USER + 1][M_USER + 1];
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
	return -1;
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


void init()
{

	//1. msg 초기화 사용한 개수만큰만 초기화 하자. 
	F(i, 0, gmc) msg[i] = { 0,0,0 };

	//2. 사용한 link 는 초기화 하자. 
	F(i, 0, glc)	link[i] = { 0,0,0 };

	//3. 사용한 user id 별 링크 시작점.
	F(i, 0, M_USER)	user[i] = 0;

	//4. 사용한 hash 별 링크 시작점과 hash 정보 초기화 
	F(i, 0, MAX_TABLE) hash[i] = 0, tb[i].key[0] = 0;

	//5. 사용한 링크 원본 삭제 
	F(i, 0, glc) link[i] = { 0,0,0 };

	F(i, 0, M_USER) {
		F(j, 0, M_USER) map[i][j] = 0;
		map[i][0] = 1; // 자기 자신은 일단 넣고 보는데 나중에 걸러내야 한다. 
		map[i][1] = i;
	}

	gmc = glc = 0;
}

void createMessage(int mid, int uid, char msgs[])
{
	log("++++: mid=%d, uid =%d, msg=%s\n", mid, uid, msgs);

	int time = (msgs[0] * 10 + msgs[1]) * 3600 + (msgs[3] * 10 + msgs[4]) * 60 + (msgs[6] * 10 + msgs[7]);

	MSG * mp = newMsg(mid, uid, time);
	LINK * lp = newLink(mp);
	lp->n = user[uid];


	// user id와 해당 msg 를 링킹한다. 
	if (user[uid]) user[uid]->p = lp;
	user[uid] = lp;

	char * s, *e, t;
	s = e = &msgs[9];
	int h;

	while (1) { // 값이 있다면 
		switch (*e) {
		case 0:
		case ' ':
			t = *e;
			*e = 0;
			h = add(s);

			lp = newLink(mp);
			lp->n = hash[h];

			if (hash[h]) hash[h]->p = lp;
			hash[h] = lp;

			*e = t; // 원복한다 	
			s = e + 1; // starting  위치를 바꿔준다. 			
			break;
		}

		if (*e == 0) break;
		e++;
	}

	// hash 를 기반으로 link 추가  	

	log("----\n\n");
}

void followUser(int id1, int id2)
{
	log("++++: id1=%d -> id2= %d\n", id1, id2);

	int cnt = ++map[id1][0];

	map[id1][cnt] = id2;

	//log("id %d fllow = %d , %d added\n", id1, cnt, id2);
#if DBG
	log("id %d: ", id1);
	F(i, 1, cnt) printf("%d->", map[id1][i]);
	printf("\n");
#endif 
	log("----\n\n");
}

void add_list(int ni) {
	int idx = -1; // 값을 바꿔야 한다. 

	F(i, 0, count - 1) {
		int ref = ids[i];
		if (msg[ref] < &msg[ni]) {
			idx = i;
			break;
		}
	}

	if (idx == -1) { // 
		if (count < 5)	ids[count++] = ni;
	}
	else {
		for (int i = count; i > idx; i--) {
			ids[i] = ids[i - 1];
		}
		ids[idx] = ni;
		if (count < 5)count++;
	}
}

int searchByHashtag(char name[], int rets[])
{
	log("++++: name = %s\n", name);
	int ret = 0;
	count = 0;
	F(i, 0, 4)rets[i] = ids[i] = 0;

	int h = find(&name[1]);
	if (h == -1)return ret;

	LINK * lp = hash[h];
	while (lp) {
		add_list(lp->m->mid);
		lp = lp->n;
	}

	ret = count;
	F(i, 0, 4)rets[i] = ids[i];
	log("----: ret=%d[%d, %d, %d %d %d\n\n", ret, rets[0], rets[1], rets[2], rets[3], rets[4]);
	if (true)
	{

	}



	return ret;
}

int getMessages(int uid, int rets[])
{
	log("++++: uid=%d \n", uid);
	int ret = 0;
	count = 0;
	F(i, 0, 4)ids[i] = 0;

	F(i, 1, map[uid][0]) {
		int uuid = map[uid][i];
		LINK * lp = user[uuid];

		while (lp) {
			add_list(lp->m->mid);
			lp = lp->n;
		}
	}


	ret = count;
	F(i, 0, 4)rets[i] = ids[i];
	log("----: ret=%d[%d, %d, %d %d %d\n\n", ret, rets[0], rets[1], rets[2], rets[3], rets[4]);
	return ret;
}
