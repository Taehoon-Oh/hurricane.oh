#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#else 
#define log(a,...)

#endif 

#define F(a,b,c) for(register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

template <typename T>
void addlink(T& l, T& r) { l->n = r; r->p = l; }

class Line; 
class User; 

class Line {
public:
	int linelen;
	int usercnt; 
	User* n;
}line[1000];


class User {
public:
	int lineid;
	int s;
	int e;
	int conf; 
}user[1000];


int W, L; 
int Lcnt; 
void Init(int w, int l, char str[])
{
	log("++++ w=%d, l=%d, str = %s", w,l,str); 
	W = w, L = l; 

	int linelen = 0; 
	int linecnt = 0; 
	F(i, 0, l - 1) {
		if (str[i] == '\n') {
			line[linecnt].linelen = linelen; 
			linelen = 0; 
			linecnt++; 
		}
		else
		{
			linelen++; 
		}
	}
	Lcnt = linecnt; 

	

	log("---- Lcnt = %d \n", Lcnt); 
}

void Edit(int id, int r, int c)
{
	log("++++id = %d , r=%d, c=%d", id, r, c);

	User* up = &user[id]; 

	int curline = 0; 
	F(i, 0, Lcnt - 1) {
		int templine = (line[i].linelen - 1) / W + 1; 
	}




	log("----\n");
}

void Add(int id, int l, char str[])
{
	log("++++ id = %d, l=%d, str=%s", id, l, str);

	log("----\n");
}

void Delete(int id, int l)
{
	log("++++ id = %d, l =%d ", id , l );

	log("----\n");
}

void Substitute(int id, int l, char str[])
{
	log("++++id =%d, l=%d, str = %s ", id, l , str);

	log("----\n");
}

int EditDone(int id)
{
	int ret = 0; 
	log("++++ id = %d", id );

	log("---- ret = %d \n", ret);
	return ret;
}






