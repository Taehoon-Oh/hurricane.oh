#define EE 1

#if EE

#include<stdio.h>
#define log(a,...)	printf ( "%10s[%03d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__ )

#else 
#define log(a,...)

#endif 


#define for(a,b,c)	for(register int a=b;a<=c;a++)
#define MAX 700

class Node {
public:
	char ch;
	Node* p, * n; 
}node[(MAX+1)*(MAX+1)] ;

int nc = 0; 

Node* newNode() {
	Node* np = &node[nc++]; 
	np->n = np->p = 0; 
	np->ch = 0; 
	return np; 
}

class Line {
public: 
	Node* head, * tail; 
	int len; 
	Line* p, * n; 
}line[MAX+1]	;

int lc = 0; 

Line* newLine() {
	Line* lp = &line[lc++]; 

	lp->head = lp->tail = 0; 
	lp->len = 0; 
	lp->n = lp->p = 0; 
	return lp; 
}

Line* TopLine;
Line* CurLine;
Node* CurNode; 


void init(int n) {
	log("+++ n= %d",n); 
	//공백을 하나 미리 만들어 두고 항상 준비 한다. 
	// 

	lc = nc = 0; 

	TopLine = newLine();
	CurLine = TopLine; 

	CurNode = newNode();

	// TO DO
	log("---");
}

void input_char(char ch) {
	// TO DO
	log("+++ = %c ", ch);

	Node* nNode = newNode();
	nNode->ch = ch; 

	nNode->p = CurNode->p; 
	nNode->n = CurNode; 

	if (CurNode ->p)
	{
		CurNode->p->n = nNode; 
	}
	else
	{
		CurLine->head = nNode; 
	}

	CurNode->p = nNode; 

	CurLine->len++; 

	// TO DO

	log("---");
}

void input_newline() {
	// TO DO
	log("+++");

	Line* nline = newLine(); 

	nline->p = CurLine; 
	nline->n = CurLine->n; 

	if (CurLine->n)
	{
		CurLine->n->p = nline; 
	}
	CurLine->n = nline; 

	CurLine->len -= 





	// TO DO

	log("---");
}

void move_cursor(int dir) { // 0: Up, 1: Down, 2: Left, 3: Right
	// TO DO
	log("+++ dir=%d	", dir);

	// TO DO

	log("---");
}

char get_char(int row, int col) {
	log("+++ row = %d, col = %d ", row, col);
	char ret = '?';	// TO DO : Need to be changed


	// TO DO

	log("--- ret = %c ", ret );
	return ret;
}
