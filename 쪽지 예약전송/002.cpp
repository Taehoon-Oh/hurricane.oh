
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

#define MAX 100000

class Msg {
public:
	int mid, sid, rid, time;
	bool read, sdel, idel;	
	Msg * sentnext, *sentprev;
	Msg * inboxnext, *inboxprev;
	void init() {
		mid = sid = rid = time = 0;
		read = sdel = idel = false;
		sentnext = sentprev = inboxnext = inboxprev = 0;
	}
	bool operator < (Msg in) {
		if (time == in.time) return mid > in.mid; 
		else return time > in.time; 
	}
}msg[MAX+1];

#define MU 1000

//#define MAX_SIZE (MAX+1)
//
//Msg* heap[MAX_SIZE];
//int heapSize = 0;
//
//void heapInit(void)
//{
//	heapSize = 0;
//}

typedef struct User {
	Msg inboxhead, inboxtail;
	Msg senthead, senttail;
	void init() {
		inboxhead.inboxnext = &inboxtail;
		inboxtail.inboxprev = &inboxhead;

		senthead.sentnext = &senttail;
		senttail.sentprev = &senthead;
	}
}user[MU+1];



#define MAXM	3

void init(int N)
{	
	F(i, 1, MAX) {
		msg[i].init();
	}
	F(i, 1, MU) {
		user[i].init(); 
	}
}

void sendMessage(int ct, int u1, int u2, int mid, int st)
{
	msg[mid] = { mid, u1, u2, st, false,false,false,0,0,0,0};	

	// U1 Sent 헤드에 추가 
	msg[mid].sentnext = user[u1].senthead.sentnext; 
	msg[mid].sentprev = &user[u1].senthead;
	user[u1].senthead.sentnext->sentprev = &msg[mid]; 
	user[u1].senthead.sentnext = &msg[mid];

	//U2 inbox  head 에 추가 
	msg[mid].inboxnext = user[u2].inboxhead.inboxnext;
	msg[mid].inboxprev = &user[u2].inboxhead;
	user[u2].inboxhead.inboxnext->inboxprev = &msg[mid];
	user[u2].inboxhead.inboxnext = &msg[mid];   	  	

}

int heapPop(Msg  *&value);
int heapPush(Msg * value);

int retrieveSentbox(int ct, int uid, int mlist[MAXM], int ulist[MAXM], int rlist[MAXM])
{
	int ret = 0;	
	Msg * head = user[uid].senthead.sentnext; 
	Msg * last = &user[uid].senttail;


	// Sent 는 현재 시간보다 다 이전에 send 한 것이니 굳이 시간을 확인할 필요가 없다. 
	while (head != last && ret <3)
	{
		if (!head->sdel) {
			mlist[ret] = head->mid; 
			ulist[ret] = head->rid;
			rlist[ret++] = head->read; 
		}
		head = head->sentnext; 
	}
	return ret;
}

template <typename T>
void swap(T &a, T&b) { T t = a; a = b; b = t; }
int retrieveInbox(int ct, int uid, int mlist[MAXM], int ulist[MAXM], int rlist[MAXM])
{
	int ret = 0;

	Msg * head = user[uid].inboxhead.inboxnext;
	Msg * last = &user[uid].inboxtail;

	Msg * retlist[4] = { 0, };

	while (head != last) {
		if (head->time <= ct && !head->idel) {
			retlist[ret++] = head; 
			NF(i, ret - 1, 1) {
				if (*retlist[i] < *retlist[i - 1]) {
					swap(retlist[i], retlist[i - 1]);
				}
				else break;
			}
			if (ret == 4) ret = 3; 
		}
		head = head->inboxnext; 

	}

	
	//heapInit(); 
	//while (head != last )
	//{
	//	if (head->time <= ct && !head->idel) {
	//		
	//	}
	//	head = head->inboxnext;
	//}


	F(i, 0, ret -1)
	{
		mlist[i] = retlist[i]->mid;
		ulist[i] = retlist[i]->sid;
		rlist[i] = retlist[i]->read;
	}

	return ret;
}

int readMessage(int ct, int uid, int mid)
{	
	// Sent 를 read 를 따로 관리할 필요는 없다. 
	if (msg[mid].sid == uid && !msg[mid].sdel) {
		return 1;		
	}
	//inbox 를 read 를 관리해줘야 한다. 
	else if (msg[mid].rid == uid && ! msg[mid].idel){
		if (msg[mid].time <= ct) {
			msg[mid].read = 1; 
			return 1; 
		}
	}
	return 0; 
}

int deleteMessage(int ct, int uid, int mid)
{
	// Sent 를 read 를 따로 관리할 필요는 없다. 
	if (msg[mid].sid == uid && !msg[mid].sdel) {
		if (msg[mid].time > ct) {
			msg[mid].idel = 1; // 미리지울수 있다. 
		}
		msg[mid].sdel = 1;
		msg[mid].sentprev->sentnext = msg[mid].sentnext; 
		msg[mid].sentnext->sentprev = msg[mid].sentprev;
		return 1;		
	}
	//inbox 를 read 를 관리해줘야 한다. 
	else if (msg[mid].rid == uid && !msg[mid].idel) {
		if (msg[mid].time <= ct) {
			msg[mid].idel = 1;
			msg[mid].inboxprev->inboxnext = msg[mid].inboxnext;
			msg[mid].inboxnext->inboxprev = msg[mid].inboxprev;
			return 1;
		}
	}
	return 0;
}

//
//int heapPush(Msg * value)
//{
//	if (heapSize + 1 > MAX_SIZE)
//	{
//		//printf("queue is full!");
//		return 0;
//	}
//
//	heap[heapSize] = value;
//
//	int current = heapSize;
//	while (current > 0 && *heap[current] < *heap[(current - 1) / 2])
//	{
//		Msg * temp = heap[(current - 1) / 2];
//		heap[(current - 1) / 2] = heap[current];
//		heap[current] = temp;
//		current = (current - 1) / 2;
//	}
//
//	heapSize = heapSize + 1;
//
//	return 1;
//}
//
//int heapPop(Msg  *&value)
//{
//	if (heapSize <= 0)
//	{
//		return false;
//	}
//
//	value = heap[0];
//	heapSize = heapSize - 1;
//
//	heap[0] = heap[heapSize];
//
//	int current = 0;
//	while (current * 2 + 1 < heapSize)
//	{
//		int child;
//		if (current * 2 + 2 == heapSize)
//		{
//			child = current * 2 + 1;
//		}
//		else
//		{
//			child = *heap[current * 2 + 1] < *heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
//		}
//
//		if (*heap[current] < *heap[child])
//		{
//			break;
//		}
//
//		Msg * temp = heap[current];
//		heap[current] = heap[child];
//		heap[child] = temp;
//
//		current = child;
//	}
//	return 1;
//}
