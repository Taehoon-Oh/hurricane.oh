
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

#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__);
#define P(a,...)		printf(a,##__VA_ARGS__); 
#else
#define log(a,...)	
#define P(a)		
#endif 

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t; }


#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define Mpath PATH_MAXLEN
#define Mname	NAME_MAXLEN

#define Mdir 50000
typedef unsigned long long ll;
ll getlong(char * str)
{
	ll ret = 0; 
	while (*str++ != '\0') {
		ret = (ret << 5) + *str - 'a' + 1; 
	}
	return ret; 
}

int gd = 0; 
class Dir {
public:
	Dir * n;
	Dir * p;
	int cnum;
	ll data; 
	Dir * par;
	Dir * chd; 
	char name[NAME_MAXLEN + 1]; 
}dir[Mdir+1];

Dir * newDir(char * str) {
	Dir * dp = &dir[gd++];
	dp->data = getlong(str);
	dp->n = dp->p = dp->par = dp->chd = 0;
	dp->cnum = 0; 
	mstrcpy(dp->name, str);
	return dp;
}
 
void addlink(Dir *p, Dir *n) {
	if (p) p->n = n; 
	if (n) n->p = p;
}


Dir * root = &dir[0]; 
void init(int n) {
	log("++++");
	//F(i, 0, gd - 1) dir[i] = { 0, }; 
	root->name[0] = 'R';
	root->cnum = 0; 
	root->chd =0;
	root->par = 0;

	gd = 1; // 인덱스만 초기화 하고 실제 사용할때 초기화 하면서 쓰도록 한다. 
	log("----"); 
}



Dir * find(char path[]) {
	Dir * ret = root; 

	if (path[1] == '\0') return root; 

	char *e, *s, c; 
	s = e = path + 1; 
	
	while (*e != '\0') {
		if (*e =='/')
		{
			c = *e; 
			*e = 0; 
			ll temp = getlong(s); 

			ret = ret->chd;

			while (ret) {
				if (ret->data == temp)
				{
					break; 
				}
				ret = ret->n; 
			}

			*e = c; 
			s = ++e;
		}
		else
		{
			e++; 
		}
	}
	return ret; 
}



void adds(Dir * dst, Dir * src) {	
	while (dst->n)
	{
		dst = dst->n;
	}
	dst->n = src;
	src->par = dst->par; 

	dst = dst->p; // 현재의 부모값을 다 올려야 한다. 
	while (dst)dst->cnum += src->cnum + 1, dst = dst->p;
}

void addc(Dir *dst, Dir *src) {
	if (dst->chd) adds(dst->chd, src); 
	else
	{
		dst->chd = src;
		src->par = dst; 
		while (dst) dst->cnum +=src->cnum+1, dst = dst->par;
	}
}

void show(Dir * c) {
	if ( c != 0 ){
	P("%s -> ", c->name); 	
		show(c->n);		
		show(c->chd);
	}	
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
	log("++++path=%s, name = %s", path, name); 

	//find parent 
	Dir * par = find(path); 
	log("par=%s",par->name);

	Dir *nd = newDir(name); 
	log("new=%s", nd->name);

	addc(par, nd);

	show(root);
	log("----\n"); 

}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
	log("++++ %s", path );
	//find parent 
	Dir * dp = find(path);
	Dir * par = dp->par; 
	Dir * sib = par->chd; 

	while (sib != dp) sib = sib->n; 

	if (sib->p) sib->p->n = sib->n;
	else par->chd = sib->n; 

	if (sib->n) sib->n->p = sib->p;

	//par = dp->p; 
	while (par) par->cnum -= (dp->cnum + 1), par = par->p; 

	show(root);

	log("----\n");
}

void copyd(Dir * dst, Dir *src) {
	Dir * temp = newDir(src->name);
	addc(dst, temp);

	Dir * chd = src->chd;

	while (chd) {

		copyd(temp, chd);
		chd = chd->n;
	}
}


void cmd_cp(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ s=%s, d=%s ", spath, dpath);
	Dir * dst = find(dpath);
	Dir * src = find(spath);	

	copyd(dst, src); 

	show(root);

	log("----\n");

}

void cmd_mv(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ s=%s, d=%s ", spath, dpath);
	Dir * dst = find(dpath);
	Dir * src = find(spath);

	cmd_rm(spath);

	addc(dst, src);

	show(root);

	log("----\n");
}

int cmd_find(char path[PATH_MAXLEN + 1]) {
	log("++++ %s ", path);
	Dir * dp = find(path);
	int ret = dp->cnum; 


	log("----ret = %d\n", ret);
	return ret; 	
}

