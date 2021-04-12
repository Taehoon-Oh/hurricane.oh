
int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return a[i] - b[i];
}

int mstrncmp(const char *a, const char *b, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	return 0;
}

int mstrlen(const char *a)
{
	int len = 0;

	while (a[len] != '\0')
		len++;

	return len;
}

void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
}

void mstrncpy(char *dest, const char *src, int len)
{
	for (int i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
}

#define EE 0

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif 
typedef unsigned long long ll; 

template <typename T>
void Swap(T &a, T&b) { T t = a; a = b; b = t; }

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

#define MD 50000
#define MAX_KEY 6
#define MAX_TABLE (MD*2+7)

#define NFD -9999999

#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999

typedef struct
{
	char key[MAX_KEY + 1];
}_Hash;
_Hash tb[MAX_TABLE];

int find(const char *key);
int add(const char *key);
unsigned long _hash(const char *str);

#define MC 30
int gd= 0; 
class Dir {
public:
	char str[NAME_MAXLEN + 1]; 
	int n;
	int did;
	int h; 	
	int cn;
	Dir * c[MC + 1];
	Dir * p; 		 
}dir[MD+1];

Dir * newDir(char str[]) {
	Dir * dp = &dir[gd++]; 
	mstrcpy(dp->str, str); 
	dp->h = add(dp->str); 
	dp->did = gd - 1; dp->n = dp->cn = 0; 
	F(i, 0, MC) dp->c[i] = 0; 
	dp -> p = 0; 
	return dp; 
}



int N;
Dir * root = &dir[0]; 
void show(Dir * dp) {
#if EE
	int cnum = dp->cn; 
	if (cnum) {
		F(i, 0, cnum - 1) {
			//printf("->%s(%d) ", dp->c[i]->str, dp->c[i]->cn);
			show(dp->c[i]);
		}
	}
	else {
		Dir * par = dp; 
		while (par) {
			printf("%s(%d,%d)<- ", par->str, par->n, par->cn);
			par = par->p; 
		}
		printf ("\n");
	}

#endif 
}


void init(int n) {
	log("++++ n=%d",n);
	N = n; 
	

	F(i, 0, gd - 1) dir[i] = { 0, }; 
	root->str[0] = '/';
	gd = 1;
	F(i, 0, MAX_TABLE - 1) tb[i].key[0] = 0; 
	log("---- \n");
}

typedef unsigned long long ll;
Dir *getd(Dir * dp, char path[]) {	
	Dir * temp=dp; 
	char *s, *e, c; 
	s = e = path; 
	while (1) {
		if (*e == '\0') break; 
		else if (*e == '/' ) {
			c = *e; 
			*e = 0; 
			int h = find(s); 
			if (h == NFD) {
				log("error!!!!!!!!");
				break; 
			}
			
			//int cnum = dp->cn; 
			F(i, 0, temp->cn - 1) {
				if (temp->c[i]->h == h) { //   자식들 중에 이 이름을 가진 있다. 
					temp = temp->c[i];
					break; 
				}
			}
			*e = c; 
			s = ++e; 
		}
		else e++; 
	}
	log("return: str = %s, did %d ", temp->str, temp->did); 
	return temp;
}

Dir * makedir(Dir * par, char name[]) {
	
	Dir * child = newDir(name);
	
	log("child: did=%d , str=%s h= %d", child->did, child->str, child->h);

	par->c[par->cn++] = child;
	child->p = par;

		
	Dir *parent = child->p;
	while (parent) { 
		log("str = %s, n = %d", parent->str, parent->n);
		parent->n++; 
		log("str = %s, n = %d", parent->str, parent->n); 
		parent = parent->p;
	}	
	return child; 
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
	log("++++ path = %s, name = %s", path, name);
	Dir * dp; 
	if (path[1] == '\0')  dp= root;
	else dp= getd(root, path+1); 

	Dir * child = makedir(dp, name); 
	log("p: did=%d, n=%d, cn=%d, %s added", dp->did, dp->n, dp->cn, dp->c[dp->cn-1]->str	);


	log("---- \n");
	show(root); 
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
	log("++++ path = %s,", path);
	Dir * dp;
	if (path[1] == '\0')  dp = root;
	else dp = getd(root, path + 1);

	log("dp : str = %s ", dp->str); 

	Dir  * parent = dp->p;
	int pcnum = parent->cn--; //  자식 숫자 줄인다.미리 줄여 둔다. 
	F(i, 0, pcnum - 1) if (dp == parent->c[i])parent->c[i] = parent->c[pcnum - 1]; // 매 뒤에꺼를 땡긴다 //맨 뒤어꺼면 그자리 그대로인데 이미  개수를 줄여서 거기까지 가지 않게된다. 
	
	while (parent) {
		log("parent: str=%s, n= %d", parent->str, parent->n);
		parent->n -= (dp->n + 1);
		log("parent: str=%s, n= %d", parent->str, parent->n);
		parent = parent->p;
	}

	log("---- \n");
	show(root);
}

Dir * copyd(Dir * src, Dir * dst) {
	Dir * child = makedir(dst, src->str);
	
	log("%s copy to %s / ", src->str, dst->str); 

	int scnum = src->cn;
	F(i, 0, scnum - 1) {
		copyd( src->c[i], child);
	}

	return dst; 
}

void cmd_cp(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ spath = %s, dpath = %s", spath, dpath);

	Dir * src, *dst;
	if (spath[1] == '\0')  src = root;
	else src = getd(root, spath + 1);

	if (dpath[1] == '\0')  dst = root;
	else dst = getd(root, dpath + 1);


	Dir * nsrc = copyd(src, dst); 


	log("---- \n");
	show(root);

}

void cmd_mv(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ spath = %s, dpath = %s", spath, dpath);
	Dir * src, *dst;
	if (spath[1] == '\0')  src = root;
	else src = getd(root, spath + 1);

	Dir  * parent = src->p;
	int pcnum = parent->cn--; //  미리 줄여 둔다. 
	F(i, 0, pcnum - 1) {
		if (src == parent->c[i]) {
			parent->c[i] = parent->c[pcnum - 1]; // 매 뒤에꺼를 땡긴다 //맨 뒤어꺼
			break; 
		}
	}
	while (parent) {
		log("parent: str=%s, n= %d", parent->str, parent->n);
		parent->n -= (src->n + 1);
			log("parent: str=%s, n= %d", parent->str, parent->n);
		parent = parent->p;
	}


	if (dpath[1] == '\0')  dst = root;
	else dst = getd(root, dpath + 1);

	parent = dst;
	//pcnum = parent->n; //  미리 줄여 둔다.
	parent->c[parent->cn++] = src; 
	src->p = parent; 

	while (parent) {
		log("parent: str=%s, n= %d", parent->str, parent->n);
		parent->n += (src->n + 1);
			log("parent: str=%s, n= %d", parent->str, parent->n);
		parent = parent->p;
	}
	

	log("----\n"); 
	show(root);
}

int cmd_find(char path[PATH_MAXLEN + 1]) {
	log("++++ path = %s", path);
	int ret = 0; 
	Dir * dp;
	if (path[1] == '\0')  dp = root;
	else dp = getd(root, path + 1);

	ret = dp->n; 

	log("---- ret = %d\n",ret);
	show(root);
	return ret;
}



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
	return NFD;
}

int add(const char *key )
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