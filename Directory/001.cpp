#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999

// The below commented functions are for your reference. If you want 
// to use it, uncomment these functions.

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
	for (int i = 0; i<len; i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
}

#define MAX_KEY 64
#define MAX_DATA 128
#define MAX_TABLE 4096
#define MDEP 100




#define DDD 1


#if DDD
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a,__FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define log(a,...)

#endif 
#define F(a,b,c)	for(register int a=b;a<=c;a++)


#define MD 50000
#define MSTR 6
#define MC 30


class Dir; 


int gdc = 0;

class Dir {
public:
	int did; 
	int dix; 
	char name[MSTR + 1]; 
	int cnum; 
	Dir * c[MC + 1]; 
	Dir * p; 
	int tnum; 

} dir[MD +1];


Dir * newDir(char name[]) {
	Dir * dp = &dir[gdc];
	dp->dix = gdc++;
	mstrcpy(dp->name, name);
	dp->cnum = dp->tnum = 0;
	F(i, 0, MC) dp->c[i] = 0;
	dp->p = 0;
	return dp;
}


Dir * root = &dir[0];

void init(int n) {
	log("++++\n");
	root->cnum = 0;

	log("---- \n\n");
}

typedef unsigned long long ll; 




void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
	log("++++ path = %s, name = %s \n", path, name); 
	
	Dir * p = 0; 
	Dir * c = 0; 

	if (mstrcmp(path, "/") == 0) {
		p = root; 
	}
	else {
		char *s, *e, c;
		s = e = ++path;
		while (1)
		{
			if (*e == '/' || *e == '\0') {

			}
		}

	}

	



		// 디렉토리를 추가 한다. 
		Dir * dp = newDir(name);


	log("---- \n\n");
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
	log("++++ path = %s, \n", path);
	// 



	log("---- \n\n");
}



void cmd_cp(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ spath = %s, dpath = %s \n", spath, dpath);




	log("---- \n\n");

}

void cmd_mv(char spath[PATH_MAXLEN + 1], char dpath[PATH_MAXLEN + 1]) {
	log("++++ spath = %s, dpath = %s \n", spath, dpath);

}

int cmd_find(char path[PATH_MAXLEN + 1]) {
	log("++++ path = %s\n", path);

	log("---- \n\n"); 

	return 0;
}



