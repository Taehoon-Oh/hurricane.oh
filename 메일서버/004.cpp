#define EE 0


#if EE 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif
typedef unsigned long long ll; 

template <typename T>
void Swap(T& a, T &b) { T t = a; a = b; b = a; }

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))
#define F(a,b,c)	for (register int a=b;a<=c;a++)


int N, K;

#define MI 10
class Item {
public: 
	int cnt; 
	ll list[MI + 1]; 

	int operator == (Item temp) {
		if (cnt == temp.cnt) {
			F(i, 0, cnt - 1)if (list[i] != temp.list[i]) return 0;
			return 1;
		}
		else return 0;
	}
};

#define MM 10000
#define MU 1000
#define ML (MU*MM)
int gmc = 0; 
class Mail {
public: 
	Item item; 
	void init() {
		item.cnt = 0; 
	}

}mail[MM+1];

Mail * newMail() {
	Mail * mp = &mail[gmc++]; 
	return mp; 
}

int glc = 0; 
class Link {
public: 
	Mail * mp; 
	Link * n; 
	Link * p; 
	void init() {
		mp = 0,n = p = 0; 
	}
} link[ML+1];

Link * newLink(Mail * mp) {
	Link * lp = &link[glc++]; 
	lp->mp = mp; 
	lp->n = lp->n = 0; 
	return lp; 
}

class User {
public:
	int cnt; 
	Link head; 
	Link tail; 
	void init() {
		head.n = &tail; 
		tail.p = &head;
		head.p = tail.n = 0; 
		cnt = 0;
	}
}user[MU+1];




void init(int n, int k)
{
	N = n, K = k;

	F(i, 0, MU) user[i].init(); 
	F(i, 0, glc - 1) link[i].init(); 
	F(i, 0, gmc - 1)mail[i].init(); 

	glc = gmc = 0; 
	log("---- \n\n");
}

ll getlong(char str[]) {
	//log("str = %s", str); 
	ll ret = 0; 
	while (*str != 0 || *str != '\0') {
		ret = (ret << 5) + *str - 'a' + 1; 
		str++; 
	}
	//log("ret = %lld", ret); 
	return ret; 
}


Item getlist(char str[]) {
	Item ret = { 0, }; 

	char *e, *s, c; 
	s = e = str; 

	while (1) {
		if (*e == ' ' || *e == '\0')
		{
			c = *e;
			*e = 0;

			ret.list[ret.cnt++] = getlong(s); 
			*e = c; 
			if (*e == '\0') break; 

			s = ++e; 
		}
		else e++; 
	}

	return ret; 
}

void sendMail(char str[], int uid, int cnt, int ids[])
{
	log("++++ str = %s uid = %d, cnt = %d ", str, uid, cnt);
	Mail * mp = newMail(); 

	mp->item = getlist(str); 

	F(i, 0, cnt - 1) {
		int uid = ids[i];

		Link * lp = newLink(mp); 

		lp->n = user[uid].head.n; 
		lp->p = &user[uid].head; 
		if (user[uid].head.n) user[uid].head.n->p = lp; 
		user[uid].head.n = lp; 		
		

		if (user[uid].cnt ==K) {
			lp = user[uid].tail.p; 
			lp->n->p = lp->p; 
			lp->p->n = lp->n; 
		}
		else user[uid].cnt++; 
		log("uid = %d, cnt = %d ", uid, user[uid].cnt);
	}

	log("---- \n");
}

int getCount(int uid)
{
	log("++++ uid = %d ", uid);
	int ret = 0; 
	ret = user[uid].cnt; 

	log("---- %d \n", ret);
	return ret;
}

int deleteMail(int uid, char str[])
{
	log("++++ uid = %d , str = %s", uid, str);
	int ret = 0;
	Item in = getlist(str); 

	Link * lp = user[uid].head.n; 

	while (lp != &user[uid].tail) {
		Item temp = lp->mp->item; 
		if (in == temp) { //  
			lp->n->p = lp->p; 
			lp->p->n = lp->n; 
			user[uid].cnt--; 
			ret++; 
		}
		lp = lp->n; 
	}


	log("---- ret=%d  uid=%d, cnt = %d, \n", ret, uid, user[uid].cnt);
	return ret;
}

int searchMail(int uid, char text[])
{
	log("++++ uid = %d , text = %s", uid, text);
	int ret = 0;
	ll in = getlong(text); 
	log("in = %lld", in); 

	Link * lp = user[uid].head.n;

	while (lp != &user[uid].tail) {
		F(i,0,lp->mp->item.cnt -1 ) { // 
			if (lp->mp->item.list[i] == in) {
				ret++;
				log("list %lld ", lp->mp->item.list[i]);
				break; 
			}
		}
		lp = lp->n;
	}
	log("---- %d \n", ret);
	return ret;
}