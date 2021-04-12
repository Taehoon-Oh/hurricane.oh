template <typename T>
void Swap(T&a, T&b) {
	T t = a; a = b; b = t;
}
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))


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

#define MAX 50000
#define HASH (MAX*2+7) 
#define USER 1000

#define MAX_KEY 10
//#define MAX_DATA 128
#define MAX_TABLE HASH

typedef struct
{
	char key[MAX_KEY + 1];
	//char data[MAX_DATA + 1];
}Hash;
Hash tb[MAX_TABLE];

//아래가 기본인데 실제 고유한 값을 만들어 낼수만 있으면 된다. 
unsigned long hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	}

	return hash % MAX_TABLE;
}

//int find(const char *key, char *data)
int find(const char *key)
{
	unsigned long h = hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			//strcpy(data, tb[h].data);
			//return 1;
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	//return 0;
	return -1;  // 못찾았을때의 값이 필요하다. 
}


//int add(const char *key, char *data)
int add(const char *key)
{
	unsigned long h = hash(key);

	while (tb[h].key[0] != 0)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[h].key, key);
	//strcpy(tb[h].data, data);
	//return 1;
	return h;
}


class Msg {
public:
	int mid;
	int uid;
	int t;	
	// 아래는 링크드리스트간의 값을 비교 할때 아래 처럼 할 수도 있다. 
	/*
	bool operator <  ( Msg right) {
		if (t == right.t) return uid < right.uid;  //
		else return t > right.t;
	}
	*/

	//가장 최근에 생성된 메시지가 우선시 되고, 생성시간이같을 경우 유저 id 가 작을수록 우선시 된다.
	// user id 는 총 1000 까지 가능 1000 대략  2^10  
	// score = time <<10  - uid  즉 , time 을 <<10  (*1024) 를 해서 관리하면 시간 값이 같을 경우에만 uid 의 영향을 받게 된다. 
	int score; 

};

Msg msg[MAX + 1];
int gmc = 0;

Msg * newMsg(int mid, int t, int uid) {
	Msg * mp = &msg[gmc++];
	mp->mid = mid;
	mp->t = t;
	mp->uid = uid;
	//가장 최근에 생성된 메시지가 우선시 되고, 생성시간이같을 경우 유저 id 가 작을수록 우선시 된다.
	// user id 는 총 1000 까지 가능 1000 대략  2^10  
	// score = time <<10  - uid  즉 , time 을 <<10  (*1024) 를 해서 관리하면 시간 값이 같을 경우에만 uid 의 영향을 받게 된다. 	
	mp->score = (t << 10) - uid; // (t<<10) 괄호를 꼭 해야 한다 

	return mp;
}

class Link {
public:
	Msg * mp;
	Link *n;
	Link *p; // 나중에 prev 를 쓸일 없으면 막는다. 
};
Link link[(MAX * 12)];
int glc = 0;

Link * newLink(Msg * mp) {
	Link * lp = &link[glc++];
	lp->mp = mp;
	lp->n = lp->p = 0; // 나중에 prev 를 쓸일 없으면 막는다. 
	return lp;
}

#define garolink(left,right) (left)->n=right; (right)->p=left
//나중에 ->p (이전링크)를 쓸일 없다면 아래로  바꾼다. 
// #define garolink(left,right) (left)->n=right

Link userhead[USER + 1], usertail[USER + 1];
Link hashhead[HASH], hashtail[HASH];

int umap[USER + 1][USER + 1];

void init()
{
	for (register int i = 0; i <= USER; i++) {
		garolink(&userhead[i], &usertail[i]); 
	}
	for (register int i = 0; i <= HASH - 1; i++) {
		garolink(&hashhead[i], &hashtail[i]);
	}

	for (register int i = 0; i <= USER; i++) {
		for (register int j = 0; j <= USER; j++) umap[i][j] = 0;
		umap[i][0] = 1, umap[i][1] = i;
	}
	glc = gmc = 0;
}

void createMessage(int mid, int uid, char msgs[])
{
	int t = ((msgs[0] - '0') * 10 + (msgs[1] - '0')) * 3600 + ((msgs[3] - '0') * 10 + (msgs[4] - '0')) * 60 + ((msgs[6] - '0') * 10 + (msgs[7] - '0'));
	//가장 최근에 생성된 메시지가 우선시 되고, 생성시간이같을 경우 유저 id 가 작을수록 우선시 된다.
	// user id 는 총 1000 까지 가능 1000 대략  2^10  
	// score = time <<10  - uid  즉 , time 을 <<10  (*1024) 를 해서 관리하면 시간 값이 같을 경우에만 uid 의 영향을 받게 된다. 	
	Msg * mp = newMsg(mid, t, uid);
	Link * lp = newLink(mp);

	garolink(lp, userhead[uid].n); 
	garolink(&userhead[uid], lp );

	char *s, *e;
	char c;
	s = e = &msgs[8];

	while (1) {
		if (*e == ' ' || *e == 0)
		{
			c = *e;
			*e = 0;

			int h = add(s);
			lp = newLink(mp);

			garolink(lp, hashhead[h].n);
			garolink(&hashhead[h], lp);

			*e = c;
			if (*e == 0) break;
			s = ++e;
		}
		else e++;
	}
}

void followUser(int id1, int id2)
{
	int cnt = ++umap[id1][0];
	umap[id1][cnt] = id2;
}

//최고 5개를 구할때 방법. 
//이렇게 하지 않고 처음부터 링크드 리스트를 순서대로 관리할 수도 있다. 
class Ret {
public:
	int cnt;
	Msg*  list[5 + 1];

	void add(Msg *mp) {
		list[cnt++] = mp;

		for (register int i = cnt - 1; i > 0; i--)
		{
			//if (list[i]e  < list[i - 1])  // Score 를 쓰지 않고 원래대로 한다면.. Msg class 에서 class 간 비교 할 수 있도록  operator overriding 을 해서 사용이 가능하다. 
			if (list[i]->score > list[i - 1]->score)
			{
				Swap(list[i], list[i - 1]);
			}
			else
			{
				break;
			}
		}

		cnt = Min(cnt,5);
	}

};

int searchByHashtag(char name[], int rets[])
{
	Ret ret = { 0, };

	int h = find(name);
	if (h == -1) return ret.cnt;

	Link * lp = hashhead[h].n;

	while (lp!= &hashtail[h])
	{
		Msg  * mp = lp->mp;
		ret.add(mp);
		lp = lp->n;
	}

	for (register int i = 0; i <= ret.cnt - 1; i++)rets[i] = ret.list[i]->mid;
	return ret.cnt;
}

int getMessages(int uid, int rets[]) {
	Ret ret = { 0, };

	int fcnt = umap[uid][0];

	for (register int i = 1; i <= fcnt; i++) {
		int fid = umap[uid][i];

		Link * lp = userhead[fid].n;

		while (lp != &usertail[fid])
		{
			Msg  * mp = lp->mp;
			ret.add(mp);
			lp = lp->n;
		}
	}

	for (register int i = 0; i <= ret.cnt - 1; i++)rets[i] = ret.list[i]->mid;
	return ret.cnt;
}
